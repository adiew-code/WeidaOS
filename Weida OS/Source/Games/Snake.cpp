#include"Snake.h"
#include"Math.h"

 extern   void KEY_port_init(void);
 
int  x,y;
int  key=RIGHT;
///int Key_State=1; //����һ����Ϊ��������ʼ��Ϊ��


struct SNAKE
{     int x[300];
      int y[300];
      int jie;
      int life;
      int direction;
}snake;

int gamespeed = 20;

struct Food
{   int  x;
    int y;
    int  need;
}food;

unsigned long  score=0; 

 

  
/*
 void  Play_0000Snake()    ///��Ϸ��ʼ
{ 
  unsigned  char Ans;
   
  loop: 
      choosespeed();
	  playgame();
	
	 // Uart0_PutStr("\nDo you want Try Again('y' or 'n'?):");
	  
	  Ans=Uart0_Getc();
	  if (Ans == 'Y' || Ans == 'y')
	      goto loop;
}
   
 int choosespeed()  ///ѡ���ٶ�
{   
  unsigned char   num;

	WeidaOS::Cur_OS->Uart_Printf("\nplease  input the speed you want (from 1 to 9):");
  
    
  ///num=Uart0_Getc();

  if(num>='1'&&num<='9')
  {   WeidaOS::Cur_OS->Uart_Printf("\nThank  you ,Enjoy It....\n");
      gamespeed= 60000 - (num-'1')*7375;
  }
  else
 {    
	  WeidaOS::Cur_OS->Uart_Printf("\nPlease Check the number and Try again...  Thank you !Good Bye........!!\n");
      // Uart0_Getc();    
       return  -1; ///����
  }
  return  0; ///��ȷ
}  
   */
   
void  playsnake()///����Ϸ�Ĺؼ�����
{ 
	   KEY_port_init();
     Rand  RandNum;
	   
	//  RandNum.srandom();
	
	int i,last_x,last_y; snake.jie=2; score=0; snake.direction=R;  food.need=you;  snake.life=1;
   snake.x[0]=10; snake.y[0]=10;
   snake.x[1]=20;snake.y[1]=10;  
    food.x=100;  food.y=100;
   
     key=RIGHT;
    WeidaOS::Cur_OS-> Clear(Snake_BkColor);
  
     WeidaOS::Cur_OS->FilledRectangle(snake.x[1],snake.y[1],snake.x[1]+10,snake.y[1]+10,Snake_Color);
	  WeidaOS::Cur_OS->FilledRectangle(snake.x[0],snake.y[0],snake.x[0]+10,snake.y[0]+10,Snake_Color);

       while(1)  ///ѭ������
      {                        
					 last_x= snake.x[0],last_y=snake.y[0];

                     WeidaOS::Cur_OS->FilledRectangle(last_x,last_y,last_x+10,last_y+10,Snake_BkColor);
      
	                 for(i=1;i<=snake.jie-1;i++)
                          snake.x[i-1]=snake.x[i],snake.y[i-1]=snake.y[i];

                     switch(snake.direction)///�ж���ҿ��Ƶķ���
                     {
		                   case R:     snake.x[snake.jie-1]+=10;   break;
	                     case L:     snake.x[snake.jie-1]-=10;   break;
	                     case U:     snake.y[snake.jie-1]-=10;   break;
	                     case D:     snake.y[snake.jie-1]+=10;   break;
	                 } 
 
                     while(snake.jie>=4)
                    {  
			            for(i=snake.jie-4;i>=0&&!(snake.x[snake.jie-1]==snake.x[i]&&snake.y[snake.jie-1]==snake.y[i]);i--);

                          if(snake.x[snake.jie-1]==snake.x[i]&&snake.y[snake.jie-1]==snake.y[i])
			              {
				                   snake.life=0;
				                   WeidaOS::Cur_OS->Uart_Printf("\n��ϲ��ҧ�����Լ�!");  //Uart0_Getc(); 
											     return ;
			              }else
				              break;
                      } 
		      
				   if(snake.life==0) break; ///���������ҧβ! 
                   
		           if(snake.x[snake.jie-1]>(LCD_WIDTH-10)||snake.x[snake.jie-1]<0||snake.y[snake.jie-1]>(LCD_HEIGHT-10)||snake.y[snake.jie-1]<0)////ײǽ��
                   { 
			           snake.life=0;    
			            for(i=0;i<=snake.jie-1;i++) ///�ٻ���ͼ��
                          WeidaOS::Cur_OS->FilledRectangle(snake.x[i],snake.y[i],snake.x[i]+10,snake.y[i]+10,Snake_Color);
			           WeidaOS::Cur_OS->Uart_Printf("\n��ϲ��,�ɹ����Լ�ײ����!");
			             break;  
		           }


                   if(snake.x[snake.jie-1]==food.x&&snake.y[snake.jie-1]==food.y)///�Ե�ʳ���Ķ���
                   {
                       WeidaOS::Cur_OS-> FilledRectangle(food.x,food.y,food.x+10,food.y+10,Snake_Color);
                         food.need= mei;  score++;    //  pfscore();
                        WeidaOS::Cur_OS->FilledRectangle(last_x,last_y,last_x+10,last_y+10,Snake_Color);
                          snake.jie++;
              
				        for(i=snake.jie-1;i>=1;i--)
					        snake.x[i]=snake.x[i-1], snake.y[i]=snake.y[i-1];  

				          snake.x[0]=last_x,snake.y[0]=last_y;
		            }  
		
                    for(i=0;i<=snake.jie-1;i++) ///����ͼ��
                       WeidaOS::Cur_OS-> FilledRectangle(snake.x[i],snake.y[i],snake.x[i]+10,snake.y[i]+10,Snake_Color);
          
			        if(food.need==mei)///ûʳ��ͳ���ʳ��
                    { 
		                food.x= RandNum.random(320);  food.y=RandNum.random(240);
										
                    food.x -= (food.x %10);		food.y -= (food.y %10);										
											
	                   food.need=you;
	                 }else ///��ʳ��͵���ʾRed   
                        WeidaOS::Cur_OS->FilledRectangle(food.x,food.y,food.x+10,food.y+10,Food_Color);
                    

                        Delay(gamespeed);///DELAY( gamespeed);///�ٶȿ���

            if(snake.life==0) break;
 
            if(key==ESC)
		            break;
            if(key==UP&&snake.direction!=D)
		           snake.direction=U;
            if(key==DOWN&&snake.direction!=U)
		           snake.direction=D;
            if(key==LEFT&&snake.direction!=R)
		           snake.direction=L;
	        if(key==RIGHT&&snake.direction!=L)
		           snake.direction= R;
  
       }

}
 void  pfscore()////��ӡ����
{  
}
