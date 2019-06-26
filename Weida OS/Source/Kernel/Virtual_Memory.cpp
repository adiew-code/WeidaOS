#include"Virtual_Memory.h"


void* Virtual_Memory::malloc(U32 size,U8 Prior)
{
  struct MemBlock *p;
  S32 Len;
	U8 Last;
	
	 for(p = UsrMem_Head; p ; p = p->Next) ///遍历内存链表
        {
            if((p->Used << 24) == 0)///未使用的内存块
            {
							if( ( Len = (p->Block_Size - (sizeof(*p)+size)) ) >= 0 )//有足够的空间
							{
								
								Last = Len%4;//确保新分配的内存是4字节对齐
								
								if( Last == Len )///直接把这块内存给分出去
								{
									p->Used = ((Prior<<24) | 1);
									
								 return  ((U8*)p+sizeof(*p));
								}
								
                Len -= Last;								
							
							  ///下面的地址必须是4字节对齐，否则会有问题	
								struct MemBlock *pNew = (struct MemBlock *)((U8*)p + sizeof(*p) + Len ) ;
								
								pNew->Block_StartAddr = (U32*)pNew; 
								pNew->Block_Size      = p->Block_Size - Len - sizeof(*p);
							
								p->Block_Size         = Len;  //上一个的大小
								pNew->Used            = ((Prior<<24) | 1);
								
								///如果p不是最后一个
								if ( p->Next ) 
								{
									p->Next->Prev         = pNew;
								}
								
								 pNew->Prev            = p;
							   pNew->Next            = p->Next;
								 p->Next               = pNew;

								
							  return  ((U8*)pNew+sizeof(*p));
							}
	
						}
				}
 return p;///应该是返回0，只有是0时才会执行至此
}

bool  Virtual_Memory::Con(struct MemBlock* p, S8 Dir) //判断是相连？
{
	switch(Dir)
	{
		case 1:///跟下面比较
		   
          if(p->Next == 0)//没有下一个
            break;						
		
					if((p->Next->Used << 24) != 0)///下一个正在被使用
            break;

				  if((U32)p + sizeof(*p) + p->Block_Size != (U32)p->Next)//不相连着	
		        break;
					
					return true;
		case -1://跟上面比较
			
			    if(p->Prev == 0)//没有下一个
            break;						
		
					if((p->Prev->Used << 24) != 0)///下一个正在被使用
            break;

				  if((U32)p->Prev + sizeof(*p) + p->Prev->Block_Size != (U32)p)//不相连着	
		        break;
					
					return true;
		default:
	 	        break;
	}
	return false;
}


U32 Virtual_Memory::free_once(U8 Pri)
{
 struct MemBlock *p ;
	
	  for(p = UsrMem_Head ; p ; p = p->Next) ///遍历内存链表
	  {
			if(p->Used << 24 != 0 ) ///不空闲
			{
				if(( p->Used >> 24) == Pri)
				{
				   free(p); ////将该内存块释放
				   
					 return 1;
				}
			}
			
		}	
	return  0; //到了链表最后都没有找到需要释放的内存
}

void Virtual_Memory::free( )
{
	U8 Pri = Virtual_Task::Cur_Prior; //取当前优先级
	
	Enter_CriSec();
	
    while(free_once(Pri)) ; ///只要free_once函数返回非0就继续执行
	
	Exit_CriSec();
}

void Virtual_Memory::free(void *pMem)
{
	struct MemBlock *p = (struct MemBlock *)pMem;
	S8 State = 0;
/*************************************************
  State 表示的含义如下：
-1  :能和上面的相连
0   :不能和上面且也不能和下面
1   :能和下面相连
2   :能和下面且能和上面相连
*************************************************/
	if(Con(p,-1))
		State = -1;
	
	if(Con(p,1))
	{
		if( State == -1 )
		  State = 2;
		else
			State = 1;
	}
	
	switch(State)
	{
	case -1:
         p->Prev->Block_Size += (sizeof(*p) + p->Block_Size);
	
	       p->Prev->Next = p->Next;
	       
	       if(p->Next)
				 {
					 p->Next->Prev = p->Prev;
				 }
	      
	
          break;	
	case  0:
         p->Used = 0;
          break;			
	case  1:
         p->Block_Size += (sizeof(*p) + p->Next->Block_Size);
	 
	       p->Next = p->Next->Next;
	
	       if(p->Next)
				 {
					 p->Next->Prev = p;
					 
				 }
	
          break;	
	case  2:
        p->Prev->Block_Size += (p->Block_Size + p->Next->Block_Size + 2*sizeof(*p) );
	
	      p->Prev->Next = p->Next->Next;
	
	      if(p->Prev->Next)
				{
					p->Prev->Next->Prev = p->Prev;
				}
	
          break;	
	 default:///有错
		 
		      return;
	}
 
}


//增加内存块到链表
void Virtual_Memory::Add(U32* pStartAddr,U32* pEndAddr) //[pStartAddr , pEndAddr )范围内有效
{
	struct MemBlock *p = (struct MemBlock*)pStartAddr;
	S32 size =  ((U32)pEndAddr -  (U32)pStartAddr) ;
	
	try
	{
	   if(size < sizeof(*p))
       throw  "Too Short";			 
	 
	   if(pStartAddr != (U32*)(&p->Block_StartAddr))
			 throw "Error";
	}
	catch(int)
	{
		
	}
	p->Block_StartAddr = pStartAddr;
  p->Block_Size      = size - sizeof(*p);
  p->Next            = 0;
	p->Prev            = 0;
	p->Used            = 0;
	 
	
	///头插法
	if(UsrMem_Head)///如果不是第一次增加
	{
		UsrMem_Head->Prev = p;
		p->Next = UsrMem_Head;
		
	}
	
	UsrMem_Head = p; 
	
}

S32 Virtual_Memory::setowner(U8 NewPrior)
{
	Enter_CriSec();
	
	
	
	Exit_CriSec();
}


Virtual_Memory::Virtual_Memory(): Critical_Res(MEMORY_RESOURCE)
{
   SysMem_Head = UsrMem_Head = 0;//初始化为0
}

Virtual_Memory::~Virtual_Memory()
{
   
}