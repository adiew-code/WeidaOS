    GET Start.inc ;;////�������ͷ�ļ�,����ʹ��GET,���Һ�׺����Ϊinc
   
    PRESERVE8   {TRUE} ;///�������Ϳ��Ե���ֱ��bl  delay
	 
	  EXPORT __Start

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;����ʼִ��.......
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;   
   AREA  Reset ,CODE,READONLY
       ENTRY 
__Start
     b	ResetHandler    ;/��λ,�������ģʽ
  	 b	HandlerUndef	;///����δ����ָ��ģʽ
	 b	HandlerSWI	    ;//���������ж�,�������ģʽ
	 b	HandlerPabort	;//ָ��Ԥȡ��ֹ,������ֹģʽ
	 b	HandlerDabort	;//����Ԥȡ��ֹ,������ֹģʽ
	 b	 .              ;//�õ�ַ����
	 b	HandlerIRQ	    ;//������ͨ�ж�ģʽIsrIRQ
	 b	HandlerFIQ	    ;//��������ж�ģʽ
      
;;;;;;///////////////////////////////////////////////////////////////////////////////////////////////////
;;;;;///��һ�����رտ��Ź�////////////////////////////////////////////////////////////////////////////
ResetHandler
   ldr  r0, =0x53000000  ;WTCON���Ź����ƼĴ�����ַ
   mov  r1, #0
   str r1,[r0]   
;;;;//////////////////////////////////////////////////////////////////////////////////////////
;;//////;�ڶ�������ʼ��SDRAM///////////////////////////////////////////////////////////////////
   bl  MemInit  ;;;////C���Ժ���
;;////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
;;;///��ʼ����ģʽ�Ķ�ջָ��!,��Ϊ�����C�������ܻ�ʹ�õ���ջ,�����ڻ��ǵ��ȳ�ʼ���¸���ģʽ�µĶ�ջָ��
InitStacks
  ; ldr r3, = ||Image$$ZeroInit$$ZI$$Limit||;
	mrs	r0,cpsr ;///����CPSR��R0
	
	bic	r0,r0,#MODEMASK
	orr	r1,r0,#UNDEFMODE|NOINT
	msr	cpsr_cxsf,r1		;����δ����ָ��ģʽUndefMode
	ldr	sp,=UndefStack		; UndefStack=0x33ff8000

	orr	r1,r0,#ABORTMODE|NOINT
	msr	cpsr_cxsf,r1		;������ֹģʽAbortMode
	ldr	sp,=AbortStack		; AbortStack=0x33ff7000

	orr	r1,r0,#IRQMODE|NOINT
	msr	cpsr_cxsf,r1		;������ͨ�ж�ģʽIRQMode
	ldr	sp,=IRQStack		; IRQStack=0x33ff6000

	orr	r1,r0,#FIQMODE|NOINT
	msr	cpsr_cxsf,r1		;��������ж�ģʽFIQMode
	ldr	sp,=FIQStack		; FIQStack=0x33ff5000

    

    mov  R1 , #SYSMODE|NOINT;���ܽ����û�ģʽ,�������û�ģʽ���޷���������ģʽ��
	msr	cpsr_cxsf,r1		;����ϵͳģʽSYSMode�ö�ջָ�� ��ϵͳģʽ���û�ģʽ����,��������Ҳ˳���ʼ�����û�ģʽ��ջ
	ldr	sp,=SYSStack		; SYSStack=0x33ff4000

     bic	r0,r0,#MODEMASK|NOINT
	orr	r1,r0,#SVCMODE
	msr	cpsr_cxsf,r1		;���ع���ģʽSVCMode
	ldr	sp,=SVCStack		; SVCStack=0x33ff3000

 ;;;;;;;;;//////////////////////////////////////////////////////////////////////////////////  
 ;;;;///����������ROM�и��Ƴ�ʼ�����ݵ�RW Base��ʼ���ڴ�λ�ã�����ʼ��ZI��Ϊ0��ֱ��ZI Limit//////  
    bl  CopyRW_ZI ;;����C��������RW����ZI  
 
;;;;;////////////////////////////////////////////////////////////////////////////
;;;;;///ѭ���������е�LED��˵��������ʼ,ѭ�����������R4�Ĵ�����//////////////////////////////////////
Led
   ldr  r4,=0x3        ;;;��ʾѭ������Ĵ���r4�Σ������������������������������ѭ��R4��
   ldr r0, =0x56000010    ;;;GPBCON�Ĵ�����ַ
   ldr r1, =0x55555555   
   str r1, [r0]           ;;;����Ϊ���ģʽ  
