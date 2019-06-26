#ifndef  __WEIDA_CRITICAL_RES_H
#define  __WEIDA_CRITICAL_RES_H
/***********************************************************************************
			临界资源处理....
*************************************************************************************/
#include"DataTypes.h"
#include"Config.h"
#include"Resource.h"

class Critical_Res
{
    U32  Res_Type; ///资源类型(0x1-0xffffffff)不能为0
    U32 Res_Used; //资源是否被占用，没有则值应该大于255,否则值从0-255
public:
    Critical_Res(U32 Type , U32 Used = 0):Res_Type(Type), Res_Used(Used)
    {
      if(Type ==0 )///出错，不能为0
			{
				Type = -1u;
			}
    }
    ~Critical_Res()
    {

    }
    
protected:
	  void  Enter_CriSec();		
    void  Exit_CriSec() ;///退出临界区代码	
		
private:
    S32 Enter_CriSec_Test(bool &First) ;///进入临界区代码
};

/********************************************************************************/
#endif