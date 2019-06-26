#include"Uart.h"
#include"stdio.h"
#include"stdarg.h"


UART::UART(int n)
{
    Init(n);
}

void UART::Init(int n) ///ʵ�ִ�����Ĵ��麯��...
{
    switch(n)
    {
    case 0:
        CurUart = n;


        rGPHCON = 0xA0; ////GPHCON
        rGPHUP = 0x0 ; ////GPHUP
        rULCON0 = 0x03 ; ////ULCON0��������֡��ʽ
        rUCON0 = 0x05 ;////UCON0
        rUFCON0 = 0x0 ;///��ʹ��FIFO
        rUMCON0 = 0x0 ; ////��ʹ������
        rUBRDIV0 = 26; ///PCLK=50M ������115200


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
        while( !( (rUTRSTAT0) & 1 )  ); ///��ѯrUTRSTAT0״̬

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
        while( !( (rUTRSTAT0) & (1 << 2) )  ); ///��ѯrUTRSTAT0״̬

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
