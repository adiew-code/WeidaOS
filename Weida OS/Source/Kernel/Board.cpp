#include"Board.h"
#include"stdio.h"
#include"stdarg.h"

Board::Board()
{
    pUart  = 0; //��ʼʱû�д���...
    pLCD   = 0;
    pRTC   = 0;
    pTouSc = 0;
	  pMem   = 0; 
}

Board::~Board()
{


}
/*
int Board::SetDevices()
{
	Return();  ///���ظ���Ӳ��
	
	return 0;
}

*/

int Board::Get_Time(time_t &Cur)
{
    if(!pRTC)	///û��ʱ��
        return -1;

    pRTC->Get_Time(Cur);

    return 0;
}