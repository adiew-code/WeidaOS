#ifndef  __BOARD_H
#define  __BOARD_H
#include"DataTypes.h"
#include"Virtual_Uart.h"
#include"Virtual_LCD.h"
#include"Virtual_RTC.h"
#include"Virtual_Memory.h"
#include"Virtual_TouchScreen.h"




/***********************************************************************************
			虚拟硬件平台...
*************************************************************************************/
class WeidaOS;
class Virtual_Task;

class Board
{
	friend class WeidaOS;
	friend void* operator new(U32 size) ;
	friend void operator delete(void* p) ;
	friend class Virtual_Task;
	friend void Query();
    ///Data
protected:
    Virtual_Uart  *pUart;	//硬件串口
    Virtual_LCD   *pLCD;  //硬件LCD显示器
    Virtual_RTC   *pRTC; ///实时时钟
    Virtual_TouchScreen *pTouSc;	//触摸屏...
    Virtual_Memory   *pMem;  ///内存，用于系统动态申请

    

public:
    Board();
    ~Board();

    virtual void SetDevices( ) =0;
    

    int Get_Time(time_t &Cur);

    /* void Lcd_PowerEnable(int invpwren,int pwren);
    void Circle(int x1, int y1, int r, int color);
    void Show_Str(int x, int y, unsigned char *HZ, int color, int bkcolor);
    void Circlefill(int x1, int y1, int r, int color);
    void Clear(int color);
    void DrawLine(int x1, int y1, int x2, int y2, int color);
    void Rect(int x0, int y0, int x1, int y1, int color);
    void FilledRectangle(int x1, int y1, int x2, int y2, int color);
    void Paint_Bmp(int x0, int y0, int h, int l, const unsigned char *bmp);
    void LCD_Test();*/


    /// void Uart_PutStr(char *fmt, ...);
protected:

};

/********************************************************************************/
#endif