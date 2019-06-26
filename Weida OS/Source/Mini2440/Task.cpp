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

    Stack[i--] = (int)_OS_Exec; ////设置PC(R15)指针的初始值
    Stack[i--] = 0;          ////设置LR(R14)的初始值
    Stack[i--] = 0;          ////设置R12寄存器的初始值
    Stack[i--] = 0;          ////设置R11寄存器的初始值
    Stack[i--] = 0;          ////设置R10寄存器的初始值
    Stack[i--] = 0;          ////设置R9寄存器的初始值
    Stack[i--] = 0;          ////设置R8寄存器的初始值
    Stack[i--] = 0;          ////设置R7寄存器的初始值
    Stack[i--] = 0;          ////设置R6寄存器的初始值
    Stack[i--] = 0;          ////设置R5寄存器的初始值
    Stack[i--] = 0;          ////设置R4寄存器的初始值
    Stack[i--] = 0;          ////设置R3寄存器的初始值
    Stack[i--] = 0;          ////设置R2寄存器的初始值
    Stack[i--] = 0;          ////设置R1寄存器的初始值
    Stack[i--] = Prior;      ////设置R0寄存器的初始值
    Stack[i] = 0x13L;        ////设置CPSR的初始值:CPSR  (SVC mode, Enable both IRQ and FIQ interrupts)

    Cur_Stack = (U8 *)&Stack[i]; ////重新设置Sp指针....


}

Task::~Task()
{



}
