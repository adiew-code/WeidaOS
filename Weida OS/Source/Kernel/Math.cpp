#include"Math.h"


U32 Rand::random( )
{
	
	return Next = Next * 1103515245 + 12345;	 
				
}
	   
U32 Rand::random(U32  Max)
{
	
	return random( )%Max; 
				
}

void Rand::srandom(U32 seed)   
{

	Next = seed;
	
}
				 