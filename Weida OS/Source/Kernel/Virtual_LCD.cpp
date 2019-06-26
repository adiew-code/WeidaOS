#include"Virtual_LCD.h"
#include"Virtual_Task.h"
//#include"PictureData\Beautiful_Girl.cpp"
//#include"../PictureData\Sex.cpp"
//#include"../HZK16.cpp"


//U8 *Sex=0;
const U8 *HZK16=0;



Virtual_LCD::Virtual_LCD()
{
    Width = 0;
    Height = 0;
	  pBuffer_Base=0;
	  Cur_Bkcor=Cur_Color=0;
}

Virtual_LCD::~Virtual_LCD()
{


}


void Virtual_LCD::Show_Hzk16(U32 x, U32 y,const U8 *HZ, U32 color , U32 bkcolor)
{
    U8 qh, wh;
    U32 offset;
    S32 i, j, k;
    qh = (*HZ) - 0xa0;	//获得区码
    wh =  *(HZ + 1) - 0xa0;	//获得位码
    offset = (94 * (qh - 1) + (wh - 1)) * 32; //得到偏移位置

    for(i = 0; i < 16; i++)
    {
        for(j = 0; j < 2; j++)
        {
            for(k = 0; k < 8; k++)
            {
                if(  *(HZK16+offset+i*2+j) & (0x80 >> k))
                {
                    //测试为1的位则显示
                    PutPixel( x + j * 8 + k, y + i, color);
                }
                else
                {
                    PutPixel( x+j*8+k, y+i, bkcolor);
                }
            }
        }
    }
}

void Virtual_LCD::Show_Str(int x, int y, unsigned char *HZ, int color, int bkcolor)
{
    int i = 0;
    for(; *HZ ; HZ++)
    {
        if(*HZ > 0xa0)//第一个字节是汉字的第一个字节
        {
            if(*(HZ + 1) > 0xa0)
            {
                Show_Hzk16( x + i * 13 + 2, y,  HZ,  color , bkcolor);
                HZ++;
                i++;
            }
            else
            {
                HZ++;
            }
        }
    }

}




