#ifndef  __MATH__H
#define  __MATH__H
/***********************************************************************************/

#include"DataTypes.h"




class  Rand
{
      U32 Next;
	
public:

/**************伪随机函数************************************************************/
      U32 random( );  ///返回一个随机数
      U32 random(U32 Max);//返回一个0—Max的随机数
      void srandom(U32 seed);
      


};

/********************************************************************************/
#endif