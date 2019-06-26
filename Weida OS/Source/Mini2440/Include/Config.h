#ifndef  __CONFIG_H
#define  __CONFIG_H
/***********************************************************************************/
#include"DataTypes.h"



///#define __BIG_ENDIAN //����Ǵ�˷�ʽ

#define STACKSIZE   (1024 * 2)

#define Set_Bit(X,n)   ( (X) |= (1<<(n)) )      //��X�ĵ�n(0--31)λ��Ϊ1������λ����
#define Clr_Bit(X,n)   ( (X) &= (~ (1<<(n))) ) //��X�ĵ�n(0--31)λ��Ϊ0������λ����


////ʹ��ǰ���붨����Cpu_Sr �ֲ�����������������ʹ��
#define Get_Critical()    U32  Cpu_Sr;  //�������Ҫ�����ٽ����������������
#define Enter_Critical() (Cpu_Sr = WeidaOS_CPU_SR_Save( ))
#define Exit_Critical()  (WeidaOS_CPU_SR_Restore(Cpu_Sr))

extern "C" U32 WeidaOS_CPU_SR_Save( );
extern "C" void WeidaOS_CPU_SR_Restore(U32);



#define INT_TICK_ENABLE()    (Set_Bit(rTICNT , 7)) //ʱ���ж�Ƭʹ��
#define INT_TICK_DISABLE()   (Clr_Bit(rTICNT , 7)) //ʱ���ж�Ƭ��ֹ

/********************************************************************************/
#endif