#include"Task.h"
#include"Uart.h"

Task::Task(U8 Prior, Do_Task Main,U32 StackSize): Virtual_Task(Prior, Main,StackSize)
{
    U32 i = StackSize - 1;
    /************************************************************************************************************
    The stack frame of the task to save looks as follows:
    ;
    ;                                         PC        (High memory)
    ;                                         LR(R14)           ^
    ;			           			                    R12               |
    ;           		            			        R11               |
    ;                   		        			    R10               |
    ;                           		   			 	R9                |
    ;                               					R8                |
    ;                               					R7                |
    ;			                               			R6                |
    ;           		                    			R5                |
    ;                   		            			R4                |
    ;                           		    			R3                |
    ;                               					R2                |
    ;                               					R1                |
    ;			                               			R0                |
    ; 					                            	CPSR					(Low memory)
    ;*********************************************************************************************************/

    Stack[i--] = (int)_OS_Exec; ////����PC(R15)ָ��ĳ�ʼֵ
    Stack[i--] = 0;          ////����LR(R14)�ĳ�ʼֵ
    Stack[i--] = 0;          ////����R12�Ĵ����ĳ�ʼֵ
    Stack[i--] = 0;          ////����R11�Ĵ����ĳ�ʼֵ
    Stack[i--] = 0;          ////����R10�Ĵ����ĳ�ʼֵ
    Stack[i--] = 0;          ////����R9�Ĵ����ĳ�ʼֵ
    Stack[i--] = 0;          ////����R8�Ĵ����ĳ�ʼֵ
    Stack[i--] = 0;          ////����R7�Ĵ����ĳ�ʼֵ
    Stack[i--] = 0;          ////����R6�Ĵ����ĳ�ʼֵ
    Stack[i--] = 0;          ////����R5�Ĵ����ĳ�ʼֵ
    Stack[i--] = 0;          ////����R4�Ĵ����ĳ�ʼֵ
    Stack[i--] = 0;          ////����R3�Ĵ����ĳ�ʼֵ
    Stack[i--] = 0;          ////����R2�Ĵ����ĳ�ʼֵ
    Stack[i--] = 0;          ////����R1�Ĵ����ĳ�ʼֵ
    Stack[i--] = Prior;      ////����R0�Ĵ����ĳ�ʼֵ
    Stack[i] = 0x13L;        ////����CPSR�ĳ�ʼֵ:CPSR  (SVC mode, Enable both IRQ and FIQ interrupts)

    Cur_Stack = (U8 *)&Stack[i]; ////��������Spָ��....


}

Task::~Task()
{



}
