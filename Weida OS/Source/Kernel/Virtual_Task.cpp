#include"Virtual_Task.h"
#include"MapTab_16.h"
#include"Address.h"
#include"WeidaOS.h" //为什么不能包含？？一般含就运行就有错？

/*************静态变量初始化**********************************/

U16 Virtual_Task::OSReadyGrp = 0;             ///就绪任务组初始为0
U16 Virtual_Task::OSReadyTab[16] = {0};       ///就绪表
U8  Virtual_Task::Cur_Prior = 0;              ///当前任务优先级
Virtual_Task  *Virtual_Task::Task_Head = 0;   ///任务链表头
Virtual_Task  *Virtual_Task::TaskPriorTab[256] = {0};

/*************静态变量初始化**********************************/


Virtual_Task::Virtual_Task(U8 Prio, Do_Task Main, U32 Stack_Size): Prior(Prio),StackSize(Stack_Size)
{
	
    Get_Critical(); ////获得临界区
	
	
   if(WeidaOS::Run == false)
   {		 
	   Stack = (U32*)(WeidaOS::Cur_OS)->pBoard->pMem->malloc(Stack_Size<<2,Prio);//申请内存作为任务的堆栈;
	 }else
	 {
		 Stack =  (U32*)new int[Stack_Size<<2];	///动态申请
	 }
	 
	 try
	 {
		 if(Stack == 0 )
			 return;///throw "Error !";
		
	 }
	 catch(int)
	 {
		 
		 
	 }
	
	
    Enter_Critical();  ///以下进入临界区

    if(TaskPriorTab[Prio] == 0 ) ///如果该优先级未被使用
    {
        TaskPriorTab[Prio] = this;	///存储任务指针
        _Add_Task(this);   ///将本任务按顺序加入任务链表

        Cur_Stack = (U8 *)&Stack[StackSize-1]; ///设置堆栈指针
        Stack[0] = (U32)Main; ///Stack[0] 暂存Main函数地址

        DelayClock = 0; ///延时时间片，不为0则表示任务处于延时等待中
        CriResType = NON_RESOURCE; //任务等待资源，不为0则表示任务处于阻塞等待所等待的 资源类型

        Set_Bit(OSReadyGrp, Prio >> 4); ///将就绪组中对于为置1
        Set_Bit(OSReadyTab[Prio>>4], Prio & 0xf);//设置就绪标志

    }
    Exit_Critical();  ///退出临界区
}


///将Cur按从小到大 插入双向任务链表中
void Virtual_Task::_Add_Task(Virtual_Task *Cur)
{
    Virtual_Task *p;
    U8 Pri;

    Get_Critical(); ////获得临界区

    if(Cur == 0)///检查参数
    {
        return ;
    }

    Pri = Cur->Prior; ///当前任务指针


    Enter_Critical();  ///以下进入临界区

    if(Task_Head == 0)  ///如果Task_Head==0,这是第一个任务
    {
        Task_Head = Cur;   //将本任务放在第一个

        Cur->Next = 0;
        Cur->Prev = 0;
    }
    else      ///按照从小到大的顺序插入到链表
    {
        for(p = Task_Head;  ; p = p->Next) ///遍历任务链表
        {
            if(p->Prior > Pri)///循环已经找到了插入位置，即插到p前面即可
            {
                if(p->Prev == 0)//p是第一个
                {
                    Task_Head = Cur; ///Cur is Head now....
                    p->Prev = Cur;

                    Cur->Next = p;
                    Cur->Prev = 0;

                }
                else///p不是第一个
                {
                    p->Prev->Next = Cur;
                    Cur->Prev = p->Prev;

                    Cur->Next = p;
                    p->Prev = Cur;
                }

                break;
            }


            if(p->Next == 0)///P是最后一个，则将Cur插入链表尾部
            {
                p->Next = Cur;

                Cur->Prev = p;
                Cur->Next = 0;

                break;
            }
        }

    }

    Exit_Critical();  ///退出临界区
}

void Virtual_Task::_OS_Exec(U8 Prior)  ///每个任务第一次要执行的主函数....
{
    Virtual_Task *p = TaskPriorTab[Prior];
    S32 Ret;
    ///调用任务函数...
    if(p->Stack[0])  ///确认Stack[0]不为空
        Ret=(* (Do_Task)(p->Stack[0])  )  ( );  ///调用任务....
		
		
    ////任务结束了，删除任务
    p->DelTask();  ///删除本任务...
}


void Virtual_Task::_Delay(U32 Ticks)  ///延时函数
{
    Get_Critical(); ////获得临界区

    if(Ticks)
    {
        Enter_Critical();  ///以下进入临界区

        Virtual_Task *p = TaskPriorTab[Cur_Prior];

        if(p->DelayClock) ///若第0位 == 1则直接返回
        {
            Exit_Critical();  ///退出临界区

            return ; //返回
        }

        Clr_Bit(OSReadyTab[Cur_Prior >>4] , Cur_Prior & 0xf);///在就绪表中取消就绪标志
        if( OSReadyTab[Cur_Prior >>4] == 0)///取消就绪组标志
            Clr_Bit(OSReadyGrp, Cur_Prior >> 4 );
				
        p->DelayClock = Ticks;  ///延长时钟节拍数
				
        Exit_Critical();  ///退出临界区

        _Sched(); ////重新调度运行
    }

}


