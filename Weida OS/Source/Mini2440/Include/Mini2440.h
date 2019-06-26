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
    UART   Uart0;//在开发板上安装一个0号串口，此时不能初始化
    Memory      Mem;
	  LCD    W35;
    RTC    Clock;
    TouchScreen TouSc;
	 
public:
    Mini2440();
    ~Mini2440();

protected:    
   virtual void SetDevices( );  ///实现上层的虚函数，安装各种硬件



};

/********************************************************************************/
#endif