#include"Virtual_Task.h"
#include"MapTab_16.h"
#include"Address.h"
#include"WeidaOS.h" //Ϊʲô���ܰ�������һ�㺬�����о��д�

/*************��̬������ʼ��**********************************/

U16 Virtual_Task::OSReadyGrp = 0;             ///�����������ʼΪ0
U16 Virtual_Task::OSReadyTab[16] = {0};       ///������
U8  Virtual_Task::Cur_Prior = 0;              ///��ǰ�������ȼ�
Virtual_Task  *Virtual_Task::Task_Head = 0;   ///��������ͷ
Virtual_Task  *Virtual_Task::TaskPriorTab[256] = {0};

/*************��̬������ʼ��**********************************/


Virtual_Task::Virtual_Task(U8 Prio, Do_Task Main, U32 Stack_Size): Prior(Prio),StackSize(Stack_Size)
{
	
    Get_Critical(); ////����ٽ���
	
	
   if(WeidaOS::Run == false)
   {		 
	   Stack = (U32*)(WeidaOS::Cur_OS)->pBoard->pMem->malloc(Stack_Size<<2,Prio);//�����ڴ���Ϊ����Ķ�ջ;
	 }else
	 {
		 Stack =  (U32*)new int[Stack_Size<<2];	///��̬����
	 }
	 
	 try
	 {
		 if(Stack == 0 )
			 return;///throw "Error !";
		
	 }
	 catch(int)
	 {
		 
		 
	 }
	
	
    Enter_Critical();  ///���½����ٽ���

    if(TaskPriorTab[Prio] == 0 ) ///��������ȼ�δ��ʹ��
    {
        TaskPriorTab[Prio] = this;	///�洢����ָ��
        _Add_Task(this);   ///��������˳�������������

        Cur_Stack = (U8 *)&Stack[StackSize-1]; ///���ö�ջָ��
        Stack[0] = (U32)Main; ///Stack[0] �ݴ�Main������ַ

        DelayClock = 0; ///��ʱʱ��Ƭ����Ϊ0���ʾ��������ʱ�ȴ���
        CriResType = NON_RESOURCE; //����ȴ���Դ����Ϊ0���ʾ�����������ȴ����ȴ��� ��Դ����

        Set_Bit(OSReadyGrp, Prio >> 4); ///���������ж���Ϊ��1
        Set_Bit(OSReadyTab[Prio>>4], Prio & 0xf);//���þ�����־

    }
    Exit_Critical();  ///�˳��ٽ���
}


///��Cur����С���� ����˫������������
void Virtual_Task::_Add_Task(Virtual_Task *Cur)
{
    Virtual_Task *p;
    U8 Pri;

    Get_Critical(); ////����ٽ���

    if(Cur == 0)///������
    {
        return ;
    }

    Pri = Cur->Prior; ///��ǰ����ָ��


    Enter_Critical();  ///���½����ٽ���

    if(Task_Head == 0)  ///���Task_Head==0,���ǵ�һ������
    {
        Task_Head = Cur;   //����������ڵ�һ��

        Cur->Next = 0;
        Cur->Prev = 0;
    }
    else      ///���մ�С�����˳����뵽����
    {
        for(p = Task_Head;  ; p = p->Next) ///������������
        {
            if(p->Prior > Pri)///ѭ���Ѿ��ҵ��˲���λ�ã����嵽pǰ�漴��
            {
                if(p->Prev == 0)//p�ǵ�һ��
                {
                    Task_Head = Cur; ///Cur is Head now....
                    p->Prev = Cur;

                    Cur->Next = p;
                    Cur->Prev = 0;

                }
                else///p���ǵ�һ��
                {
                    p->Prev->Next = Cur;
                    Cur->Prev = p->Prev;

                    Cur->Next = p;
                    p->Prev = Cur;
                }

                break;
            }


            if(p->Next == 0)///P�����һ������Cur��������β��
            {
                p->Next = Cur;

                Cur->Prev = p;
                Cur->Next = 0;

                break;
            }
        }

    }

    Exit_Critical();  ///�˳��ٽ���
}

void Virtual_Task::_OS_Exec(U8 Prior)  ///ÿ�������һ��Ҫִ�е�������....
{
    Virtual_Task *p = TaskPriorTab[Prior];
    S32 Ret;
    ///����������...
    if(p->Stack[0])  ///ȷ��Stack[0]��Ϊ��
        Ret=(* (Do_Task)(p->Stack[0])  )  ( );  ///��������....
		
		
    ////��������ˣ�ɾ������
    p->DelTask();  ///ɾ��������...
}


void Virtual_Task::_Delay(U32 Ticks)  ///��ʱ����
{
    Get_Critical(); ////����ٽ���

    if(Ticks)
    {
        Enter_Critical();  ///���½����ٽ���

        Virtual_Task *p = TaskPriorTab[Cur_Prior];

        if(p->DelayClock) ///����0λ == 1��ֱ�ӷ���
        {
            Exit_Critical();  ///�˳��ٽ���

            return ; //����
        }

        Clr_Bit(OSReadyTab[Cur_Prior >>4] , Cur_Prior & 0xf);///�ھ�������ȡ��������־
        if( OSReadyTab[Cur_Prior >>4] == 0)///ȡ���������־
            Clr_Bit(OSReadyGrp, Cur_Prior >> 4 );
				
        p->DelayClock = Ticks;  ///�ӳ�ʱ�ӽ�����
				
        Exit_Critical();  ///�˳��ٽ���

        _Sched(); ////���µ�������
    }

}


