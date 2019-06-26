#include"Memory.h"
#include"stdio.h"
#include"stdarg.h"
#include"Scatter_File.h"
#include"LCD.h"

#define  IMAGE_SIZE        (&Image_ZI_Limit - &Image_RO_Base) //Image是连续存放的
#define  DRAM_START        (0x30000000)
#define  DRAM_END          (0x34000000)
#define  DRAM_SIZE         (DRAM_END - DRAM_START)  ///64M大小

Memory::Memory( )
{
	///按照从大到小的顺序增加至链表中
///	Add( (U32*)LCD_BUFFER_END , DRAM_END - LCD_BUFFER_END );
	
  Add( (U32*)( &Image_ZI_Limit ) , (U32*)LCD_BUFFER_BASE );  ///将DRAM加入系统内存管理链表中
	
}