1   
    ldr r0, =0x56000014    ;;;GPBDAT�Ĵ�����ַ
    ldr  r1,=0x0
    str r1, [r0]           ;;;����Ϊ0��ʾ����    
    
    bl   delay             ;;;��ʱC����
   
   ldr r0, =0x56000014    ;;;GPBDAT�Ĵ�����ַ 
   ldr r2,=0x1E0  ;
   str r2, [r0]           ;;;���
 
    bl   delay   ;;;��ʱC����
 
   subs  r4, r4,#1          ;;r4=r4-1
   bne   %b1                  ;;;��r4 != 0 �����������1��      
;;;;//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
;;;;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
;;;;;////���岽����������CPUʱ��Ƶ�ʺ�����ģʽ���ӿ�ָ��ִ��//////////////
   ldr r0, =0x4c000000  ;;///LockTIme�Ĵ���
   ldr r1,=0xffffffff
   str r1,[r0]
   
   ldr r0,=0x4c000014   ;;;;/////CLKDIVN��Ƶ�Ĵ���
   mov r1,#0x05
   str r1,[r0]
   
   ldr r0,=0x4c000018
   mov r1,#0
   str r1,[r0]
   
   mrc p15,0,r1,c1,c0,0    ;;;//////�޸�����ģʽ
   orr r1,r1 , #0xc0000000
   mcr p15, 0, r1, c1, c0,0
   
   ldr r0,=0x4c000004    ;;;///MPLLCon���ƼĴ���
   ldr r1,=0x0007f021
   str r1,[r0]


;;;;/////////////////////////////////////////////////////////////////////////////////////////////////////////////
;;;////���߲�������C���Եĺ�������ʼ���в���ϵͳ//////////////////////////////////////////////////////////////////////
    b LoadOS 



Offset EQU  0x24
      
    EXPORT  INT_TASK_SW ;;void  INT_TASK_SW(Virtual_Task* New,Virtual_Task* Old); 
