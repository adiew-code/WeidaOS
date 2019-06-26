#ifndef  __WEIDA_VIRTUAL_LCD_H
#define  __WEIDA_VIRTUAL_LCD_H
/***********************************************************************************
			虚拟串口......
*************************************************************************************/
#include"DataTypes.h"

class Virtual_LCD
{
	
protected:
	
    U32  Width;
    U32  Height;
    U32 *pBuffer_Base;///显存基地址
    U32  Cur_Color; //当前颜色
    U32  Cur_Bkcor; //当前背景色
public:
	
    Virtual_LCD();
    ~Virtual_LCD();
    
    void Show_Hzk16(U32 x, U32 y,const U8 *HZ, U32 color , U32 bkcolor);
    void Circle(int x1, int y1, int r, int color); 
    void Show_Str(int x, int y, unsigned char *HZ, int color, int bkcolor);
    void Circlefill(int x1, int y1, int r, int color);
    void Clear(int color) ;
    void DrawLine(int x1, int y1, int x2, int y2, int color);
    void Rect(int x0, int y0, int x1, int y1, int color);
    void FilledRectangle(int x1, int y1, int x2, int y2, int color);
    void Paint_Bmp(int x0, int y0, int h, int l, const unsigned char *bmp);
    void LCD_Test();

protected:
	
	  virtual void PutPixel(int x, int y, int color)=0;
    virtual  void Lcd_PowerEnable(int invpwren, int pwren)=0;

};

/********************************************************************************/
#endif