#ifndef  __LCD_H
#define  __LCD_H
/***********************************************************************************/
#include"Address.h"
#include"Virtual_LCD.h"

#define LCD_PIXCLOCK 4
#define LCD_RIGHT_MARGIN 0x44
#define LCD_LEFT_MARGIN 4
#define LCD_HSYNC_LEN 1

#define LCD_UPPER_MARGIN 10
#define LCD_LOWER_MARGIN 4
#define LCD_VSYNC_LEN 1
#define LCD_CON5 ((1<<11) | (1<<8) | (1<<9) | (1<<0) )
#define	M5D(n)	((n)&0x1fffff)

#define LCD_HEIGHT       (240)
#define LCD_WIDTH        (320)
#define LCD_BUFFER_BASE  (0x33800000)///ÏÔ´æÆðÊ¼µØÖ·
                        //0x33825800
#define LCD_BUFFER_END   ( LCD_BUFFER_BASE + LCD_WIDTH*LCD_HEIGHT*2 )///ÏÔ´æ½áÊøµØÖ·
                       


class LCD: public Virtual_LCD ///´Ó´¿ÐéÀà¼Ì³Ð
{


public:
    LCD();
    ~LCD();

protected:

   void PutPixel(int x, int y, int color);

   void Lcd_PowerEnable(int invpwren, int pwren);

   
};



/********************************************************************************/
#endif
