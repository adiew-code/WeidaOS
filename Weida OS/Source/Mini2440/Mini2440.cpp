#include"Mini2440.h"

extern const unsigned char Beautiful_Girl[];
extern "C" void MMU_Init();

Mini2440::Mini2440()
{
    MMU_Init();//初始化MMU
    ///Uart0.Uart_PutStr("\n您使用的是Mini2440开发板，硬件已经成功初始化....\n");///打印信息....
}

Mini2440::~Mini2440()
{
    ///Uart0.Uart_PutStr("\nMini2440正在退出....\n");///打印信息....
}


///安装各种硬件，返回硬件指针给上层
void Mini2440::SetDevices( )
{
   pUart   = &Uart0 ;	//硬件串口
   pLCD    = &W35;  //硬件LCD显示器
   pRTC    = &Clock; ///实时时钟
   pTouSc  = &TouSc;	//触摸屏...
   pMem    = &Mem;  ///内存，用于系统动态申请	  
	
}

