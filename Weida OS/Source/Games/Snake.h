#ifndef  __SNAKE_H
#define  __SNAKE_H
#include"DataTypes.h"
#include "WeidaOS.h"



#define  R  0
#define  U  1
#define   D  2
#define   L  3
#define  mei 0
#define   you  1

#define  UP    0x4800 
#define  DOWN  0x5000 
#define  LEFT  0x4b00  
#define  RIGHT 0x4d00  

#define  ESC   0x11b 
#define  Enter 0xfff

#define Snake_Color   0x60fd
#define Food_Color    0xfd45
#define Snake_BkColor 0xffffff ////����


extern int  key;
extern int Key_State; //����һ����Ϊ��������ʼ��Ϊ��















#endif