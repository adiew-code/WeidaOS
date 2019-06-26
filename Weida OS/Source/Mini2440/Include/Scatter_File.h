#ifndef  __SCATTER_FILE_H
#define  __SCATTER_FILE_H
/***********************************************************************************/
#ifdef Scatter_File  //��������˸ú꣬��ζ������������Scatter file�����ӳ���.	���·����������������ɵ�.

///ֻ��������C++����ʹ��
extern  int Image$$ExecutableAddress$$RO$$Base;   //���������ɵ�.
extern  int Image$$ExecutableAddress$$RO$$Limit ;  //; End of ROM code (=start of ROM data)

extern  int Image$$ReadWriteAddress$$RW$$Base;  //; Base of RAM to initialise
extern  int Image$$ReadWriteAddress$$RW$$Limit ;

extern int  Image$$ZeroInit$$ZI$$Base ;  //; Base and limit of area
extern  int Image$$ZeroInit$$ZI$$Limit ; ///; to zero initialise



#define Image_RO_Base  Image$$ExecutableAddress$$RO$$Base
#define Image_RO_Limit Image$$ExecutableAddress$$RO$$Limit

#define Image_RW_Base  Image$$ReadWriteAddress$$RW$$Base
#define Image_RW_Limit Image$$ReadWriteAddress$$RW$$Limit

#define Image_ZI_Base  Image$$ZeroInit$$ZI$$Base
#define Image_ZI_Limit Image$$ZeroInit$$ZI$$Limit

#else ///���� ���·����������������ɵ�. �����������Ϳ�����������  
extern int Image$$RO$$Base;	//; Base of ROM code
extern int Image$$RO$$Limit;

extern  int  Image$$RW$$Base ;
extern  int  Image$$RW$$Limit ;  //; Base of RAM to initialise

extern  int  Image$$ZI$$Base ;
extern  int  Image$$ZI$$Limit ;

#define Image_RO_Base  Image$$RO$$Base
#define Image_RO_Limit Image$$RO$$Limit

#define Image_RW_Base  Image$$RW$$Base
#define Image_RW_Limit Image$$RW$$Limit

#define Image_ZI_Base  Image$$ZI$$Base
#define Image_ZI_Limit Image$$ZI$$Limit

#endif

/********************************************************************************/
#endif