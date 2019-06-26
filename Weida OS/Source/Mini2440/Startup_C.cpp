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
		   ��д������������Ҫ�ļ�����C����
*************************************************************************/

/////���øú���ǰ�����ȵ���MenInit�����ڴ��ʼ������Ϊ�ó����ж��ڴ���ж�д����������
extern "C" void delay()
{
    int  tem;
    int i = 0x1000;     ///��ȡRW����ֵ��֮ǰ�����ʼ����DRAM
    int j = 0xfff00;
    for(; i > 0; i--)
        for(j = i; j > 0; j--)
        {
            tem = i + j;
        }
	
}



extern "C" void C_Swi_Handle(unsigned int Num)////���жϴ���C������NumΪ���жϺ�
{
	//rGPBCON = (rGPBCON & (~(GPB5_MASK | GPB6_MASK | GPB7_MASK | GPB8_MASK))) | GPB5_OUT|GPB6_OUT|GPB7_OUT;// ����Ϊ���ģʽ
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


/////���øú���ǰ�����ȵ���MenInit�����ڴ��ʼ������Ϊ�ó����ж��ڴ���ж�д����������
extern "C" void  CopyRW_ZI()
{

#define Int_RoLi  ((int)&Image_RO_Limit)//ǿ��ת��Ϊint
#define RoLi_4   (Int_RoLi % 4) //������  

    int *p = (int *)(RoLi_4 ? (Int_RoLi + 4 - RoLi_4) : Int_RoLi) ; //����д��(int*)Image$$RO$$Limit
    int *RW =  &Image_RW_Base;

#undef Int_RoLi
#undef RoLi_4

    if( p != RW)///���������RW Base���͸���RW
        for( ; RW < &Image_ZI_Base ; p++, RW++) *RW = *p ;

    for( p = &Image_ZI_Base; p < &Image_ZI_Limit ; p++)   *p = 0 ; ///��ZI����

}



///����const��ʾ������������RO Data���򣬲�Ȼ�ͻᱻ����RW
int const Men_config[] = ///����������const��������Ȼ���ᱻ��Ϊ��RW��,����ʱRAM��δ��ʼ����
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
   MemInit�������ڴ���г�ʼ��,��Ȼ��C���Ժ���,����δʹ���ڴ��ջ,����ʹ���˼Ĵ�����
   Nor FlashҲ����ROM,������ʹ�øú���ʱ������ʼ���ڴ�Ͷ�ջ. ����ע��,Mem_config����
   �����Ǵ�����ڴ���,����ֻ������������ֻ�����ݲ���!
*********************************************************************************/
extern "C" void MemInit()
{
    int i = 0;
    int *p = (int *)0x48000000;

    for(; i < 13 ; i++, p++)
        *p = Men_config[i];
}