#ifndef  __WEIDA_MINI2440_TASK_H
#define  __WEIDA_MINI2440_TASK_H
/***********************************************************************************/
#include"Virtual_Task.h"


extern "C" void __Save();//�����ֳ������ҷ��ر������ֳ����ָ��
extern "C" void __Restore(Virtual_Task * ); //�ָ��ֳ�������

class   Task : public  Virtual_Task
{
	
public:
/*          ���ȼ�   ִ�еĺ���   ��ջ�ռ��С��Ĭ����STACKSIZE�� */
    Task(U8 Prior, Do_Task Main,U32 StackSize=STACKSIZE);
    ~Task();

protected:



};

/********************************************************************************/
#endif