U32 Virtual_Task::_Wait(U8 Pri)  ///等待pri任务结束
{
	Get_Critical(); ////获得临界区

  	Enter_Critical();  ///以下进入临界区
	
  	if(0 == TaskPriorTab[Pri])
		{
       Exit_Critical();
		 
			return 1;
	  }
	
    Clr_Bit(OSReadyTab[Cur_Prior >>4] , Cur_Prior & 0xf);///在就绪表中取消就绪标志
    if( OSReadyTab[Cur_Prior >>4] == 0)///取消就绪组标志
            Clr_Bit(OSReadyGrp, Cur_Prior >> 4 );
				

    TaskPriorTab[Cur_Prior]->CriResType = ((Pri<<24) | WAIT_SIGNL); ///当前任务设置成阻塞状态了...
				
     Exit_Critical();  ///退出临界区

        _Sched(); ////重新调度运行
    
return 0;
}



Virtual_Task::~Virtual_Task()
{
    DelTask();
}



void Virtual_Task::DelTask()
{
	U8 Pr(Prior);
	
	Get_Critical();
	
	///删除一些用户没有删除的内存资源
	(WeidaOS::Cur_OS)->pBoard->pMem->free( ); ///将Prior任务所有未删除的内存删除
	
	
   Enter_Critical();
	
   TaskPriorTab[Pr] = 0; //清空
	 
	
	//从任务链表将自己删除...
      if(this == Task_Head) //如果是第一个，下一个变第一个
				{
						
					Task_Head = Next;
					Next->Prev=0;

					}else///是中间某个，反正不可能是最后一个，最后一个是空闲任务
					{
 
						Prev->Next = Next;
						Next->Prev = Prev;

					}
					
		   for(Virtual_Task * p = Task_Head; p ; p = p->Next) ///遍历任务链表
        {
           if(p->CriResType == ((Prior<<24) | WAIT_SIGNL))///找到了就唤醒它
					{
						///将该任务加入就绪列表中   
						U8 Pri = p->Prior;
						Set_Bit(p->OSReadyGrp, Pri >> 4); ///将就绪组中对于为置1
            Set_Bit(p->OSReadyTab[Pri>>4], Pri & 0xf);
						
						 
						////并取消任务的阻塞等待状态...
						  p->CriResType = NON_RESOURCE; ///当前任务已经不是阻塞状态了...
				  }
        }
					
							 
				 Clr_Bit(OSReadyTab[Pr >>4] , Pr & 0xf);///在就绪表中取消就绪标志
        if( OSReadyTab[Pr >>4] == 0)///取消就绪组标志
            Clr_Bit(OSReadyGrp, Pr >> 4 );
				
				  delete Stack; 
				
					_Sched(); ////重新调度运行
}


////重新调度
void Virtual_Task::_Sched()
{
    Get_Critical();

    Enter_Critical();
	
    U8 Line( MapTab[OSReadyGrp ] );
    U8 Prior( (Line << 4) | ( MapTab[OSReadyTab[Line] ] ) ); ///得到最高优先级

    Line = Cur_Prior; ///Line先预存上一次任务优先级，因为下面会被覆盖掉

    if(Line != Prior) ///需要重新调度
    {
        Cur_Prior = Prior;  ///保存现在最新得到高优先级，覆盖了！
        TASK_SW(TaskPriorTab[Prior] , TaskPriorTab[Line]); ///保存上一次的任务的现场，并且运行高优先级任务
    }

    Exit_Critical();
}

void  __irq Virtual_Task::_INT_TICK( )
{
    U8  New_Prior, Old_Prior;
    Virtual_Task *p;

    for(p = Task_Head; p ; p = p->Next) ///遍历任务链表
    {

         if(p->DelayClock)     /////如果本任务处于延时等待状态则..
            if( (--(p->DelayClock)) == 0 )///时间到则将该任务从等待链表中删除，并设为就绪状态
            {
                New_Prior = p->Prior;

                ///设置成就绪
                Set_Bit(OSReadyGrp, New_Prior >> 4); ///将就绪组中对于为置1
                Set_Bit(OSReadyTab[New_Prior>>4], New_Prior & 0xf);
            }
    }

    New_Prior = MapTab[OSReadyGrp ];
    New_Prior = (New_Prior << 4) | MapTab[ OSReadyTab[New_Prior] ] ;

    if(Cur_Prior != New_Prior)///有新的优先级更高的任务，要重新调度
    {
        Old_Prior  = Cur_Prior;
        Cur_Prior = New_Prior;

        INT_TASK_SW(TaskPriorTab[New_Prior] , TaskPriorTab[Old_Prior]); ///保存上一次的任务的现场，并且运行高优先级任务

    }

    Set_Bit(rSRCPND , 8); //写1清除中断
    Set_Bit(rINTPND , 8); //写1清除中断
}
