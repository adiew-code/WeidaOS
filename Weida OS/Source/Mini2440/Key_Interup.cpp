

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
 extern "C" int HandleEINT0; ///�ж��������׵�ַ...�ڻ���ļ��ж���

 extern   void KEY_port_init(void)
 {
 	//��ʼ��LED��BEEP��Ҫ�õ���IO   �õ���������˿ڶ�����Ϊ���
// 	rGPBCON &= ~((3<<BEEP)|(3<<LED1)|(3<<LED2)|(3<<LED3)|(3<<LED4));
 	rGPBCON |= (1<<BEEP)|(1<<LED1)|(1<<LED2)|(1<<LED3)|(1<<LED4);
 	
 	//��ʼ��KEY���õ�IO     ���е�KEY�˿ڶ���ʼ��Ϊ�жϷ�ʽ
 	rGPGCON &= ~((3<<KEY1)|(3<<KEY2)|(3<<KEY3)|(3<<KEY4)|(3<<KEY5)|(3<<KEY6));
 	rGPGCON |= (2<<KEY1)|(2<<KEY2)|(2<<KEY3)|(2<<KEY4)|(2<<KEY5)|(2<<KEY6);
 	
 	//�жϳ�ʼ��EINT8
// 	rEXTINT1 &=~((0xf)|(0xf<<12)|(0xf<<20)|(0xf<<24)|(0xf<<28));			//����K1�ĵ͵�ƽ�ж�
 	rEXTINT1 &=~0xfff0f00f;
 	rEXTINT2 &=~(0xf<<12);
 	rEINTPEND |=(1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19);		//���K1���жϱ�־
 	rEINTMASK = ~((1<<8)|(1<<11)|(1<<13)|(1<<14)|(1<<15)|(1<<19));	//��EINT8���ж�ʹ��
 	
 
 	///  SetIRQ(key_handler, 5);
	  int *pIrq = &HandleEINT0;

    pIrq += 5;  ///��װ5���ж�...
    *pIrq = (int)(key_handler);

 	 
 	/// rINTMOD = 0x1<<5 ;  ///����Ϊ���ٿ����ж�
 	 rINTMSK &= ~( 0x1<<5);//�ⲿ�ж�8-23���õ��ж�λʹ�ܣ���ClearPending��Ӧ	
     
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
  //  Key_State=0; ///��ʾ�а�������
 	//Beep_run(); ///Uart0_PutStr("�����ж�..\n");
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
 	
 ///   Uart0_PutStr("�˳��ж�..\n");
	
	rSRCPND = 0x1<<5;
	rINTPND = 0x1<<5;//��������жϱ�־λ�����ȫ�ֵ��ж�	
  
 }
 
 
 