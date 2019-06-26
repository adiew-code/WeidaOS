#include"Address.h"
#include"Scatter_File.h"

#define GPB5_MASK (2<<(5*2))
#define GPB5_OUT (1<<(5*2))

#define GPB6_MASK (2<<(6*2))
#define GPB6_OUT (1<<(6*2))

#define GPB7_MASK (2<<(7*2))
#define GPB7_OUT (1<<(7*2))

#define GPB8_MASK (2<<(8*2))
#define GPB8_OUT (1<<(8*2))



/***************************************************************************
		   编写启动代码所需要的几个简单C函数
*************************************************************************/

/////调用该函数前必须先调用MenInit进行内存初始化，因为该程序有对内存进行读写！否则会出错！
extern "C" void delay()
{
    int  tem;
    int i = 0x1000;     ///读取RW区的值，之前必须初始化了DRAM
    int j = 0xfff00;
    for(; i > 0; i--)
        for(j = i; j > 0; j--)
        {
            tem = i + j;
        }
	
}



extern "C" void C_Swi_Handle(unsigned int Num)////软中断处理C函数，Num为软中断号
{
	//rGPBCON = (rGPBCON & (~(GPB5_MASK | GPB6_MASK | GPB7_MASK | GPB8_MASK))) | GPB5_OUT|GPB6_OUT|GPB7_OUT;// 设置为输出模式
    switch(Num)
    {
    case 0x00:
        break;
    case 0x01:
        Clr_Bit(rGPBDAT, 5);
        break;
    case 0x02:
        Clr_Bit(rGPBDAT, 6);
        break;
    case 0x03:
        Clr_Bit(rGPBDAT, 7);
        break;
    case 0x04:
        Clr_Bit(rGPBDAT, 8);
        break;
    case 0x05:
        Set_Bit(rGPBDAT, 5);
        break;
    case 0x06:
        Set_Bit(rGPBDAT, 6);
        break;
    case 0x07:
        Set_Bit(rGPBDAT, 7);
        break;
    case 0x08:
        Set_Bit(rGPBDAT, 8);
        break;
    case 0x09:
        break;
    default:
        break;
    }

}


/////调用该函数前必须先调用MenInit进行内存初始化，因为该程序有对内存进行读写！否则会出错！
extern "C" void  CopyRW_ZI()
{

#define Int_RoLi  ((int)&Image_RO_Limit)//强制转换为int
#define RoLi_4   (Int_RoLi % 4) //求余数  

    int *p = (int *)(RoLi_4 ? (Int_RoLi + 4 - RoLi_4) : Int_RoLi) ; //不能写成(int*)Image$$RO$$Limit
    int *RW =  &Image_RW_Base;

#undef Int_RoLi
#undef RoLi_4

    if( p != RW)///如果设置了RW Base，就复制RW
        for( ; RW < &Image_ZI_Base ; p++, RW++) *RW = *p ;

    for( p = &Image_ZI_Base; p < &Image_ZI_Limit ; p++)   *p = 0 ; ///将ZI清零

}



///加了const表示常量，所以在RO Data区域，不然就会被放置RW
int const Men_config[] = ///在这里必须加const变量，不然他会被认为在RW中,而此时RAM并未初始化。
{
    0x22000000,
    0x00000700,
    0x00000700,
    0x00000700,
    0x00000700,
    0x00000700,
    0x00000700,
    0x00018005,
    0x00018005,
    0x008e07a3,
    0x000000b1,
    0x00000030,
    0x00000030
};

/*********************************************************************************
   MemInit函数对内存进行初始化,虽然是C语言函数,但并未使用内存堆栈,仅仅使用了寄存器和
   Nor Flash也就是ROM,所以在使用该函数时允许不初始化内存和堆栈. 而且注意,Mem_config数组
   并不是存放在内存中,而是只读代码段里面的只读数据部分!
*********************************************************************************/
extern "C" void MemInit()
{
    int i = 0;
    int *p = (int *)0x48000000;

    for(; i < 13 ; i++, p++)
        *p = Men_config[i];
}