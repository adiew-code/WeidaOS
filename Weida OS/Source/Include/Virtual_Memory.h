#ifndef  __WEIDA_VIRTUAL_MEMORY_H
#define  __WEIDA_VIRTUAL_MEMORY_H
/***********************************************************************************
                                    �ڴ����
							 �޸�ʱ�䣺2012/12/3   10:12 am
							 ���ߣ�weida
							 ���ݣ���ϵͳ�����ڴ���й�����������newԤ���ʹ�û�����
							        ��̬�����ڴ档
*************************************************************************************/
#include"DataTypes.h"
#include"Critical_Res.h"
#include"Virtual_Task.h"
#include "new"


struct MemBlock
{
	U32   * Block_StartAddr; ///�ڴ����ʼ��ַ
	U32     Block_Size;      //��С�����ֽڼ���
	struct MemBlock *Next;   //ָ����һ���ڴ�飬0��ʾ���һ��
	struct MemBlock *Prev;   ///ָ����һ���ڴ�飬0��ʾ��һ��
	
	U32    Used;  //���ڴ���ʹ����������忴���棺
/************************************************************
         Which?        Unused           How Much  
    --------------------------------------------
	  |31        24|23                8|7       0|
    --------------------------------------------
1���Ͱ�λ��ʾ��ǰ�ڴ���ж��ٸ�������ʹ�ã����������Թ���һ���ڴ棩
2���߰�λ��ʾ��ǰ�ĸ���������ʹ�ø��ڴ�
3�����Ͱ�λΪ0���ʾ���ڴ�������У��������ڱ�ʹ��
4������߰�λΪ0xff���ʾ���ڴ���ϵͳ���룬���ɱ���ͨ����ɾ��
*************************************************************/	
	
};


class Virtual_Memory: public Critical_Res
{
	friend void* operator new(U32 size);
	friend void operator delete(void* p);
	friend void Query();

	
private:
	
     struct MemBlock* UsrMem_Head; ///�����û��ɷ����ڴ�������׵�ַ
     struct MemBlock* SysMem_Head; ///����ϵͳ��ʹ���ڴ�������׵�ַ

private:
	
    static  bool  Con(struct MemBlock* p, S8 Dir); //�ж���������
	  U32   free_once(U8 Pri);


public:
     Virtual_Memory();
    ~Virtual_Memory();
   
     S32   setowner( U8 Prior );  //�ı���ڴ��ӵ����
     void * malloc(U32 size,U8 Prior = Virtual_Task::Cur_Prior);//����ϵͳδ����ǰ�����ڴ�
     void   free(void *p);

     void   free(  ); 

protected:
   
   void   Add(U32* pStartAddr,U32* pEndAddr);//�����ڴ�鵽����

};

/********************************************************************************/
#endif