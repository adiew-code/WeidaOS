#include"Board.h"
#include"stdio.h"
#include"stdarg.h"

Board::Board()
{
    pUart  = 0; //初始时没有串口...
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
	Return();  ///返回各种硬件
	
	return 0;
}

*/

int Board::Get_Time(time_t &Cur)
{
    if(!pRTC)	///没有时钟
        return -1;

    pRTC->Get_Time(Cur);

    return 0;
}