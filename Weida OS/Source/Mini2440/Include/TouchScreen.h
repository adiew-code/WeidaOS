#ifndef __TOUCH_SCREEN_H__
#define __TOUCH_SCREEN_H__
#include"Virtual_TouchScreen.h"
#include"Address.h"

class TouchScreen: public  Virtual_TouchScreen
{
    static Point Position[2]; //�洢�����µ������
    static bool State; ////�ж�ADת���������ǰ���ʱ��--1�������ɿ�ʱ��--0��

    static void __irq	TouchSc_handle();

    static void INT_TC();
    static void INT_ADC_S();
public:

    TouchScreen();
    ~TouchScreen();

protected:

};



#endif
