

//---------------------Define Valurable-------------------------------//

#include"Snake.h"


#define LED1_ON   ~(1<<5)
#define LED2_ON   ~(1<<6)
#define LED3_ON   ~(1<<7)
#define LED4_ON   ~(1<<8)

#define LED1_OFF (1<<5)
#define LED2_OFF (1<<6)
#define LED3_OFF (1<<7)
#define LED4_OFF (1<<8)

#define BEEP 0

#define LED1 10
#define LED2 12
#define LED3 14
#define LED4 16

#define KEY1 0
#define KEY2 6
#define KEY3 10
#define KEY4 12
#define KEY5 14
#define KEY6 22

//--------------------Function Prototype------------------------------//

static void Show_num(int num);
static void Beep_run(void);
static void delay(unsigned int times);
extern void __irq key_handler(void);
 extern "C" int HandleEINT0; ///中断向量表首地址...在汇编文件中定义

 extern   void KEY_port_init(void)
 {
 	//初始化LED与BEEP所要用到的IO   用到的这五个端口都配置为输出
// 	rGPBCON &= ~((3<<BEEP)|(3<<LED1)|(3<<LED2)|(3<<LED3)|(3<<LED4));
 	rGPBCON |= (1<<BEEP)|(1<<LED1)|(1<<LED2)|(1<<LED3)|(1<<LED4);
 	
 	//初始化KEY所用的IO     所有的KEY端口都初始化为中断方式
 	rGPGCON &= ~((3<<KEY1)|(3<<KEY2)|(3<<KEY3)|(3<<KEY4)|(3<<KEY5)|(3<<KEY6));
 	rGPGCON |= (2<<KEY1)|(2<<KEY2)|(2<<KEY3)|(2<<KEY4)|(2<<KEY5)|(2<<KEY6);
 	
 	//中断初始化EINT8
// 	rEXTINT1 &=~((0xf)|(0xf<<12)|(0xf<<20)|(0xf<<24)|(0xf<<28));			//设置K1的低电平中断
 	rEXTINT1 &=~0xfff0f00f;
 	rEXTINT2 &=~(0xf<<12);
 	rEINTPEND |=(1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19);		//清除K1的中断标志
 	rEINTMASK = ~((1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19));	//打开EINT8的中断使能
 	
 
 	///  SetIRQ(key_handler, 5);
	  int *pIrq = &HandleEINT0;

    pIrq += 5;  ///安装5号中断...
    *pIrq = (int)(key_handler);

 	 
 	/// rINTMOD = 0x1<<5 ;  ///设置为快速快速中断
 	 rINTMSK &= ~( 0x1<<5);//外部中断8-23共用的中断位使能，与ClearPending对应	
     
 }
 

 static void Show_num(int data)
 {
 	if(data & 0x08)   rGPBDAT = rGPBDAT&(LED4_ON);
    else           rGPBDAT = rGPBDAT|(LED4_OFF);
  	if(data & 0x04)   rGPBDAT = rGPBDAT&(LED3_ON);
    else           rGPBDAT = rGPBDAT|(LED3_OFF);
  	if(data & 0x02)   rGPBDAT = rGPBDAT&(LED2_ON);
    else           rGPBDAT = rGPBDAT|(LED2_OFF);
  	if(data & 0x01)   rGPBDAT = rGPBDAT&(LED1_ON);
    else           rGPBDAT = rGPBDAT|(LED1_OFF);
 }
 

 static void Beep_run(void)
{
    rGPBDAT |= (0x1<<0);
    delay(50);
    rGPBDAT &= (0x0<<0);
    delay(50);
}
 
 
static void delay(unsigned int times)
{
	unsigned int i;
	for(;times>0;times--)
	{
		for(i=400;i>0;i--);
	}
}



extern int Key_State;
  void __irq key_handler(void)
 {
  //  Key_State=0; ///表示有按键按下
 	//Beep_run(); ///Uart0_PutStr("进入中断..\n");
 	if(rEINTPEND & (1<<8))
 	{
 		rEINTPEND |=1<<8;
 		//Show_num(1); 
		key=ESC;
 	}
 	else if(rEINTPEND & (1<<11))
 	{
 		rEINTPEND |=1<<11;
 	//Show_num(2); 
		key=UP;
 	}
	else if(rEINTPEND & (1<<13))
 	{
 	 	rEINTPEND |=1<<13;
 	//Show_num(3); 
		key=Enter;
 	} 	
 	else if(rEINTPEND & (1<<14))
 	{
 		rEINTPEND |=1<<14;
 	//	Show_num(4);
		key=DOWN;
 	}
 	else if(rEINTPEND & (1<<15))
 	{
 		rEINTPEND |=1<<15;
 		//Show_num(5); 
		key=LEFT;
 	}
 	else if(rEINTPEND & (1<<19))
 	{
	 	rEINTPEND |=1<<19;
 		//Show_num(6);  
		key=RIGHT;
 	}
 	
 ///   Uart0_PutStr("退出中断..\n");
	
	rSRCPND = 0x1<<5;
	rINTPND = 0x1<<5;//用来清除中断标志位；针对全局的中断	
  
 }
 
 
 