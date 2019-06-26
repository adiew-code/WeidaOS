#include"TouchScreen.h"
#include"Uart.h"
extern "C" int HandleEINT0; ///中断向量表首地址...在汇编文件中定义

Point TouchScreen::Position[2];
bool  TouchScreen::State;
TouchScreen::TouchScreen()
{
    int *pIrq = &HandleEINT0;

    pIrq += 31;  ///安装31号中断...
    *pIrq = (int)(TouchSc_handle); // SetIRQ(TouchSc_handle, 31); ///安装触摸屏中断处理函数

    rADCDLY = 50000;          //ADC延时时间
    rADCCON = (1 << 14) + (9 << 6); //PRSCEN=1 PRSCVL=9

    Clr_Bit(rINTMSK , 31); //中断屏蔽寄存器中的ADC位设为允许
    Clr_Bit(rINTSUBMSK , 9 );	//子中断屏蔽寄存器的TC位设为允许
    Clr_Bit(rINTSUBMSK , 10 );	//子中断屏蔽寄存器的ADC_S位设为允许


    rADCTSC = 0xd3;            //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En

}

TouchScreen::~TouchScreen()
{



}

void __irq TouchScreen::TouchSc_handle()
{
    if( rSUBSRCPND & (1 << 9) ) ///如果是触摸屏被按下或释放而发出的INT_TC中断...
    {
        INT_TC();
    }
    else ///if( rSUBSRCPND & (1<<10) ) //AD转换完成发出的INT_ADC_S中断
    {
        INT_ADC_S();

    }
    Set_Bit(rSRCPND , 31); //写1清除中断
    Set_Bit(rINTPND , 31); //写1清除中断

}

void TouchScreen::INT_TC()
{
    //UART Uart0;
    if(rADCDAT0 & 0x8000) ///松开
    {
        ///Uart0.Uart_PutStr("\nStylus Up!!");
        State = false;

        rADCTSC = 0xd3;
        Clr_Bit (rADCTSC, 8); ////设置UD_SEN为0表示 探测是否按下状态

    }
    else  ///按下
    {
        /// Uart0.Uart_PutStr("\nStylus Down!!");
        State = true;
        Set_Bit( rADCCON , 0); //打开ADC开始,设置为自动转换，第0位置1
        rADCTSC = (1 << 2) | (1 << 3);

        Set_Bit(rADCTSC, 8);	 //设置UD_SEN为1表示 探测是否松开状态 这是必须滴，
    }
    Set_Bit(rSUBSRCPND, 9);	 //第九位写1表示清除
}

void TouchScreen::INT_ADC_S()
{
    ////UART Uart0;

    Position[0].x = (rADCDAT0 & 0x3ff); ///保存X数据
    Position[0].y = (rADCDAT1 & 0x3ff); ///保存Y数据


    /// Uart0.Uart_PutStr("ADC Conversion Success! X=%d,Y=%d!!\n", Position[0].x, Position[0].y);

    rADCTSC = 0xd3; //继续将触摸屏设置为等待中断模式

    Set_Bit(rADCTSC , 8);  //设置UD_SEN为1表示 探测是否松开状态 这是必须滴，

    Set_Bit(rSUBSRCPND, 10);	 //第10位写1表示清除
}