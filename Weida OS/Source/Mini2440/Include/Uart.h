#ifndef  __UART_H
#define  __UART_H
/***********************************************************************************/
#include"Virtual_Uart.h"
#include"Address.h"

class UART: public Virtual_Uart ///�Ӵ�����̳�
{
    int CurUart; //��ǰ���ں�
public:
    UART(int n = 0);
    void Init(int n);///num��ֵ��0--2
    virtual     unsigned char Uart_Getc();
    virtual	void Uart_Putc(unsigned char c);
    virtual	void Uart_PutStr(char *string);
protected:

};



/********************************************************************************/
#endif