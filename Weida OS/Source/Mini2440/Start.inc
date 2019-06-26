
	IMPORT  ||Image$$ExecutableAddress$$RO$$Length||
	IMPORT  ||Image$$ExecutableAddress$$RO$$Base||	;Base of ROM code  	
	IMPORT  ||Image$$ExecutableAddress$$RO$$Limit||
	

	IMPORT  ||Image$$ReadWriteAddress$$RW$$Length||
	IMPORT  ||Image$$ReadWriteAddress$$RW$$Base||	   ; Base of RAM Data  	
	IMPORT  ||Image$$ReadWriteAddress$$RW$$Limit||    ; Limit of RAM  Data


	IMPORT  ||Image$$ZeroInit$$ZI$$Length||
	IMPORT  ||Image$$ZeroInit$$ZI$$Base||	     ;Base and limit of area
	IMPORT  ||Image$$ZeroInit$$ZI$$Limit||	     ;to zero initialise

	IMPORT  delay
	IMPORT  LoadOS
	IMPORT  MemInit
	IMPORT  CopyRW_ZI  

   
_STACK_BASEADDRESS	EQU 0x33ff7000 ;///0x33ff9000

IRQ_STACK_SIZE   EQU  0x400
SVCStack	EQU	(_STACK_BASEADDRESS-0x3000)	;	
SYSStack    EQU	(_STACK_BASEADDRESS-0x4000)	;
FIQStack	EQU	(_STACK_BASEADDRESS-0x3000)	;
IRQStack	EQU	(_STACK_BASEADDRESS-0x2000)	;
AbortStack  EQU	(_STACK_BASEADDRESS-0x1000)	;
UndefStack	EQU	(_STACK_BASEADDRESS-0x0)	;


USERMODE    EQU 	0x10
FIQMODE     EQU 	0x11
IRQMODE     EQU 	0x12
SVCMODE     EQU 	0x13
ABORTMODE   EQU 	0x17
UNDEFMODE   EQU 	0x1b
SYSMODE     EQU   0x1f
MODEMASK    EQU 	0x1f
NOINT       EQU 	0xc0

INTOFFSET	EQU  0x4a000014    ;Interruot request source offset



SRCPND         EQU   0x4a000000
INTPND         EQU   0x4a000010


	EXPORT  HandleEINT0

    
    
    
    
    END