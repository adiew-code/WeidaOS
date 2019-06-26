#ifndef  __WEIDA_VIRTUAL_UART_H
#define  __WEIDA_VIRTUAL_UART_H
/***********************************************************************************
			���⴮��......
*************************************************************************************/
#include"DataTypes.h"
#include"Critical_Res.h"


class Virtual_Uart: public Critical_Res
{

    static int num;	///����Ĵ�������....
public:
    Virtual_Uart();
    ~Virtual_Uart();

    virtual  unsigned char Uart_Getc() = 0;
    virtual	void Uart_Putc(unsigned char c) = 0;
    virtual  void Uart_PutStr(char *string) = 0;
protected:

};

/********************************************************************************/
#endif