#include"WeidaOS.h"
#include"../MusicData/WindowsXP_Wav.cpp"
void PlayMusic(const U8  *Wav, U32 Len);
int  playsnake();


S32  Play_Snake()
{
	for(;;)
   {
		 playsnake();
		 
		 Delay(50);
	 }	
    return 0;
}


///���Ƴ���
S32 Lighting()
{
    for(; ;)
    {
        LED1_ON();   Delay(20);

        LED2_ON();   Delay(15);

        LED3_ON();   Delay(5);

        LED4_ON();   Delay(10);

        LED1_OFF();  Delay(5);
			
        LED2_OFF();  Delay(5);
			
        LED3_OFF();  Delay(5);
			
        LED4_OFF();  Delay(5);
			
        Delay(15);
			
			  LED1_ON(); LED2_ON(); LED3_ON(); LED4_ON(); 
			    Delay(15);
			
			  LED1_OFF(); LED2_OFF(); LED3_OFF(); LED4_OFF();
				 Delay(15);
			
    }
}

S32  Play_Music()
{
	for(;;)
	 {
		 PlayMusic( WindowsXP_Wav , sizeof(WindowsXP_Wav));
		 
		 Delay(15);
	 }
	return 0;
}

void   WeidaOS::CreateUserTask( )
{
	
	/*********************�����û�����****************************/
    
	       Task Light(0x20, Lighting);  //����
	
         Task Music(0x40, Play_Music);  ///��������
	
         Task Snake(0x10, Play_Snake);    ///̰����С��Ϸ

		 
    /*************************************************************/ 
	
	 WeidaOS_Run() ; ////ϵͳ��ʼ����....
}
