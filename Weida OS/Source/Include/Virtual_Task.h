#ifndef  __WEIDA_VIRTUAL_TASK_H
#define  __WEIDA_VIRTUAL_TASK_H
/***********************************************************************************/
#include"Config.h"
#include"DataTypes.h"
#include"Critical_Res.h"

typedef   int (*Do_Task)( );


#define Delay(Ticks)  (Virtual_Task::_Delay(Ticks))
#define Wait(Prior)   (Virtual_Task::_Wait(Prior))

class  Virtual_Task
{

public:
    /////��Ա����
    Virtual_Task(U8 Prio, Do_Task Main,U32 StackSize);  ///���캯��
    ~Virtual_Task( );                       ///��������

    void  DelTask( );


    ///��̬��������ʵ����ȫ�ֱ�����
    static  U8 const MapTab[65536]; ///������ȼ��ı�
    static U8   Cur_Prior;  ///��ǰ�������ȼ�....

    static Virtual_Task *TaskPriorTab[256]; ///TaskPriorTab[i] Ϊ���ȼ�Ϊi�������TCB��ַ
    static Virtual_Task *Task_Head; ///ָ��˫�������ͷ��...Ϊ0��ʾľ������

    static U16 OSReadyGrp;  ///�����飬ÿλ��Ӧ�����������е�һ��
    static U16 OSReadyTab[16]; ///������ÿλ��ʾ��Ӧ�������Ƿ����...


    ////��̬���� (���� _ + ������),��Ȼ��ʵҲ����ȫ�ֺ���
    static void _OS_Exec(U8 Prior); ////ÿ�������Ǵ����������ʼִ�е�...
    static  void _Delay(U32  Ticks);  ///��ʱTicks��ʱ�ӽ���
    static  void _Sched( ) ; ///�������µ���
    static  U32  _Wait(U8 Prior); //�ȴ�prior�������
		static  void  __irq  _INT_TICK( ); ///�жϴ�����



    ///��Ա����
    U8  Prior;  ///����������ȼ�...
    U8 *Cur_Stack; ///��ǰ����� SP ֵ...��Χ(&Stask[0] ------ &Stask[STACKSIZE-1] )

    U32 DelayClock;  ///��ʱʱ����...

    Virtual_Task *Next;	 ///ָ����һ������.���Ϊ0��ʾ�������һ��
    Virtual_Task *Prev;	 ///ָ����һ������.���Ϊ0��ʾ����ͷ

   /// U32  Task_State;  ///����ǰ״̬ 0 bit Ϊ1��ʾ�ѽ�����ʱ�ȴ�״̬

    U32  CriResType;  ///������������ȴ����ȴ�����Դ������

    U32 const StackSize ;   ///�����ջ��С����ʼ��ʱ��ָ��
protected:

    U32 *Stack;////[STACKSIZE]; ////�����ջ��....

    ////��̬����
    static void _Add_Task(Virtual_Task *p);  ///��p����С���������������


};



///OldΪҪ���������NewΪҪ���е�����  ���ǻ��ʵ��
extern "C" void  TASK_SW(Virtual_Task *New, Virtual_Task *Old);
extern "C" void  INT_TASK_SW(Virtual_Task *New, Virtual_Task *Old);

/********************************************************************************/
#endif