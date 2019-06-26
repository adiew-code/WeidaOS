#ifndef  __RTC_H
#define  __RTC_H
/***********************************************************************************/
#include"Virtual_RTC.h"
#include"Virtual_Task.h"
#include"Address.h"

class RTC: public Virtual_RTC ///¥”¥ø–È¿‡ºÃ≥–
{
public:

    static unsigned int Time;

    RTC();
    ~RTC();
    /// static void __irq INT_TICK();
    virtual int  Set_Time(time_t  &pCuTime);
    virtual int  Get_Time(time_t &pCuTime);
protected:

};

/********************************************************************************/
#endif