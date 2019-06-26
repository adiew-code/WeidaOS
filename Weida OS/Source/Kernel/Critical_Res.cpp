#include"Critical_Res.h"
#include"Virtual_Task.h"


void  Critical_Res::Enter_CriSec()
{
	bool First=true; ///��һ�ν���
  
	while(Enter_CriSec_Test(First)); ///������-1��һֱѭ��...

}
		


void Critical_Res::Exit_CriSec( )
{
    Get_Critical();

    Enter_Critical();
    
	  U8 Pri(--Res_Used) ; ///�Ͱ�λΪ��1
  
    Clr_Bit(Res_Used,31) ; ///��Դ��־Ϊ����,31bit=0
    
       
	  if(Pri) ///����������������ڵȴ�����Դ���ҵ���һ���ȴ�����Դ�����񣬲���������	
			 for(Virtual_Task*p = Virtual_Task::Task_Head; p ; p = p->Next) ///������������
        {
          if(p->CriResType == Res_Type)///�ҵ��˾ͻ�����
					{
						///���������������б���  
						Pri = p->Prior;
						Set_Bit(p->OSReadyGrp, Pri >> 4); ///���������ж���Ϊ��1
            Set_Bit(p->OSReadyTab[Pri>>4], Pri & 0xf);
						
						 
						////��ȡ������������ȴ�״̬...
						  p->CriResType = NON_RESOURCE; ///��ǰ�����Ѿ���������״̬��...
						
						Virtual_Task::_Sched( ) ; ///���µ���
						break; ///����ѭ��
					}
        }
    Exit_Critical( );
}


S32 Critical_Res::Enter_CriSec_Test(bool &First) ///���Խ����ٽ���һ��...ʧ�ܷ���-1
{
    Virtual_Task *p;
    U8 Pri;

    Get_Critical();

    Enter_Critical();
	
    if((Res_Used >> 31) == 0 ) ///δ��ռ��
    {
			      Res_Used =  (1<<31)| (Virtual_Task::Cur_Prior<<8) | 1; ///��Դ��־Ϊ�ѱ�ռ��
    }
    else///��Դ�ѱ�ռ����...
    {
        p = Virtual_Task::TaskPriorTab[Virtual_Task::Cur_Prior];//��ǰ����ָ��..
			  
			 if(First)///��һ�ν���
			 {
				 Res_Used ++; ///��Դ��־Ϊ�ѱ�ռ�ã��Ͱ�λ��1����������
		     First = false;  ///���´θ������Խ����ٽ�ʱ�����Լ��ˣ�
				}
				
			Pri = p->Cur_Prior;

        switch(Res_Type)///������Դ���ͣ����в�ͬ�Ĵ���
        {
					          ////���⴦��
					
        default:   ///һ�㴦��

            ///�Ӿ����б����Ƴ��������Լ���Ϊ�����ȴ�...
            Clr_Bit(p->OSReadyTab[p->Cur_Prior >>4], p->Cur_Prior & 0xf);     ///�ھ�������ȡ��������־

            if(p->OSReadyTab[p->Cur_Prior >>4] == 0)///ȡ���������־
                Clr_Bit(p->OSReadyGrp, p->Cur_Prior >> 4 );

            break;
        }

			  p->CriResType = Res_Type; ///��ǰ�������ó�����״̬��...
        
				Virtual_Task::_Sched( ) ; ///���µ���

        Exit_Critical( );///����ʹ��...
        return   -1;   //����-1����ʾ�����ٽ���ʧ��
    }


    Exit_Critical( );
    return  0; ///�ɹ�����0
}