/// 画圆函数
void Virtual_LCD::Circle(int x1, int y1, int r, int color)
{
    int   x, y, d;
    x = 0;
    y = r;
    d = 1 - r;                  /// 计算初始值                                          

    while (x <= y)
    {
        /// 绘制点(x,y)及其在八分圆中的另外7个对称点             
        PutPixel(x1 + x, y1 + y, color);
        PutPixel(x1 + y, y1 + x, color);
        PutPixel(x1 - y, y1 + x, color);
        PutPixel(x1 - x, y1 + y, color);
        PutPixel(x1 - x, y1 - y, color);
        PutPixel(x1 - y, y1 - x, color);
        PutPixel(x1 + y, y1 - x, color);
        PutPixel(x1 + x, y1 - y, color);

        if (d < 0)
        {
            d += 2 * x + 3;     /// 根据误差项d的判断,决定非最大位移方向上是走还是不走   
        }
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}


//// 画实心圆函数
void Virtual_LCD::Circlefill(int x1, int y1, int r, int color)
{
    int  x, y, d;
    x = 0;
    y = r;
    d = 1 - r;                  // 计算初始值                                          

    while (x <= y)
    {
        /// 绘制点(x,y)及其在八分圆中的另外7个对称点             
        DrawLine(x1 + x, y1 + y, x1 - x, y1 + y, color);
        DrawLine(x1 + x, y1 - y, x1 - x, y1 - y, color);
        DrawLine(x1 + y, y1 + x, x1 - y, y1 + x, color);
        DrawLine(x1 + y, y1 - x, x1 - y, y1 - x, color);


        if (d < 0)
        {
            d += 2 * x + 3;     // 根据误差项d的判断,决定非最大位移方向上是走还是不走   
        }
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}





void Virtual_LCD::Clear(int color)
{
	  unsigned Red, Green, Blue;
    int x = 0, y = 0;

    Red = ( (color >> 16) & 0xff ) >> 3;
    Green = ( (color >> 8) & 0xff ) >> 2;
    Blue = ( color & 0xff) >> 3;
    color = (Red << 11) | (Green << 5) | (Blue);

    for(; y < Height; y++)
        for(x = 0; x < Width; x++)
            *(((short *)pBuffer_Base) + (y * Width + x)) = (short)color;
}

void Virtual_LCD::DrawLine(int x1, int y1, int x2, int y2, int color)
{
    int dx, dy, e;
    dx = x2 - x1;
    dy = y2 - y1;

    if(dx >= 0)
    {
        if(dy >= 0) // dy>=0
        {
            if(dx >= dy) // 1/8 octant
            {
                e = dy - dx / 2;
                while(x1 <= x2)
                {
                    PutPixel(x1, y1, color);
                    if(e > 0)
                    {
                        y1 += 1;
                        e -= dx;
                    }
                    x1 += 1;
                    e += dy;
                }
            }
            else		// 2/8 octant
            {
                e = dx - dy / 2;
                while(y1 <= y2)
                {
                    PutPixel(x1, y1, color);
                    if(e > 0)
                    {
                        x1 += 1;
                        e -= dy;
                    }
                    y1 += 1;
                    e += dx;
                }
            }
        }
        else		   // dy<0
        {
            dy = -dy; // dy=abs(dy)

            if(dx >= dy) // 8/8 octant
            {
                e = dy - dx / 2;
                while(x1 <= x2)
                {
                    PutPixel(x1, y1, color);
                    if(e > 0)
                    {
                        y1 -= 1;
                        e -= dx;
                    }
                    x1 += 1;
                    e += dy;
                }
            }
            else		// 7/8 octant
            {
                e = dx - dy / 2;
                while(y1 >= y2)
                {
                    PutPixel(x1, y1, color);
                    if(e > 0)
                    {
                        x1 += 1;
                        e -= dy;
                    }
                    y1 -= 1;
                    e += dx;
                }
            }
        }
    }
    else //dx<0
    {
        dx = -dx;		//dx=abs(dx)
        if(dy >= 0) // dy>=0
        {
            if(dx >= dy) // 4/8 octant
            {
                e = dy - dx / 2;
                while(x1 >= x2)
                {
                    PutPixel(x1, y1, color);
                    if(e > 0)
                    {
                        y1 += 1;
                        e -= dx;
                    }
                    x1 -= 1;
                    e += dy;
                }
            }
            else		// 3/8 octant
            {
                e = dx - dy / 2;
                while(y1 <= y2)
                {
                    PutPixel(x1, y1, color);
                    if(e > 0)
                    {
                        x1 -= 1;
                        e -= dy;
                    }
                    y1 += 1;
                    e += dx;
                }
            }
        }
        else		   // dy<0
        {
            dy = -dy; // dy=abs(dy)

            if(dx >= dy) // 5/8 octant
            {
                e = dy - dx / 2;
                while(x1 >= x2)
                {
                    PutPixel(x1, y1, color);
                    if(e > 0)
                    {
                        y1 -= 1;
                        e -= dx;
                    }
                    x1 -= 1;
                    e += dy;
                }
            }
            else		// 6/8 octant
            {
                e = dx - dy / 2;
                while(y1 >= y2)
                {
                    PutPixel(x1, y1, color);
                    if(e > 0)
                    {
                        x1 -= 1;
                        e -= dy;
                    }
                    y1 -= 1;
                    e += dx;
                }
            }
        }
    }
}


/// 画矩形函数
void Virtual_LCD::Rect(int x0, int y0, int x1, int y1, int color)
{
    int   i;

    if (x0 > x1)
    {
        i = x0;
        x0 = x1;
        x1 = i;
    }
    if (y0 > y1)
    {
        i = y0;
        y0 = y1;
        y1 = i;
    }

    for (i = x0; i < x1 + 1; i++)
    {
        PutPixel(i, y0, color);
        PutPixel(i, y1, color);
    }

    for (i = y0; i < y1 + 1; i++)
    {
        PutPixel(x0, i, color);
        PutPixel(x1, i, color);
    }
}


/**************************************************************
在LCD屏幕上用颜色填充一个矩形
**************************************************************/
void Virtual_LCD::FilledRectangle(int x1, int y1, int x2, int y2, int color)
{
    int i, j;

    for(j = 0; j <= y2 - y1 ; j++)
        for(i = 0; i <= x2 - x1; i++)
            PutPixel(x1 + i, y1 + j, color);

}


/**************************************************************
在LCD屏幕上指定坐标点画一个指定大小的图片
**************************************************************/
void Virtual_LCD::Paint_Bmp(int x0, int y0, int h, int l, const unsigned char *bmp)
{
    int x, y;
    short c;
    int p = 0;

    for( y = 0 ; y < l ; y++ )
        for( x = 0 ; x < h ; x++ )
        {
            c = bmp[p+1] | (bmp[p] << 8) ;

            if ( ( (x0 + x) < Width) && ( (y0 + y) < Height) )
						   *(((short *)pBuffer_Base) + ((y0 + y)*Width + (x0 + x))) = (short)c;

            p += 2 ;
        }

}



void  Virtual_LCD::LCD_Test()
{
    int X_Head = 0, Y_Head = 0;
    int x, y;
    int x1, y1;
    static  int i=0;
    /**************************************************************
    下面的程序在LCD屏幕上，以蛇形环绕LCD，直至完全覆盖！
    *****************************************************************/
    for(; Y_Head <= 110; Y_Head += 10 , X_Head += 10) ////主循环
    {

        for(x = 0; x < ( 32 - 2 * (X_Head / 10) ); x++) ///=================>>
         {
					 FilledRectangle(X_Head + x * 10, Y_Head, X_Head + x * 10 + 10, Y_Head + 10, 8989 );
					 Delay(1);
				 }

        for(y = 0; y < (22 - Y_Head / 5); y++)
         {
					 FilledRectangle( X_Head + (x - 1) * 10, Y_Head + 10 + y * 10,  X_Head + x * 10, Y_Head + 20 + y * 10 , 7844 );
					 Delay(10);

				 }
        for(x1 = 0; x1 < ( 32 - 2 * (X_Head / 10) ); x1++)
				 { 
					 FilledRectangle( X_Head + (x - 1) * 10 - x1 * 10, Y_Head + 10 + y * 10 , X_Head + (x - 1) * 10 - x1 * 10 + 10, Y_Head + 20 + y * 10 , 0xfd );
					 Delay(5);
				 }

        for(y1 = 0; y1 < (22 - Y_Head / 5); y1++)
          {
						FilledRectangle( X_Head + (x - 1) * 10 - (x1 - 1) * 10, Y_Head + 10 + (y - 1) * 10 - y1 * 10 , X_Head + (x - 1) * 10 - (x1 - 1) * 10 + 10, Y_Head + 10 + (y - 1) * 10 + 10 - y1 * 10 , 20);
						Delay(3);
					}
    }
     
		//Paint_Bmp(0,0,320,240,Sex); 
    Clear(i++);
}






