#include"Virtual_Memory.h"
#include"WeidaOS.h"

/*******************************************************
                 重载全局new预算符
						参数：size 为欲分配的大小
						返回值：返回分配的空间首地址
*******************************************************/
void* operator new(U32 size) 
{ 
  WeidaOS *p; 
  Virtual_Memory *pMem;
	

	if(size == 0)//不需要分配，直接返回0
		return 0;
	
	if((p=p->Cur_OS) == 0)//无法找到当前系统指针，（大概是系统故障）
		return 0;
	
	if(p->pBoard == 0)//
		return 0;
	
	if((pMem = p->pBoard->pMem) == 0)
	  return 0;
	
	if(p->Run == false)
		return 0;
 
	
   pMem->Enter_CriSec();
	
     p =(WeidaOS*)pMem->malloc(size);
	
	pMem->Exit_CriSec();
	
	
 return  (void*)p;

}



void operator delete(void* p) 
{ 	
	WeidaOS *pOS; 
	Virtual_Memory *pMem;
	
	
  if( p == 0)
		return ;
	
  if((pOS=pOS->Cur_OS) == 0)
  	return ;
	
	if(pOS->pBoard == 0)//
		return ;
	
	if((pMem = pOS->pBoard->pMem) == 0)
	  return ;
	
	if(pOS->Run == false)
		return ;
	
	
	p = (void *)( (U32)p - sizeof(struct MemBlock) );

#define VP      ((struct MemBlock*)p)
//检测p指针时候会有错？
	if( (VP->Block_StartAddr) != ((U32*)&(VP->Block_StartAddr)) )//p指针可能有问题
		return;
	
	if( VP->Block_Size == 0 ) //p指针可能有问题
		return;
	
	if( (VP->Used << 24) == 0 )///该任务处于空闲，不需要删除
		return;
	
	if( (VP->Used >> 24) != Virtual_Task::Cur_Prior )///不属于该任务的动态内存，不能删除
		return;
	
#undef	VP
	
	
  pMem->Enter_CriSec();
	
     pMem->free(p);
	
	pMem->Exit_CriSec();
	

}

inline void operator delete[](void* p) 
{ 
  delete p;
}