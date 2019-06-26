#ifndef  __WEIDA_VIRTUAL_RTC_H
#define  __WEIDA_VIRTUAL_RTC_H
/***********************************************************************************
			虚拟实时时钟......
*************************************************************************************/
#include"Config.h"
#include"DataTypes.h"

typedef struct
{
    unsigned char  _year;///年数一般是四个数，这里存放前两位 如果是1991年，这里存放0x19

    unsigned char  year_ ; ///年数的后两位,如果是1991年，这里存放0x91
    unsigned char month; //月
    unsigned char  date ;	//日
    unsigned char  week ; ///星期
    unsigned char hour ;
    unsigned char  minute;
    unsigned char  second ;
} time_t;


class Virtual_RTC
{

public:
    Virtual_RTC();
    ~Virtual_RTC();
    virtual int  Set_Time(time_t  &CurTime) = 0; //将CurTime设置为当前系统时间....
    virtual int  Get_Time(time_t &CurTime) = 0; ///得到当前时间并存放到CurTime

protected:
};

/********************************************************************************/
#endif