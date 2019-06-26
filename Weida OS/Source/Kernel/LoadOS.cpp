#include"Mini2440.h"
#include"WeidaOS.h"


/**********************************************************************************************************
			函数名：LoadOS  //加载操作系统函数
			参数：无
			返回值：无
			函数说明：
						当开发板经过简单的初始化并调用了LoadOS加载系统时，调用此函数
						可以认为这个函数就是整个系统，当这个函数运行退出了，系统也就
						退出了。该函数首先定义了操作系统所要运行的硬件环境，因为这里
						是基于Mini2440开发板，所以定义了一个Mini2440的开发板硬件。后
						面定义了一个操作系统对象，并以开发板作为参数初始化该操作系统
						，当名为OS的系统的初始化函数退出了，系统也随之退出.........
**********************************************************************************************************/


extern "C" void LoadOS()
{

    Mini2440  Weida_Mini2440 ; ///定义一块Mini2440开发板

    WeidaOS   OS(&Weida_Mini2440); ///定义一个名为OS的操作系统，并告诉它该操作系统运行硬件是Mini2440
	
}


