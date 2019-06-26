#include"Critical_Res.h"
#include"Virtual_Task.h"


void  Critical_Res::Enter_CriSec()
{
	bool First=true; ///第一次进入
  
	while(Enter_CriSec_Test(First)); ///若返回-1则一直循环...

}
		


void Critical_Res::Exit_CriSec( )
{
    Get_Critical();

    Enter_Critical();
    
	  U8 Pri(--Res_Used) ; ///低八位为减1
  
    Clr_Bit(Res_Used,31) ; ///资源标志为空闲,31bit=0
    
       
	  if(Pri) ///如果有阻塞任务正在等待该资源，找到下一个等待该资源的任务，并唤醒它！	
			 for(Virtual_Task*p = Virtual_Task::Task_Head; p ; p = p->Next) ///遍历任务链表
        {
          if(p->CriResType == Res_Type)///找到了就唤醒它
					{
						///将该任务加入就绪列表中  
						Pri = p->Prior;
						Set_Bit(p->OSReadyGrp, Pri >> 4); ///将就绪组中对于为置1
            Set_Bit(p->OSReadyTab[Pri>>4], Pri & 0xf);
						
						 
						////并取消任务的阻塞等待状态...
						  p->CriResType = NON_RESOURCE; ///当前任务已经不是阻塞状态了...
						
						Virtual_Task::_Sched( ) ; ///重新调度
						break; ///跳出循环
					}
        }
    Exit_Critical( );
}


S32 Critical_Res::Enter_CriSec_Test(bool &First) ///尝试进入临界区一次...失败返回-1
{
    Virtual_Task *p;
    U8 Pri;

    Get_Critical();

    Enter_Critical();
	
    if((Res_Used >> 31) == 0 ) ///未被占用
    {
			      Res_Used =  (1<<31)| (Virtual_Task::Cur_Prior<<8) | 1; ///资源标志为已被占用
    }
    else///资源已被占用了...
    {
        p = Virtual_Task::TaskPriorTab[Virtual_Task::Cur_Prior];//当前任务指针..
			  
			 if(First)///第一次进入
			 {
				 Res_Used ++; ///资源标志为已被占用，低八位加1，其他不变
		     First = false;  ///但下次该任务尝试进入临界时，不自加了！
				}
				
			Pri = p->Cur_Prior;

        switch(Res_Type)///根据资源类型，进行不同的处理
        {
					          ////特殊处理
					
        default:   ///一般处理

            ///从就绪列表中移除，并把自己设为阻塞等待...
            Clr_Bit(p->OSReadyTab[p->Cur_Prior >>4], p->Cur_Prior & 0xf);     ///在就绪表中取消就绪标志

            if(p->OSReadyTab[p->Cur_Prior >>4] == 0)///取消就绪组标志
                Clr_Bit(p->OSReadyGrp, p->Cur_Prior >> 4 );

            break;
        }

			  p->CriResType = Res_Type; ///当前任务设置成阻塞状态了...
        
				Virtual_Task::_Sched( ) ; ///重新调度

        Exit_Critical( );///配套使用...
        return   -1;   //返回-1，表示进入临界区失败
    }


    Exit_Critical( );
    return  0; ///成功返回0
}