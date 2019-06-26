#include"Virtual_Memory.h"


void* Virtual_Memory::malloc(U32 size,U8 Prior)
{
  struct MemBlock *p;
  S32 Len;
	U8 Last;
	
	 for(p = UsrMem_Head; p ; p = p->Next) ///�����ڴ�����
        {
            if((p->Used << 24) == 0)///δʹ�õ��ڴ��
            {
							if( ( Len = (p->Block_Size - (sizeof(*p)+size)) ) >= 0 )//���㹻�Ŀռ�
							{
								
								Last = Len%4;//ȷ���·�����ڴ���4�ֽڶ���
								
								if( Last == Len )///ֱ�Ӱ�����ڴ���ֳ�ȥ
								{
									p->Used = ((Prior<<24) | 1);
									
								 return  ((U8*)p+sizeof(*p));
								}
								
                Len -= Last;								
							
							  ///����ĵ�ַ������4�ֽڶ��룬�����������	
								struct MemBlock *pNew = (struct MemBlock *)((U8*)p + sizeof(*p) + Len ) ;
								
								pNew->Block_StartAddr = (U32*)pNew; 
								pNew->Block_Size      = p->Block_Size - Len - sizeof(*p);
							
								p->Block_Size         = Len;  //��һ���Ĵ�С
								pNew->Used            = ((Prior<<24) | 1);
								
								///���p�������һ��
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
 return p;///Ӧ���Ƿ���0��ֻ����0ʱ�Ż�ִ������
}

bool  Virtual_Memory::Con(struct MemBlock* p, S8 Dir) //�ж���������
{
	switch(Dir)
	{
		case 1:///������Ƚ�
		   
          if(p->Next == 0)//û����һ��
            break;						
		
					if((p->Next->Used << 24) != 0)///��һ�����ڱ�ʹ��
            break;

				  if((U32)p + sizeof(*p) + p->Block_Size != (U32)p->Next)//��������	
		        break;
					
					return true;
		case -1://������Ƚ�
			
			    if(p->Prev == 0)//û����һ��
            break;						
		
					if((p->Prev->Used << 24) != 0)///��һ�����ڱ�ʹ��
            break;

				  if((U32)p->Prev + sizeof(*p) + p->Prev->Block_Size != (U32)p)//��������	
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
	
	  for(p = UsrMem_Head ; p ; p = p->Next) ///�����ڴ�����
	  {
			if(p->Used << 24 != 0 ) ///������
			{
				if(( p->Used >> 24) == Pri)
				{
				   free(p); ////�����ڴ���ͷ�
				   
					 return 1;
				}
			}
			
		}	
	return  0; //�����������û���ҵ���Ҫ�ͷŵ��ڴ�
}

void Virtual_Memory::free( )
{
	U8 Pri = Virtual_Task::Cur_Prior; //ȡ��ǰ���ȼ�
	
	Enter_CriSec();
	
    while(free_once(Pri)) ; ///ֻҪfree_once�������ط�0�ͼ���ִ��
	
	Exit_CriSec();
}

void Virtual_Memory::free(void *pMem)
{
	struct MemBlock *p = (struct MemBlock *)pMem;
	S8 State = 0;
/*************************************************
  State ��ʾ�ĺ������£�
-1  :�ܺ����������
0   :���ܺ�������Ҳ���ܺ�����
1   :�ܺ���������
2   :�ܺ��������ܺ���������
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
	 default:///�д�
		 
		      return;
	}
 
}


//�����ڴ�鵽����
void Virtual_Memory::Add(U32* pStartAddr,U32* pEndAddr) //[pStartAddr , pEndAddr )��Χ����Ч
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
	 
	
	///ͷ�巨
	if(UsrMem_Head)///������ǵ�һ������
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
   SysMem_Head = UsrMem_Head = 0;//��ʼ��Ϊ0
}

Virtual_Memory::~Virtual_Memory()
{
   
}