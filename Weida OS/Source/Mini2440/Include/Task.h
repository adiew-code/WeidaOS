#ifndef  __WEIDA_MINI2440_TASK_H
#define  __WEIDA_MINI2440_TASK_H
/***********************************************************************************/
#include"Virtual_Task.h"


extern "C" void __Save();//保存现场，并且返回保存了现场后的指针
extern "C" void __Restore(Virtual_Task * ); //恢复现场．．．

class   Task : public  Virtual_Task
{
	
public:
/*          优先级   执行的函数   堆栈空间大小（默认是STACKSIZE） */
    Task(U8 Prior, Do_Task Main,U32 StackSize=STACKSIZE);
    ~Task();

protected:



};

/********************************************************************************/
#endif