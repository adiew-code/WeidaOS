#include"Mini2440.h"

extern const unsigned char Beautiful_Girl[];
extern "C" void MMU_Init();

Mini2440::Mini2440()
{
    MMU_Init();//��ʼ��MMU
    ///Uart0.Uart_PutStr("\n��ʹ�õ���Mini2440�����壬Ӳ���Ѿ��ɹ���ʼ��....\n");///��ӡ��Ϣ....
}

Mini2440::~Mini2440()
{
    ///Uart0.Uart_PutStr("\nMini2440�����˳�....\n");///��ӡ��Ϣ....
}


///��װ����Ӳ��������Ӳ��ָ����ϲ�
void Mini2440::SetDevices( )
{
   pUart   = &Uart0 ;	//Ӳ������
   pLCD    = &W35;  //Ӳ��LCD��ʾ��
   pRTC    = &Clock; ///ʵʱʱ��
   pTouSc  = &TouSc;	//������...
   pMem    = &Mem;  ///�ڴ棬����ϵͳ��̬����	  
	
}

