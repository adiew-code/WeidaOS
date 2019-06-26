    GET Start.inc ;;////包涵汇编头文件,必须使用GET,而且后缀必须为inc
   
    PRESERVE8   {TRUE} ;///加上这句就可以调用直接bl  delay
	 
	  EXPORT __Start

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;程序开始执行.......
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;   
   AREA  Reset ,CODE,READONLY
       ENTRY 
__Start
     b	ResetHandler    ;/复位,进入管理模式
  	 b	HandlerUndef	;///进入未定义指令模式
	 b	HandlerSWI	    ;//调用了软中断,进入管理模式
	 b	HandlerPabort	;//指令预取中止,进入中止模式
	 b	HandlerDabort	;//数据预取中止,进入中止模式
	 b	 .              ;//该地址保留
	 b	HandlerIRQ	    ;//进入普通中断模式IsrIRQ
	 b	HandlerFIQ	    ;//进入快速中断模式
      
;;;;;;///////////////////////////////////////////////////////////////////////////////////////////////////
;;;;;///第一步，关闭开门狗////////////////////////////////////////////////////////////////////////////
ResetHandler
   ldr  r0, =0x53000000  ;WTCON看门狗控制寄存器地址
   mov  r1, #0
   str r1,[r0]   
;;;;//////////////////////////////////////////////////////////////////////////////////////////
;;//////;第二步，初始化SDRAM///////////////////////////////////////////////////////////////////
   bl  MemInit  ;;;////C语言函数
;;////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
;;;///初始化各模式的堆栈指针!,因为下面的C函数可能会使用到堆栈,所以在还是得先初始化下各个模式下的堆栈指针
InitStacks
  ; ldr r3, = ||Image$$ZeroInit$$ZI$$Limit||;
	mrs	r0,cpsr ;///加载CPSR到R0
	
	bic	r0,r0,#MODEMASK
	orr	r1,r0,#UNDEFMODE|NOINT
	msr	cpsr_cxsf,r1		;进入未定义指令模式UndefMode
	ldr	sp,=UndefStack		; UndefStack=0x33ff8000

	orr	r1,r0,#ABORTMODE|NOINT
	msr	cpsr_cxsf,r1		;进入中止模式AbortMode
	ldr	sp,=AbortStack		; AbortStack=0x33ff7000

	orr	r1,r0,#IRQMODE|NOINT
	msr	cpsr_cxsf,r1		;进入普通中断模式IRQMode
	ldr	sp,=IRQStack		; IRQStack=0x33ff6000

	orr	r1,r0,#FIQMODE|NOINT
	msr	cpsr_cxsf,r1		;进入快速中断模式FIQMode
	ldr	sp,=FIQStack		; FIQStack=0x33ff5000

    

    mov  R1 , #SYSMODE|NOINT;不能进入用户模式,进入了用户模式就无法进入其他模式了
	msr	cpsr_cxsf,r1		;进入系统模式SYSMode该堆栈指针 由系统模式和用户模式共用,所以这里也顺便初始化了用户模式堆栈
	ldr	sp,=SYSStack		; SYSStack=0x33ff4000

     bic	r0,r0,#MODEMASK|NOINT
	orr	r1,r0,#SVCMODE
	msr	cpsr_cxsf,r1		;返回管理模式SVCMode
	ldr	sp,=SVCStack		; SVCStack=0x33ff3000

 ;;;;;;;;;//////////////////////////////////////////////////////////////////////////////////  
 ;;;;///第三步，从ROM中复制初始化数据到RW Base开始的内存位置，并初始化ZI区为0，直到ZI Limit//////  
    bl  CopyRW_ZI ;;调用C函数复制RW并且ZI  
 
;;;;;////////////////////////////////////////////////////////////////////////////
;;;;;///循环点灭所有的LED，说明启动开始,循环次数存放在R4寄存器中//////////////////////////////////////
Led
   ldr  r4,=0x3        ;;;表示循环点灭的次数r4次，灯先亮，呆会在灭，再亮，再灭，如此循环R4次
   ldr r0, =0x56000010    ;;;GPBCON寄存器地址
   ldr r1, =0x55555555   
   str r1, [r0]           ;;;设置为输出模式  
1   
    ldr r0, =0x56000014    ;;;GPBDAT寄存器地址
    ldr  r1,=0x0
    str r1, [r0]           ;;;设置为0表示灯亮    
    
    bl   delay             ;;;延时C函数
   
   ldr r0, =0x56000014    ;;;GPBDAT寄存器地址 
   ldr r2,=0x1E0  ;
   str r2, [r0]           ;;;灭灯
 
    bl   delay   ;;;延时C函数
 
   subs  r4, r4,#1          ;;r4=r4-1
   bne   %b1                  ;;;如r4 != 0 就跳到上面的1处      
