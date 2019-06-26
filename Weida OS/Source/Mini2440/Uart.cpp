#include"Uart.h"
#include"stdio.h"
#include"stdarg.h"


UART::UART(int n)
{
    Init(n);
}

void UART::Init(int n) ///实现纯虚类的纯虚函数...
{
    switch(n)
    {
    case 0:
        CurUart = n;


        rGPHCON = 0xA0; ////GPHCON
        rGPHUP = 0x0 ; ////GPHUP
        rULCON0 = 0x03 ; ////ULCON0设置数据帧格式
        rUCON0 = 0x05 ;////UCON0
        rUFCON0 = 0x0 ;///不使用FIFO
        rUMCON0 = 0x0 ; ////不使用流控
        rUBRDIV0 = 26; ///PCLK=50M 波特率115200


        break;
    case 1:
        CurUart = n;
        break;
    case 2:
        CurUart = n;

        break;
    default:
        return;
    }

}

unsigned char UART::Uart_Getc()
{
    switch(CurUart)
    {
    case 0:
        while( !( (rUTRSTAT0) & 1 )  ); ///查询rUTRSTAT0状态

        return rURXH0;
        break;
    case 1:

        break;

    case 2:

        break;
    }
}


void UART::Uart_Putc(unsigned char c)
{
    switch(CurUart)
    {
    case 0:
        while( !( (rUTRSTAT0) & (1 << 2) )  ); ///查询rUTRSTAT0状态

        rUTXH0 = c;

        break;
    case 1:

        break;

    case 2:

        break;
    }

}


void  UART::Uart_PutStr(char *str)
{
	Enter_CriSec();

    for(; *str != '\0'; str++)
    {
        if(*str == '\n')
        {
            Uart_Putc('\r');
            Uart_Putc('\n');
        }
        else
            Uart_Putc(*str);
    }
  Exit_CriSec();		
}
