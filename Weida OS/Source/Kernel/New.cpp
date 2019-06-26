#include"Virtual_Memory.h"
#include"WeidaOS.h"

/*******************************************************
                 ����ȫ��newԤ���
						������size Ϊ������Ĵ�С
						����ֵ�����ط���Ŀռ��׵�ַ
*******************************************************/
void* operator new(U32 size) 
{ 
  WeidaOS *p; 
  Virtual_Memory *pMem;
	

	if(size == 0)//����Ҫ���䣬ֱ�ӷ���0
		return 0;
	
	if((p=p->Cur_OS) == 0)//�޷��ҵ���ǰϵͳָ�룬�������ϵͳ���ϣ�
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
//���pָ��ʱ����д�
	if( (VP->Block_StartAddr) != ((U32*)&(VP->Block_StartAddr)) )//pָ�����������
		return;
	
	if( VP->Block_Size == 0 ) //pָ�����������
		return;
	
	if( (VP->Used << 24) == 0 )///�������ڿ��У�����Ҫɾ��
		return;
	
	if( (VP->Used >> 24) != Virtual_Task::Cur_Prior )///�����ڸ�����Ķ�̬�ڴ棬����ɾ��
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