#ifndef __TOUCH_SCREEN_H__
#define __TOUCH_SCREEN_H__
#include"Virtual_TouchScreen.h"
#include"Address.h"

class TouchScreen: public  Virtual_TouchScreen
{
    static Point Position[2]; //存储所按下点的坐标
    static bool State; ////判断AD转换的数据是按下时的--1，还是松开时的--0？

    static void __irq	TouchSc_handle();

    static void INT_TC();
    static void INT_ADC_S();
public:

    TouchScreen();
    ~TouchScreen();

protected:

};



#endif
