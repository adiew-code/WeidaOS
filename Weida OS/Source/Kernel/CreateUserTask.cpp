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


///闪灯程序
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
	
	/*********************创建用户任务****************************/
    
	       Task Light(0x20, Lighting);  //闪灯
	
         Task Music(0x40, Play_Music);  ///播放音乐
	
         Task Snake(0x10, Play_Snake);    ///贪吃蛇小游戏

		 
    /*************************************************************/ 
	
	 WeidaOS_Run() ; ////系统开始运行....
}