INT_TASK_SW   ;;�жϼ�������� ע��˴���R0��Ҫ�ָ������� ָ��..R1��Ҫ���������ָ��
    
	MOV  R2 , SP       ;�ȱ���IRQ��SP	  
    ldr	sp,=IRQStack   ;��������IRQ�µ�SP...
	MRS  R12,  SPSR    ;�õ�SPSR���ŵ�R1Ҳ����ԭ�����CPSR
    
	;����SVCģʽ	 
	MSR  CPSR_cxsf,#SVCMODE|NOINT  ;��ͨ�ж�ģʽ����>����ģʽ(with IRQ&FIQ Disable)
        
		LDR  R4,[R2,#Offset]   ;R2������IRQ��SP������ջ�е�LR���ص�ַȡ��...
        SUB  R4,R4,#0x4      ;LR=LR-4	
        LDR   R3 , [R2 , #Offset-0x4]  ;��������IRQ��ջ�е�R12�ҵ�.. 		
	   
	    STMFD SP!, {R4}	  ;PC
	    STMFD SP!, {LR}	  ;LR
		STMFD SP!, {R3}	  ;R12
	    
		STMFD SP!, {R8-R11}	   ;��R8-R11(δ�ı�),R12,LR ,PCѹ�������ջ...
    
        LDMFD  R2!, {R3-R10}  ;��ԭ�����е�R0--R7������R3--R10��..
       	STMFD SP!, {R3-R10}	 ;;ѹ��ԭ�����R0-R7
		STMFD  SP! , {R12}   ;;ѹ��ԭ�����CPSR
		
 	    str   SP ,  [R1 , #0x4]  ;���� Old �����SPָ��...Old->Cur_Stack = SP
		
	    b  __Restore ;�ָ�����������...R0 ΪNew 
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
TASK_SW  ;;�����������
      STMFD SP!, {LR}              ;PC 
      STMFD SP!, {R0-R12,LR}   
      MRS  R2,  CPSR               ;R2 = CPSR
 	  STMFD SP!, {R2}              ; CPSR 
      STR  SP,  [R1 , #0x4]        ;Old->Cur_Stack = SP           

      EXPORT  __Restore;////�ָ��ֳ�����
__Restore
	ldr  r0,  [R0 , #0x4] ; Cur_Stack�ĵ�ַ...
	MOV 	SP, R0   ;//R0�Ǵ�������SPָ��λ��...       
	
	LDMFD 	SP!, {R0}  ;///����CPSR 
	MSR 	SPSR_cxsf, R0   ;///���浽SPSR
	
	LDMFD 	SP!, {R0-R12, LR, PC}^   ;////�ָ������Ĵ�����PC���������и�����...
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




HandlerFIQ                ;�����ж����
     ; b key_handler;          
          
HandlerIRQ                ;��ͨ�жϴ�����,ͨ��INTOFFSET�Ĵ����������Ǹ��ж�? �ٵ�����Ӧ�ĺ���   
	sub	sp,sp,#4          ;reserved for PC
	stmfd	sp!,{r8-r9}

	ldr	r9,=INTOFFSET     
	ldr	r9,[r9]           ;r9�����ж�ƫ�ƼĴ���INTOFFSET��ֵ,��0--31,��ʾ�жϺ�
	ldr	r8,=HandleEINT0   ;r8���ڴ�Ÿ����жϵ�ַ����׵�ַ,����������ݶ�����
	add	r8,r8,r9,lsl #2   ;r8 = r8 + r9 <<2; r9֮����Ҫ����4����Ϊһ����ַռ4���ֽ�,����....
	ldr	r8,[r8]
	str	r8,[sp,#8]
	ldmfd	sp!,{r8-r9,pc} ;HandlerIRQ      HANDLER HandleIRQ

HandlerUndef  
      b .                 ;HANDLER HandleUndef
    

      IMPORT  C_Swi_Handle  ;����C������
HandlerSWI     
  stmfd sp!, {r0-r3,r12,lr}
  ldr r0, [lr,#-4]     ;�ҵ����ж�ָ��
  bic r0,r0,#0xff000000;�ҵ����жϺţ������ж�ָ��ĵ�24λ	
  bl  C_Swi_Handle     ;��ת��C��������r0��Ϊ�������� 
  ldmfd sp!, {r0-r3,r12,pc}^  ;����              ;HANDLER HandleSWI

HandlerDabort   
      b   .               ;HANDLER HandleDabort

HandlerPabort  
      b   .               ;HANDLER HandlePabort



 ;;;;/////��ֵ�����Bin�ļ��У���������Ӧ����RAM��RW����������CopyRW_ZI()���������þ��ǰ���Щֵ��ROM���Ƶ�RW Base����ȥ
   AREA  Data, DATA,READWRITE   
;;;///��һ��һ���ٲ���:ARBITER0
HandleEINT0	     DCD     0x0	   ;
HandleEINT1	     DCD     0x1       ;ÿ4���ֽڶ�����Ŷ�Ӧ���жϴ�������ַ
HandleEINT2	     DCD     0x2
HandleEINT3	     DCD     0x3

;;;///�ڶ���һ���ٲ���:ARBITER1
HandleEINT4_7	 DCD     0x4
HandleEINT8_23	 DCD     0x5
HandleCAM		 DCD     0x6
HandleBATFLT	 DCD     0x7
HandleTICK		 DCD     0x8
HandleWDT		 DCD     0x9

;;;///������һ���ٲ���:ARBITER2
HandleTIMER0 	 DCD     0xA
HandleTIMER1 	 DCD     0xB
HandleTIMER2 	 DCD     0xC
HandleTIMER3 	 DCD     0xD
HandleTIMER4 	 DCD     0xE
HandleUART2  	 DCD     0xF

;;;///���ĸ�һ���ٲ���:ARBITER3
HandleLCD 		 DCD     0x10
HandleDMA0		 DCD     0x11
HandleDMA1		 DCD     0x12
HandleDMA2		 DCD     0x13
HandleDMA3		 DCD     0x14
HandleMMC		 DCD     0x15

;;;///�����һ���ٲ���:ARBITER4
HandleSPI0		 DCD     0x16
HandleUART1		 DCD     0x17
HandleNFCON		 DCD     0x18
HandleUSBD		 DCD     0x19
HandleUSBH	     DCD     0x1A
HandleIIC		 DCD     0x1B

;;;///������һ���ٲ���:ARBITER5
HandleUART0 	 DCD     0x1C
HandleSPI1 		 DCD     0x1D
HandleRTC 		 DCD     0x1E
HandleADC 		 DCD     0x1F


;;///���߸��Ƕ����ٲ���:ARBITER6,һ��ֱ����CPU����,��һ�˷ֱ����������һ���ٲ���
		
	END
  