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
			�ļ�����WeidaOS.h
			˵����
					����ϵͳ���ͷ�ļ������ඨ��һ����Ӳ���޹صĲ���ϵͳ�����
					������һƽ̨ʹ�ø�ϵͳ����ô�͵����ж���һ������Board���
					���࣬��������ʵ��Board����Ĵ��麯�����ٽ������ַ�ڶ���
					����ϵͳ����ʱ��Ϊ�������ݸ�ϵͳ���󼴿ɡ�
			ע�⣺
					���ڸò���ϵͳ��������֪���Լ���Ҫ���Ǹ������ƽ̨���У�Ҳ��
					�ǲ�֪���Լ���Ҫ���е�ʵ��Ӳ����������ֻ����Board��������ƽ
					̨�����С�Board��ͳ��˸�ϵͳ���е���СӲ��������....
		    ���ߣ�Weida
			���ڣ�2012/3/22/10:02am
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
    time_t CurTime;///ϵͳ��ǰʱ��.
    
public:

    static WeidaOS *Cur_OS;
    static bool     Run;

    static U32 ErrNum; ///��һ���Ĳ����Ƿ���ȷ����������μ�Error.h

    WeidaOS(Board *pBoard = 0); ///����ϵͳʱ�Զ����õĺ���
    ~WeidaOS();			  ///�˳�ϵͳʱ�Զ����õĺ���

    static int OS_Idle( ); ///ϵͳ��������...

    ///LCD������
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

    ///���������
public:
    void Uart_Printf(char *fmt, ...);

private:
	  void   CreateUserTask();
    void SetDevices(); ///��ѯpBoard������豸������װ��
    static  void  WeidaOS_Run();

};







/********************************************************************************/
#endif