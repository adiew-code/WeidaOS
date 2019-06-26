#ifndef  __WEIDA_VIRTUAL_MEMORY_H
#define  __WEIDA_VIRTUAL_MEMORY_H
/***********************************************************************************
                                    内存管理
							 修改时间：2012/12/3   10:12 am
							 作者：weida
							 内容：将系统空闲内存进行管理，并且重载new预算符使用户可以
							        动态申请内存。
*************************************************************************************/
#include"DataTypes.h"
#include"Critical_Res.h"
#include"Virtual_Task.h"
#include "new"


struct MemBlock
{
	U32   * Block_StartAddr; ///内存块起始地址
	U32     Block_Size;      //大小，按字节计算
	struct MemBlock *Next;   //指向下一个内存块，0表示最后一个
	struct MemBlock *Prev;   ///指向上一个内存块，0表示第一个
	
	U32    Used;  //该内存块得使用情况，具体看下面：
/************************************************************
         Which?        Unused           How Much  
    --------------------------------------------
	  |31        24|23                8|7       0|
    --------------------------------------------
1、低八位表示当前内存块有多少个任务在使用（多个任务可以共享一块内存）
2、高八位表示当前哪个任务正在使用该内存
3、若低八位为0则表示该内存块正空闲，否则正在被使用
4、若最高八位为0xff则表示该内存由系统申请，不可被普通进程删除
*************************************************************/	
	
};


class Virtual_Memory: public Critical_Res
{
	friend void* operator new(U32 size);
	friend void operator delete(void* p);
	friend void Query();

	
private:
	
     struct MemBlock* UsrMem_Head; ///保存用户可分配内存块链表首地址
     struct MemBlock* SysMem_Head; ///保存系统已使用内存块链表首地址

private:
	
    static  bool  Con(struct MemBlock* p, S8 Dir); //判断是相连？
	  U32   free_once(U8 Pri);


public:
     Virtual_Memory();
    ~Virtual_Memory();
   
     S32   setowner( U8 Prior );  //改变该内存的拥有者
     void * malloc(U32 size,U8 Prior = Virtual_Task::Cur_Prior);//用于系统未运行前分配内存
     void   free(void *p);

     void   free(  ); 

protected:
   
   void   Add(U32* pStartAddr,U32* pEndAddr);//增加内存块到链表

};

/********************************************************************************/
#endif