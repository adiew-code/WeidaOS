#include"LCD.h"


LCD::LCD()
{
    rGPGUP = rGPGUP & (~(1 << 4)) | (1 << 4); // Pull-up disable
    rGPGCON = rGPGCON & (~(3 << 8)) | (3 << 8); //GPG4=LCD_PWREN
    rGPGDAT = rGPGDAT | (1 << 4) ;


    Width = 320;
    Height = 240;
	  pBuffer_Base = (U32*)LCD_BUFFER_BASE;

    rLCDCON1 = (LCD_PIXCLOCK << 8) | (3 <<  5) | (12 << 1);
    rLCDCON2 = (LCD_UPPER_MARGIN << 24) | ((Height - 1) << 14) | (LCD_LOWER_MARGIN << 6) | (LCD_VSYNC_LEN << 0);

    rLCDCON3 = (LCD_RIGHT_MARGIN << 19) | ((Width  - 1) <<  8) | (LCD_LEFT_MARGIN << 0);
    rLCDCON4 = (13 <<  8) | (LCD_HSYNC_LEN << 0);

    rLCDCON5   =  LCD_CON5;
    rLCDSADDR1 = ((LCD_BUFFER_BASE >> 22) << 21) | ((M5D(LCD_BUFFER_BASE >> 1)) <<  0);

    rLCDSADDR2 = M5D((LCD_BUFFER_BASE + Width * Height * 2) >> 1);
    rLCDSADDR3 = Width;

    rLCDINTMSK |= 3;
    rTCONSEL   &= (~7);

    rGPCCON = 0xaaaaaaaaU;
    rGPCUP  = 0xffffU;
    rTPAL     = 0x0;
    rTCONSEL &= ~((1 << 4) | 1);
    rLCDCON1  |= 1;

    rGPGCON = 1 << 8;
    rGPGDAT = 1 << 4;

    rGPCUP  = 0x00000000;
    rGPCCON = 0xaaaa02a9;

    rGPDUP  = 0x00000000;
    rGPDCON = 0xaaaaaaaa; //Initialize VD[15:8]



    Lcd_PowerEnable(0, 1);
    rLCDCON1 |= 1; // ENVID=ON

   // Paint_Bmp(0, 0, 320, 240, Beautiful_Girl);
   
    //Show_Str(0,0,"\nLCD已经成功初始化了....\n", 0,0xffffffff);
    //// Show_Str(0,16,Hz2, 0,0xffffffff);
}

LCD::~LCD()
{
	
}



void  LCD::Lcd_PowerEnable(int invpwren, int pwren)
{
    rLCDCON5 = rLCDCON5 & (~(1 << 3)) | (pwren << 3); // PWREN
    rLCDCON5 = rLCDCON5 & (~(1 << 5)) | (invpwren << 5); // INVPWREN
}




void LCD::PutPixel(int x, int y, int color)
{
    unsigned Red, Green, Blue;

    if(x < 0)x = 0;
    if(x >= Width)x = Width - 1;
    if(y < 0)y = 0;
    if(y >= Height)y = Height - 1;

    Red = ( (color >> 16) & 0xff ) >> 3;
    Green = ( (color >> 8) & 0xff ) >> 2;
    Blue = ( color & 0xff) >> 3;
    color = (Red << 11) | (Green << 5) | (Blue);
	
    *(((short *)LCD_BUFFER_BASE) + (y * Width + x)) = (short)color;
}

