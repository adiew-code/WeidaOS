#include"RTC.h"
#include"Uart.h"
#include"Virtual_Task.h"

unsigned int RTC::Time; ///�ڴ�����RTC��̬��Ա����������������֮���ڸ���ֵ...

extern "C" int HandleEINT0; ///�ж��������׵�ַ...�ڻ���ļ��ж���

RTC::RTC( )
{
    // time_t CuTime={0x20,0x12,0x03,0x23,0x05,0x20,0x42,0x30};///���õ�ǰʱ��Ϊ��2012/3/22����4 13:22:30
    int *pIrq = &HandleEINT0;

    pIrq += 8;  ///��װ8���ж�...
    *pIrq = (int)(Virtual_Task::_INT_TICK);

    Set_Bit(rRTCCON, 0); //RTC����ʹ��,�����д

    rTICNT = 2 | (0 << 7); //��ֹʱ���ж�Ƭ,ʱ��Ƭ����ֵΪ90

    //Set_Time(CuTime); //���õ�ǰʱ��
    Clr_Bit(rINTMSK , 8); //�ж����μĴ����е�INT_TICKλ��Ϊ����

    Clr_Bit(rRTCCON, 0); //RTC��ֹ��д
}

RTC::~RTC()
{

}

int   RTC::Set_Time(time_t  &CuTime) ///ʵ�ִ�����Ĵ��麯��...
{
    Set_Bit(rRTCCON, 0); //RTC����ʹ��,�����д

    rBCDYEAR = CuTime.year_ ;		//��
    rBCDMON  = CuTime.month ;		//��
    rBCDDATE = CuTime.date ;		//��
    rBCDDAY  = CuTime.week ;		//����
    rBCDHOUR = CuTime.hour ;		//Сʱ
    rBCDMIN  = CuTime.minute ;		//��
    rBCDSEC  = CuTime.second ;		//��

    Clr_Bit(rRTCCON, 0); //RTC��ֹ��д
    return 0;
}

int  RTC::Get_Time(time_t &CuTime)
{
    Set_Bit(rRTCCON, 0); //RTC����ʹ��,�����д

    CuTime._year  =  0x20 ;	 ///������21����
    CuTime.year_  = rBCDYEAR ;		//��
    CuTime.month  = rBCDMON ;		//��
    CuTime.date    = rBCDDATE ;		//��
    CuTime.week   = rBCDDAY ;		//����
    CuTime.hour   = rBCDHOUR ;		//Сʱ
    CuTime.minute = rBCDMIN ;		//��
    CuTime.second = rBCDSEC ;		//��

    Clr_Bit(rRTCCON, 0); //RTC��ֹ��д

    return 0;
}

