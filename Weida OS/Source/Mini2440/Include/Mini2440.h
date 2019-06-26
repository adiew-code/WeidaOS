#ifndef  __MINI2440_H
#define  __MINI2440_H
/***********************************************************************************/
#include"Board.h"
#include"Uart.h"
#include"Device.h"
#include"LCD.h"
#include"RTC.h"
#include"TouchScreen.h"
#include"Memory.h"

class Mini2440: public Board
{
    UART   Uart0;//�ڿ������ϰ�װһ��0�Ŵ��ڣ���ʱ���ܳ�ʼ��
    Memory      Mem;
	  LCD    W35;
    RTC    Clock;
    TouchScreen TouSc;
	 
public:
    Mini2440();
    ~Mini2440();

protected:    
   virtual void SetDevices( );  ///ʵ���ϲ���麯������װ����Ӳ��



};

/********************************************************************************/
#endif