;;;;//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
;;;;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
;;;;;////第五步，重新设置CPU时钟频率和总线模式，加快指令执行//////////////
   ldr r0, =0x4c000000  ;;///LockTIme寄存器
   ldr r1,=0xffffffff
   str r1,[r0]
   
   ldr r0,=0x4c000014   ;;;;/////CLKDIVN分频寄存器
   mov r1,#0x05
   str r1,[r0]
   
   ldr r0,=0x4c000018
   mov r1,#0
   str r1,[r0]
   
   mrc p15,0,r1,c1,c0,0    ;;;//////修改总线模式
   orr r1,r1 , #0xc0000000
   mcr p15, 0, r1, c1, c0,0
   
   ldr r0,=0x4c000004    ;;;///MPLLCon控制寄存器
   ldr r1,=0x0007f021
   str r1,[r0]


;;;;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
;;;////第七步，跳到C语言的函数处开始运行操作系统//////////////////////////////////////////////////////////////////////
    b LoadOS 



Offset EQU  0x24
      
    EXPORT  INT_TASK_SW ;;void  INT_TASK_SW(Virtual_Task* New,Virtual_Task* Old); 
INT_TASK_SW   ;;中断级任务调度 注意此处的R0是要恢复的任务 指针..R1是要保存的任务指针
    
	MOV  R2 , SP       ;先保存IRQ的SP	  
    ldr	sp,=IRQStack   ;重新设置IRQ下的SP...
	MRS  R12,  SPSR    ;得到SPSR，放到R1也就是原任务的CPSR
    
	;进入SVC模式	 
	MSR  CPSR_cxsf,#SVCMODE|NOINT  ;普通中断模式——>管理模式(with IRQ&FIQ Disable)
        
		LDR  R4,[R2,#Offset]   ;R2保存了IRQ的SP，将堆栈中的LR返回地址取出...
        SUB  R4,R4,#0x4      ;LR=LR-4	
        LDR   R3 , [R2 , #Offset-0x4]  ;将保存在IRQ堆栈中的R12找到.. 		
	   
	    STMFD SP!, {R4}	  ;PC
	    STMFD SP!, {LR}	  ;LR
		STMFD SP!, {R3}	  ;R12
	    
		STMFD SP!, {R8-R11}	   ;将R8-R11(未改变),R12,LR ,PC压入任务堆栈...
    
        LDMFD  R2!, {R3-R10}  ;将原任务中的R0--R7弹出到R3--R10中..
       	STMFD SP!, {R3-R10}	 ;;压入原任务的R0-R7
		STMFD  SP! , {R12}   ;;压入原任务的CPSR
		
 	    str   SP ,  [R1 , #0x4]  ;保存 Old 任务的SP指针...Old->Cur_Stack = SP
		
	    b  __Restore ;恢复新任务运行...R0 为New 
;/***************************************************************************************************
;          	   The stack frame of the task to resume looks as follows:
;
;			  		  								PC					(High memory)
;                                                   LR(R14)	
;			           			                    R12
;           		            			        R11
;                   		        			    R10
;                           		   			 	R9
;                               					R8
;                               					R7
;			                               			R6
;           		                    			R5
;                   		            			R4
;                           		    			R3
;                               					R2
;                               					R1
;			                               			R0
;                                                   CPSR                 (Low memory)
;*********************************************************************************************************/
       EXPORT   TASK_SW  ;void  TASK_SW(Virtual_Task* New,Virtual_Task* Old); 
TASK_SW  ;;任务级任务调度
      STMFD SP!, {LR}              ;PC 
      STMFD SP!, {R0-R12,LR}   
      MRS  R2,  CPSR               ;R2 = CPSR
 	  STMFD SP!, {R2}              ; CPSR 
      STR  SP,  [R1 , #0x4]        ;Old->Cur_Stack = SP           

      EXPORT  __Restore;////恢复现场工作
__Restore
	ldr  r0,  [R0 , #0x4] ; Cur_Stack的地址...
	MOV 	SP, R0   ;//R0是传过来的SP指针位置...       
	
	LDMFD 	SP!, {R0}  ;///弹出CPSR 
	MSR 	SPSR_cxsf, R0   ;///保存到SPSR
	
	LDMFD 	SP!, {R0-R12, LR, PC}^   ;////恢复各个寄存器和PC，继续运行该任务...
;;;End  of  __Restore 
 
 
 
 
   EXPORT  WeidaOS_CPU_SR_Save
WeidaOS_CPU_SR_Save   ; U32 WeidaOS_CPU_SR_Save(); 
   MRS  R0 , CPSR
   ORR  R1 , R0 , #NOINT 
   MSR  CPSR_c , R1 
   BX   LR             ;Return back

  




    EXPORT   WeidaOS_CPU_SR_Restore
WeidaOS_CPU_SR_Restore  ;WeidaOS_CPU_SR_Restore(U32 Cpsr)
   MSR  CPSR_c , R0
   BX   LR     ;Return




HandlerFIQ                ;快速中断入口
     ; b key_handler;          
          
HandlerIRQ                ;普通中断处理函数,通过INTOFFSET寄存器查找是那个中断? 再调用相应的函数   
	sub	sp,sp,#4          ;reserved for PC
	stmfd	sp!,{r8-r9}

	ldr	r9,=INTOFFSET     
	ldr	r9,[r9]           ;r9等于中断偏移寄存器INTOFFSET的值,从0--31,表示中断号
	ldr	r8,=HandleEINT0   ;r8等于存放各个中断地址表的首地址,在下面的数据定义区
	add	r8,r8,r9,lsl #2   ;r8 = r8 + r9 <<2; r9之所以要乘以4是因为一个地址占4个字节,所以....
	ldr	r8,[r8]
	str	r8,[sp,#8]
	ldmfd	sp!,{r8-r9,pc} ;HandlerIRQ      HANDLER HandleIRQ

HandlerUndef  
      b .                 ;HANDLER HandleUndef
    

      IMPORT  C_Swi_Handle  ;导入C处理函数
HandlerSWI     
  stmfd sp!, {r0-r3,r12,lr}
  ldr r0, [lr,#-4]     ;找到软中断指令
  bic r0,r0,#0xff000000;找到软中断号，即软中断指令的低24位	
  bl  C_Swi_Handle     ;跳转至C处理函数，r0作为参数传递 
  ldmfd sp!, {r0-r3,r12,pc}^  ;返回              ;HANDLER HandleSWI

HandlerDabort   
      b   .               ;HANDLER HandleDabort

HandlerPabort  
      b   .               ;HANDLER HandlePabort



 ;;;;/////该值存放在Bin文件中，但是他本应该在RAM的RW区，，所以CopyRW_ZI()函数的作用就是把这些值从ROM复制到RW Base里面去
   AREA  Data, DATA,READWRITE   
;;;///第一个一级仲裁器:ARBITER0
HandleEINT0	     DCD     0x0	   ;
HandleEINT1	     DCD     0x1       ;每4个字节都存放着对应的中断处理函数地址
HandleEINT2	     DCD     0x2
HandleEINT3	     DCD     0x3

;;;///第二个一级仲裁器:ARBITER1
HandleEINT4_7	 DCD     0x4
HandleEINT8_23	 DCD     0x5
HandleCAM		 DCD     0x6
HandleBATFLT	 DCD     0x7
HandleTICK		 DCD     0x8
HandleWDT		 DCD     0x9

;;;///第三个一级仲裁器:ARBITER2
HandleTIMER0 	 DCD     0xA
HandleTIMER1 	 DCD     0xB
HandleTIMER2 	 DCD     0xC
HandleTIMER3 	 DCD     0xD
HandleTIMER4 	 DCD     0xE
HandleUART2  	 DCD     0xF

;;;///第四个一级仲裁器:ARBITER3
HandleLCD 		 DCD     0x10
HandleDMA0		 DCD     0x11
HandleDMA1		 DCD     0x12
HandleDMA2		 DCD     0x13
HandleDMA3		 DCD     0x14
HandleMMC		 DCD     0x15

;;;///第五个一级仲裁器:ARBITER4
HandleSPI0		 DCD     0x16
HandleUART1		 DCD     0x17
HandleNFCON		 DCD     0x18
HandleUSBD		 DCD     0x19
HandleUSBH	     DCD     0x1A
HandleIIC		 DCD     0x1B

;;;///第六个一级仲裁器:ARBITER5
HandleUART0 	 DCD     0x1C
HandleSPI1 		 DCD     0x1D
HandleRTC 		 DCD     0x1E
HandleADC 		 DCD     0x1F


;;///第七个是二级仲裁器:ARBITER6,一端直接与CPU相连,另一端分别连接上面的一级仲裁器
		
	END
  