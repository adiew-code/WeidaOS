#ifndef  __CONFIG_H
#define  __CONFIG_H
/***********************************************************************************/
#include"DataTypes.h"



///#define __BIG_ENDIAN //如果是大端方式

#define STACKSIZE   (1024 * 2)

#define Set_Bit(X,n)   ( (X) |= (1<<(n)) )      //将X的第n(0--31)位置为1，其他位不变
#define Clr_Bit(X,n)   ( (X) &= (~ (1<<(n))) ) //将X的第n(0--31)位置为0，其他位不变


////使用前必须定义了Cpu_Sr 局部变量，并且须配套使用
#define Get_Critical()    U32  Cpu_Sr;  //如果函数要进入临界区，必须加上这行
#define Enter_Critical() (Cpu_Sr = WeidaOS_CPU_SR_Save( ))
#define Exit_Critical()  (WeidaOS_CPU_SR_Restore(Cpu_Sr))

extern "C" U32 WeidaOS_CPU_SR_Save( );
extern "C" void WeidaOS_CPU_SR_Restore(U32);



#define INT_TICK_ENABLE()    (Set_Bit(rTICNT , 7)) //时间中断片使能
#define INT_TICK_DISABLE()   (Clr_Bit(rTICNT , 7)) //时间中断片禁止

/********************************************************************************/
#endif