#include"RTC.h"
#include"Uart.h"
#include"Virtual_Task.h"

unsigned int RTC::Time; ///在此声明RTC静态成员变量，定义了任务之后在赋初值...

extern "C" int HandleEINT0; ///中断向量表首地址...在汇编文件中定义

RTC::RTC( )
{
    // time_t CuTime={0x20,0x12,0x03,0x23,0x05,0x20,0x42,0x30};///设置当前时间为：2012/3/22星期4 13:22:30
    int *pIrq = &HandleEINT0;

    pIrq += 8;  ///安装8号中断...
    *pIrq = (int)(Virtual_Task::_INT_TICK);

    Set_Bit(rRTCCON, 0); //RTC控制使能,允许读写

    rTICNT = 2 | (0 << 7); //禁止时间中断片,时间片计数值为90

    //Set_Time(CuTime); //设置当前时间
    Clr_Bit(rINTMSK , 8); //中断屏蔽寄存器中的INT_TICK位设为允许

    Clr_Bit(rRTCCON, 0); //RTC禁止读写
}

RTC::~RTC()
{

}

int   RTC::Set_Time(time_t  &CuTime) ///实现纯虚类的纯虚函数...
{
    Set_Bit(rRTCCON, 0); //RTC控制使能,允许读写

    rBCDYEAR = CuTime.year_ ;		//年
    rBCDMON  = CuTime.month ;		//月
    rBCDDATE = CuTime.date ;		//日
    rBCDDAY  = CuTime.week ;		//星期
    rBCDHOUR = CuTime.hour ;		//小时
    rBCDMIN  = CuTime.minute ;		//分
    rBCDSEC  = CuTime.second ;		//秒

    Clr_Bit(rRTCCON, 0); //RTC禁止读写
    return 0;
}

int  RTC::Get_Time(time_t &CuTime)
{
    Set_Bit(rRTCCON, 0); //RTC控制使能,允许读写

    CuTime._year  =  0x20 ;	 ///现在是21世纪
    CuTime.year_  = rBCDYEAR ;		//年
    CuTime.month  = rBCDMON ;		//月
    CuTime.date    = rBCDDATE ;		//日
    CuTime.week   = rBCDDAY ;		//星期
    CuTime.hour   = rBCDHOUR ;		//小时
    CuTime.minute = rBCDMIN ;		//分
    CuTime.second = rBCDSEC ;		//秒

    Clr_Bit(rRTCCON, 0); //RTC禁止读写

    return 0;
}

