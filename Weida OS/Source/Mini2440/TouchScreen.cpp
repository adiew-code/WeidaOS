#include"TouchScreen.h"
#include"Uart.h"
extern "C" int HandleEINT0; ///�ж��������׵�ַ...�ڻ���ļ��ж���

Point TouchScreen::Position[2];
bool  TouchScreen::State;
TouchScreen::TouchScreen()
{
    int *pIrq = &HandleEINT0;

    pIrq += 31;  ///��װ31���ж�...
    *pIrq = (int)(TouchSc_handle); // SetIRQ(TouchSc_handle, 31); ///��װ�������жϴ�����

    rADCDLY = 50000;          //ADC��ʱʱ��
    rADCCON = (1 << 14) + (9 << 6); //PRSCEN=1 PRSCVL=9

    Clr_Bit(rINTMSK , 31); //�ж����μĴ����е�ADCλ��Ϊ����
    Clr_Bit(rINTSUBMSK , 9 );	//���ж����μĴ�����TCλ��Ϊ����
    Clr_Bit(rINTSUBMSK , 10 );	//���ж����μĴ�����ADC_Sλ��Ϊ����


    rADCTSC = 0xd3;            //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

}

TouchScreen::~TouchScreen()
{



}

void __irq TouchScreen::TouchSc_handle()
{
    if( rSUBSRCPND & (1 << 9) ) ///����Ǵ����������»��ͷŶ�������INT_TC�ж�...
    {
        INT_TC();
    }
    else ///if( rSUBSRCPND & (1<<10) ) //ADת����ɷ�����INT_ADC_S�ж�
    {
        INT_ADC_S();

    }
    Set_Bit(rSRCPND , 31); //д1����ж�
    Set_Bit(rINTPND , 31); //д1����ж�

}

void TouchScreen::INT_TC()
{
    //UART Uart0;
    if(rADCDAT0 & 0x8000) ///�ɿ�
    {
        ///Uart0.Uart_PutStr("\nStylus Up!!");
        State = false;

        rADCTSC = 0xd3;
        Clr_Bit (rADCTSC, 8); ////����UD_SENΪ0��ʾ ̽���Ƿ���״̬

    }
    else  ///����
    {
        /// Uart0.Uart_PutStr("\nStylus Down!!");
        State = true;
        Set_Bit( rADCCON , 0); //��ADC��ʼ,����Ϊ�Զ�ת������0λ��1
        rADCTSC = (1 << 2) | (1 << 3);

        Set_Bit(rADCTSC, 8);	 //����UD_SENΪ1��ʾ ̽���Ƿ��ɿ�״̬ ���Ǳ���Σ�
    }
    Set_Bit(rSUBSRCPND, 9);	 //�ھ�λд1��ʾ���
}

void TouchScreen::INT_ADC_S()
{
    ////UART Uart0;

    Position[0].x = (rADCDAT0 & 0x3ff); ///����X����
    Position[0].y = (rADCDAT1 & 0x3ff); ///����Y����


    /// Uart0.Uart_PutStr("ADC Conversion Success! X=%d,Y=%d!!\n", Position[0].x, Position[0].y);

    rADCTSC = 0xd3; //����������������Ϊ�ȴ��ж�ģʽ

    Set_Bit(rADCTSC , 8);  //����UD_SENΪ1��ʾ ̽���Ƿ��ɿ�״̬ ���Ǳ���Σ�

    Set_Bit(rSUBSRCPND, 10);	 //��10λд1��ʾ���
}