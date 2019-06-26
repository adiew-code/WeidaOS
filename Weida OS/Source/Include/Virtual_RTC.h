#ifndef  __WEIDA_VIRTUAL_RTC_H
#define  __WEIDA_VIRTUAL_RTC_H
/***********************************************************************************
			����ʵʱʱ��......
*************************************************************************************/
#include"Config.h"
#include"DataTypes.h"

typedef struct
{
    unsigned char  _year;///����һ�����ĸ�����������ǰ��λ �����1991�꣬������0x19

    unsigned char  year_ ; ///�����ĺ���λ,�����1991�꣬������0x91
    unsigned char month; //��
    unsigned char  date ;	//��
    unsigned char  week ; ///����
    unsigned char hour ;
    unsigned char  minute;
    unsigned char  second ;
} time_t;


class Virtual_RTC
{

public:
    Virtual_RTC();
    ~Virtual_RTC();
    virtual int  Set_Time(time_t  &CurTime) = 0; //��CurTime����Ϊ��ǰϵͳʱ��....
    virtual int  Get_Time(time_t &CurTime) = 0; ///�õ���ǰʱ�䲢��ŵ�CurTime

protected:
};

/********************************************************************************/
#endif