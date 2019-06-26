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
    /////成员函数
    Virtual_Task(U8 Prio, Do_Task Main,U32 StackSize);  ///构造函数
    ~Virtual_Task( );                       ///析构函数

    void  DelTask( );


    ///静态变量（其实就是全局变量）
    static  U8 const MapTab[65536]; ///查高优先级的表
    static U8   Cur_Prior;  ///当前任务优先级....

    static Virtual_Task *TaskPriorTab[256]; ///TaskPriorTab[i] 为优先级为i的任务的TCB地址
    static Virtual_Task *Task_Head; ///指向双向链表的头部...为0表示木有任务

    static U16 OSReadyGrp;  ///就绪组，每位对应就绪表数组中的一个
    static U16 OSReadyTab[16]; ///就绪表，每位表示对应的任务是否就绪...


    ////静态函数 (形如 _ + 函数名),当然其实也就是全局函数
    static void _OS_Exec(U8 Prior); ////每个任务都是从这个函数开始执行的...
    static  void _Delay(U32  Ticks);  ///延时Ticks个时钟节拍
    static  void _Sched( ) ; ///任务级重新调度
    static  U32  _Wait(U8 Prior); //等待prior任务结束
		static  void  __irq  _INT_TICK( ); ///中断处理函数



    ///成员变量
    U8  Prior;  ///本任务的优先级...
    U8 *Cur_Stack; ///当前任务的 SP 值...范围(&Stask[0] ------ &Stask[STACKSIZE-1] )

    U32 DelayClock;  ///延时时钟数...

    Virtual_Task *Next;	 ///指向下一个任务.如果为0表示链表最后一个
    Virtual_Task *Prev;	 ///指向上一个任务.如果为0表示链表头

   /// U32  Task_State;  ///任务当前状态 0 bit 为1表示已进入延时等待状态

    U32  CriResType;  ///任务进入阻塞等待所等待的资源链类型

    U32 const StackSize ;   ///任务堆栈大小，初始化时已指定
protected:

    U32 *Stack;////[STACKSIZE]; ////任务堆栈区....

    ////静态函数
    static void _Add_Task(Virtual_Task *p);  ///将p按从小到大插入任务链表


};



///Old为要保存的任务，New为要运行的任务  都是汇编实现
extern "C" void  TASK_SW(Virtual_Task *New, Virtual_Task *Old);
extern "C" void  INT_TASK_SW(Virtual_Task *New, Virtual_Task *Old);

/********************************************************************************/
#endif