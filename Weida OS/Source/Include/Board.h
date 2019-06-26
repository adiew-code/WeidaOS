#ifndef  __BOARD_H
#define  __BOARD_H
#include"DataTypes.h"
#include"Virtual_Uart.h"
#include"Virtual_LCD.h"
#include"Virtual_RTC.h"
#include"Virtual_Memory.h"
#include"Virtual_TouchScreen.h"




/***********************************************************************************
			����Ӳ��ƽ̨...
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
    Virtual_Uart  *pUart;	//Ӳ������
    Virtual_LCD   *pLCD;  //Ӳ��LCD��ʾ��
    Virtual_RTC   *pRTC; ///ʵʱʱ��
    Virtual_TouchScreen *pTouSc;	//������...
    Virtual_Memory   *pMem;  ///�ڴ棬����ϵͳ��̬����

    

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