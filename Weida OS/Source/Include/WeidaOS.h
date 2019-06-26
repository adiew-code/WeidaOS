#ifndef  __WEIDA_OS_H
#define  __WEIDA_OS_H
/***********************************************************************************/
#include"DataTypes.h"
#include"Board.h"
#include"stdio.h"
#include"stdarg.h"
#include"Task.h"
#include"LCD.h"
#include"Scatter_File.h"

/**********************************************************************************
			文件名：WeidaOS.h
			说明：
					操作系统类的头文件，该类定了一个与硬件无关的操作系统，如果
					想在任一平台使用该系统，那么就得自行定义一个基于Board类得
					子类，该类必须得实现Board里面的纯虚函数。再将该类地址在定义
					操作系统对象时作为参数传递给系统对象即可。
			注意：
					对于该操作系统，它并不知道自己将要在那个具体的平台运行，也就
					是不知道自己将要运行的实际硬件环境，它只根据Board这个虚拟的平
					台而运行。Board类就成了该系统运行的最小硬件依赖了....
		    作者：Weida
			日期：2012/3/22/10:02am
***********************************************************************************/

__swi(0x01) void LED1_ON();
__swi(0x02) void LED2_ON();
__swi(0x03) void LED3_ON();
__swi(0x04) void LED4_ON();

__swi(0x05) void LED1_OFF();
__swi(0x06) void LED2_OFF();
__swi(0x07) void LED3_OFF();
__swi(0x08) void LED4_OFF();
class Virtual_Task;

class WeidaOS
{
	friend void* operator new(U32 size) ;
	friend void operator delete(void* p) ;
	friend void Query();
	friend class Virtual_Task;
	
	
private:
    Board *pBoard;
    time_t CurTime;///系统当前时间.
    
public:

    static WeidaOS *Cur_OS;
    static bool     Run;

    static U32 ErrNum; ///上一步的操作是否正确，错误码请参见Error.h

    WeidaOS(Board *pBoard = 0); ///进入系统时自动调用的函数
    ~WeidaOS();			  ///退出系统时自动调用的函数

    static int OS_Idle( ); ///系统空闲任务...

    ///LCD操作类
public:
    // void Lcd_PowerEnable(int invpwren,int pwren);
    void Circle(int x1, int y1, int r, int color);
    void Show_Str(int x, int y, unsigned char *HZ, int color, int bkcolor);
    void Circlefill(int x1, int y1, int r, int color);
    void Clear(int color);
    void DrawLine(int x1, int y1, int x2, int y2, int color);
    void Rect(int x0, int y0, int x1, int y1, int color);
    void FilledRectangle(int x1, int y1, int x2, int y2, int color);
    void Paint_Bmp(int x0, int y0, int h, int l, const unsigned char *bmp);
    void LCD_Test();

    ///串口类操作
public:
    void Uart_Printf(char *fmt, ...);

private:
	  void   CreateUserTask();
    void SetDevices(); ///查询pBoard里面的设备，并安装！
    static  void  WeidaOS_Run();

};







/********************************************************************************/
#endif