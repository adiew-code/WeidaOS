#ifndef  __WEIDA_CRITICAL_RES_H
#define  __WEIDA_CRITICAL_RES_H
/***********************************************************************************
			�ٽ���Դ����....
*************************************************************************************/
#include"DataTypes.h"
#include"Config.h"
#include"Resource.h"

class Critical_Res
{
    U32  Res_Type; ///��Դ����(0x1-0xffffffff)����Ϊ0
    U32 Res_Used; //��Դ�Ƿ�ռ�ã�û����ֵӦ�ô���255,����ֵ��0-255
public:
    Critical_Res(U32 Type , U32 Used = 0):Res_Type(Type), Res_Used(Used)
    {
      if(Type ==0 )///��������Ϊ0
			{
				Type = -1u;
			}
    }
    ~Critical_Res()
    {

    }
    
protected:
	  void  Enter_CriSec();		
    void  Exit_CriSec() ;///�˳��ٽ�������	
		
private:
    S32 Enter_CriSec_Test(bool &First) ;///�����ٽ�������
};

/********************************************************************************/
#endif