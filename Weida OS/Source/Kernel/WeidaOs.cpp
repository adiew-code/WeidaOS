#include"WeidaOS.h"


WeidaOS *WeidaOS::Cur_OS=0;
bool     WeidaOS::Run = false;



WeidaOS::WeidaOS(Board *p)
{
  try{
  		if(!(pBoard = p)) ///传过来的为空值...
			  throw -1;
     }   
	  
		 catch(int )
		 {
			return ; 
		 }
    Cur_OS = this ; //Cur_OS Can't be change any more!
    SetDevices();

    Task Task_Idle(-1u, OS_Idle); ////创建系统空闲任务

    /*********************创建用户任务***************************/
    CreateUserTask();
		 
}

WeidaOS::~WeidaOS()
{

}


void WeidaOS::WeidaOS_Run()
{
    U8 Line = Virtual_Task::MapTab[ Virtual_Task::OSReadyGrp ];   ///From 0  to fifth
    U8 Row  = Virtual_Task::MapTab[ Virtual_Task::OSReadyTab[Line] ]; ///From 0  to fifth

   Virtual_Task::Cur_Prior = (Line << 4) | Row ; ///最高优先级...
 
	  Run = true; ///系统开始运行
	
   INT_TICK_ENABLE( ); ///时间中断使能

    __Restore(Virtual_Task::TaskPriorTab[Virtual_Task::Cur_Prior]);  ///恢复运行最高优先级的任务...
}

void WeidaOS::SetDevices()
{
    pBoard->SetDevices();///安装不成功则...
    {
        return ;
    }
   // if(!pBoard->SetRTC())///安装成功则显示信息...
       ;/// Uart_Printf("RTC Init Success.....\n");

   // if(!pBoard->SetLCD())///安装成功则显示信息...
       ;// Uart_Printf("LCD Init Success.....\n");


    ///pBoard->Uart_PutStr("RTC time : %02x%02x-%02x-%02x Week:%02x %02x:%02x:%02x\n",CurTime._year, CurTime.year_, CurTime.month, CurTime.date,CurTime.week, CurTime.hour, CurTime.minute, CurTime.second );

    ///Uart_Printf("\nNow Loading System , Please Wait.....\n");

}


void  WeidaOS::Circle(int x1, int y1, int r, int color)
{
    if(pBoard == 0)
        return ;
    if(pBoard->pLCD == 0)
        return ;

    pBoard->pLCD->Circle( x1,  y1, r, color);
}

void WeidaOS::Show_Str(int x, int y, unsigned char *HZ, int color, int bkcolor)
{
    if(pBoard == 0)
        return ;
    if(pBoard->pLCD == 0)
        return ;

    pBoard->pLCD->Show_Str( x, y, HZ,  color, bkcolor);
}

void WeidaOS::Circlefill(int x1, int y1, int r, int color)
{
    if(pBoard == 0)
        return ;
    if(pBoard->pLCD == 0)
        return ;
    pBoard->pLCD->Circlefill( x1,  y1,  r,  color);
}

void WeidaOS::Clear(int color)
{
    if(pBoard == 0)
        return ;
    if(pBoard->pLCD == 0)
        return ;
    pBoard->pLCD->Clear( color);
}

void WeidaOS::DrawLine(int x1, int y1, int x2, int y2, int color)
{
    if(pBoard == 0)
        return ;
    if(pBoard->pLCD == 0)
        return ;
    pBoard->pLCD->DrawLine( x1, y1, x2, y2, color);
}

void WeidaOS::Rect(int x0, int y0, int x1, int y1, int color)
{
    if(pBoard == 0)
        return ;
    if(pBoard->pLCD == 0)
        return ;
    pBoard->pLCD->Rect( x0,  y0,  x1, y1, color);
}

void WeidaOS::FilledRectangle(int x1, int y1, int x2, int y2, int color)
{
    if(pBoard == 0)
        return ;
    if(pBoard->pLCD == 0)
        return ;
    pBoard->pLCD->FilledRectangle( x1, y1, x2, y2,  color);
}

void WeidaOS::Paint_Bmp(int x0, int y0, int h, int l, const unsigned char *bmp)
{
    if(pBoard == 0)
        return ;
    if(pBoard->pLCD == 0)
        return ;
    pBoard->pLCD->Paint_Bmp( x0, y0, h, l, bmp);
}

void WeidaOS::LCD_Test()
{
    if(pBoard == 0)
        return ;
    if(pBoard->pLCD == 0)
        return ;
    pBoard->pLCD->LCD_Test();
}

/********************************************************************************
    串口类调用
******************************************************************************/
void WeidaOS::Uart_Printf(char *fmt, ...)
{
    char  string[512];
    Virtual_Uart *Uart;

    if(pBoard == 0)
        return ;
    if( (Uart = pBoard->pUart) == 0)
        return ;

    va_list ap;

    va_start(ap, fmt);

    vsprintf(string, fmt, ap); //库函数

    va_end(ap);
 
    pBoard->pUart->Uart_PutStr((char*)string);
}



int WeidaOS::OS_Idle()
{
    for(;;)
    {
      
    }
    return 0;
}