U32 Virtual_Task::_Wait(U8 Pri)  ///�ȴ�pri�������
{
	Get_Critical(); ////����ٽ���

  	Enter_Critical();  ///���½����ٽ���
	
  	if(0 == TaskPriorTab[Pri])
		{
       Exit_Critical();
		 
			return 1;
	  }
	
    Clr_Bit(OSReadyTab[Cur_Prior >>4] , Cur_Prior & 0xf);///�ھ�������ȡ��������־
    if( OSReadyTab[Cur_Prior >>4] == 0)///ȡ���������־
            Clr_Bit(OSReadyGrp, Cur_Prior >> 4 );
				

    TaskPriorTab[Cur_Prior]->CriResType = ((Pri<<24) | WAIT_SIGNL); ///��ǰ�������ó�����״̬��...
				
     Exit_Critical();  ///�˳��ٽ���

        _Sched(); ////���µ�������
    
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
	
	///ɾ��һЩ�û�û��ɾ�����ڴ���Դ
	(WeidaOS::Cur_OS)->pBoard->pMem->free( ); ///��Prior��������δɾ�����ڴ�ɾ��
	
	
   Enter_Critical();
	
   TaskPriorTab[Pr] = 0; //���
	 
	
	//�����������Լ�ɾ��...
      if(this == Task_Head) //����ǵ�һ������һ�����һ��
				{
						
					Task_Head = Next;
					Next->Prev=0;

					}else///���м�ĳ�������������������һ�������һ���ǿ�������
					{
 
						Prev->Next = Next;
						Next->Prev = Prev;

					}
					
		   for(Virtual_Task * p = Task_Head; p ; p = p->Next) ///������������
        {
           if(p->CriResType == ((Prior<<24) | WAIT_SIGNL))///�ҵ��˾ͻ�����
					{
						///���������������б���   
						U8 Pri = p->Prior;
						Set_Bit(p->OSReadyGrp, Pri >> 4); ///���������ж���Ϊ��1
            Set_Bit(p->OSReadyTab[Pri>>4], Pri & 0xf);
						
						 
						////��ȡ������������ȴ�״̬...
						  p->CriResType = NON_RESOURCE; ///��ǰ�����Ѿ���������״̬��...
				  }
        }
					
							 
				 Clr_Bit(OSReadyTab[Pr >>4] , Pr & 0xf);///�ھ�������ȡ��������־
        if( OSReadyTab[Pr >>4] == 0)///ȡ���������־
            Clr_Bit(OSReadyGrp, Pr >> 4 );
				
				  delete Stack; 
				
					_Sched(); ////���µ�������
}


////���µ���
void Virtual_Task::_Sched()
{
    Get_Critical();

    Enter_Critical();
	
    U8 Line( MapTab[OSReadyGrp ] );
    U8 Prior( (Line << 4) | ( MapTab[OSReadyTab[Line] ] ) ); ///�õ�������ȼ�

    Line = Cur_Prior; ///Line��Ԥ����һ���������ȼ�����Ϊ����ᱻ���ǵ�

    if(Line != Prior) ///��Ҫ���µ���
    {
        Cur_Prior = Prior;  ///�����������µõ������ȼ��������ˣ�
        TASK_SW(TaskPriorTab[Prior] , TaskPriorTab[Line]); ///������һ�ε�������ֳ����������и����ȼ�����
    }

    Exit_Critical();
}

void  __irq Virtual_Task::_INT_TICK( )
{
    U8  New_Prior, Old_Prior;
    Virtual_Task *p;

    for(p = Task_Head; p ; p = p->Next) ///������������
    {

         if(p->DelayClock)     /////�������������ʱ�ȴ�״̬��..
            if( (--(p->DelayClock)) == 0 )///ʱ�䵽�򽫸�����ӵȴ�������ɾ��������Ϊ����״̬
            {
                New_Prior = p->Prior;

                ///���óɾ���
                Set_Bit(OSReadyGrp, New_Prior >> 4); ///���������ж���Ϊ��1
                Set_Bit(OSReadyTab[New_Prior>>4], New_Prior & 0xf);
            }
    }

    New_Prior = MapTab[OSReadyGrp ];
    New_Prior = (New_Prior << 4) | MapTab[ OSReadyTab[New_Prior] ] ;

    if(Cur_Prior != New_Prior)///���µ����ȼ����ߵ�����Ҫ���µ���
    {
        Old_Prior  = Cur_Prior;
        Cur_Prior = New_Prior;

        INT_TASK_SW(TaskPriorTab[New_Prior] , TaskPriorTab[Old_Prior]); ///������һ�ε�������ֳ����������и����ȼ�����

    }

    Set_Bit(rSRCPND , 8); //д1����ж�
    Set_Bit(rINTPND , 8); //д1����ж�
}
