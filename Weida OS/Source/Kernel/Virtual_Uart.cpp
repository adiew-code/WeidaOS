#include"Virtual_Uart.h"

int Virtual_Uart::num;
Virtual_Uart::Virtual_Uart(): Critical_Res(UART_RESOURCE)
{
    num++;
}

Virtual_Uart::~Virtual_Uart()
{
    num--;

}