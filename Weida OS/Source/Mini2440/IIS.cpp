#include"DataTypes.h"
#include"Address.h"
#include"WeidaOS.h"


#define	AUDIO_PLAY_DEV	0x100
#define	AUDIO_REC_DEV	0x101

#define	DEVICE_REC	2
#define	DEVICE_PLAY	1
#define	DEVICE_FREE	0	
#define	PLAY	0
#define	RECORD	1

#define	PLAY_DMA_ATTR	((AUDIO_PLAY_DEV<<16)|SRC_LOC_AHB|SRC_ADDR_INC|DST_LOC_APB|DST_ADDR_FIXED|REQ_IISDO)
#define	PLAY_DMA_MODE	(HANDSHAKE_MODE|SYNC_APB|DONE_GEN_INT|TSZ_UNIT|SINGLE_SVC|HW_TRIG|RELOAD_OFF|DSZ_16b)///半字传输...

#define	REC_DMA_ATTR	((AUDIO_REC_DEV<<16)|SRC_LOC_APB|SRC_ADDR_FIXED|DST_LOC_AHB|DST_ADDR_INC|REQ_IISDI)
#define	REC_DMA_MODE	(HANDSHAKE_MODE|SYNC_APB|DONE_GEN_INT|TSZ_UNIT|SINGLE_SVC|HW_TRIG|RELOAD_OFF|DSZ_16b)



static struct{	
	U16 wFormatTag;
	U16 wBitsPerSample;
	U32 DevReq;	
	U8 FsIdx;
}PlayStatus , RecStatus;


static U32 save_MPLLCON;

static struct{
	U16 Freq;
	U32 MPLL;
	U16 PreScaler;
	U8	ofs;
}CodecPara[7] = 
     {
	      {8000,(123<<12)|(6<<4)|0,(23<<5)|23,0},
				{11025,(229<<12)|(5<<4)|1,(11<<5)|11,1},
				{16000,(123<<12)|(6<<4)|0,(11<<5)|11,0},
				{22050,(229<<12)|(5<<4)|1,(5<<5)|5,1},
				//{22050,(0x96<<12)|(5<<4)|1,(5<<5)|5,0},
				{32000,(123<<12)|(6<<4)|0,(5<<5)|5,0},
				{44100,(229<<12)|(5<<4)|1,(2<<5)|2,1},
				{48000,(123<<12)|(6<<4)|0,(3<<5)|3,0}
			};

			
			
#define L3C (1<<4)              //GPB4 = L3CLOCK
#define L3D (1<<3)              //GPB3 = L3DATA
#define L3M (1<<2)              //GPB2 = L3MODE

static void _WrL3Addr(U8 data)
{       
    S32 i,j;

    rGPBDAT  = rGPBDAT & ~(L3D | L3M | L3C) | L3C;      //L3D=L, L3M=L(in address mode), L3C=H

    for(j=0;j<4;j++);                   //tsu(L3) > 190ns

      //GPB[4:2]=L3C:L3D:L3M
    for(i=0;i<8;i++)                    //LSB first
    {
        if(data & 0x1)                  //If data's LSB is 'H'
        {
            rGPBDAT &= ~L3C;            //L3C=L
            rGPBDAT |= L3D;             //L3D=H             
           
            for(j=0;j<4;j++);           //tcy(L3) > 500ns
          
            rGPBDAT |= L3C;             //L3C=H
            rGPBDAT |= L3D;             //L3D=H
           
            for(j=0;j<4;j++);           //tcy(L3) > 500ns
        }
        else                            //If data's LSB is 'L'
        {
            rGPBDAT &= ~L3C;            //L3C=L
            rGPBDAT &= ~L3D;            //L3D=L
           
            for(j=0;j<4;j++);           //tcy(L3) > 500ns
           
            rGPBDAT |= L3C;             //L3C=H
            rGPBDAT &= ~L3D;            //L3D=L
            for(j=0;j<4;j++);           //tcy(L3) > 500ns            
        }
        data >>= 1;
    }
    rGPBDAT  = rGPBDAT & ~(L3D | L3M | L3C) | (L3C | L3M);       //L3M=H,L3C=H   
}

//==========================================================
static void _WrL3Data(U8 data,int halt)
{
    S32 i,j;

    if(halt)
    {
        rGPBDAT  = rGPBDAT & ~(L3D | L3M | L3C) | L3C;   //L3C=H(while tstp, L3 interface halt condition)        
        for(j=0;j<4;j++);                                //tstp(L3) > 190ns
    }

    rGPBDAT  = rGPBDAT & ~(L3D | L3M | L3C) | (L3C | L3M);   //L3M=H(in data transfer mode)        
    for(j=0;j<4;j++);                                        //tsu(L3)D > 190ns

      //GPB[4:2]=L3C:L3D:L3M
    for(i=0;i<8;i++)
    {
        if(data & 0x1)                   //if data's LSB is 'H'
        {
           rGPBDAT &= ~L3C;              //L3C=L
           rGPBDAT |= L3D;               //L3D=H
           
           for(j=0;j<4;j++);             //tcy(L3) > 500ns
           
           rGPBDAT |= (L3C | L3D);       //L3C=H,L3D=H
           for(j=0;j<4;j++);             //tcy(L3) > 500ns
        }
        else                             //If data's LSB is 'L'
        {
           rGPBDAT &= ~L3C;              //L3C=L
           rGPBDAT &= ~L3D;              //L3D=L
           for(j=0;j<4;j++);             //tcy(L3) > 500ns
          
           rGPBDAT |= L3C;               //L3C=H
           rGPBDAT &= ~L3D;              //L3D=L
           for(j=0;j<4;j++);             //tcy(L3) > 500ns
        }
        data >>= 1;                      //For check next bit
    }
    rGPBDAT  = rGPBDAT & ~(L3D | L3M | L3C) | (L3C | L3M);    //L3M=H,L3C=H
}



#define	MAX_VOLUME	61
static void AdjVolume(U8 volume)	
{	
	 rGPBDAT = rGPBDAT & ~(L3M|L3C|L3D) |(L3M|L3C); //Start condition : L3M=H, L3C=H
    rGPBUP  = rGPBUP  & ~(0x7<<2) |(0x7<<2);       //The pull up function is disabled GPB[4:2] 1 1100    
   	rGPBCON = rGPBCON & ~(0x3f<<4) |(0x15<<4);     //GPB[4:2]=Output(L3CLOCK):Output(L3DATA):Output(L3MODE)
	
	///volume = (volume*MAX_VOLUME)/0xffff;		    
	    
	  _WrL3Addr(0x14 + 0);				//DATA0 (000101xx+00)
   	_WrL3Data(61-volume, 0);
}




//==========================================================
static void Init1341_Play(void)
{
	U16 FsIdx;
//Port Initialize
//----------------------------------------------------------
//   PORT B GROUP
//Ports  :   GPB4    GPB3   GPB2  
//Signal :  L3CLOCK L3DATA L3MODE
//Setting:   OUTPUT OUTPUT OUTPUT 
//           [9:8]   [7:6}  [5:4]
//Binary :     01  ,   01    01 
//----------------------------------------------------------    
	rGPBDAT = rGPBDAT & ~(L3M|L3C|L3D) |(L3M|L3C); //Start condition : L3M=H, L3C=H
	rGPBUP  = rGPBUP  & ~(0x7<<2) |(0x7<<2);       //The pull up function is disabled GPB[4:2] 1 1100    
	rGPBCON = rGPBCON & ~(0x3f<<4) |(0x15<<4);     //GPB[4:2]=Output(L3CLOCK):Output(L3DATA):Output(L3MODE)
	
	//L3 Interface
	_WrL3Addr(0x14 + 2);	//STATUS (000101xx+10)
	_WrL3Data(0x60,0);		//0,1,10, 000,0 : Status 0,Reset,256fs,IIS-bus,no DC-filtering
	
	_WrL3Addr(0x14 + 2);	//STATUS (000101xx+10)
	
	FsIdx = PlayStatus.FsIdx;  ///判断是录音还是放音？ 
	
	if(CodecPara[FsIdx].ofs)
		_WrL3Data(0x18,0);	//0,0,01, 000,0 : Status 0,No reset,384fs,IIS-bus,no DC-filtering
	else
	 _WrL3Data(0x28,0);	//0,0,10, 000,0 : Status 0,No reset,256fs,IIS-bus,no DC-filtering
	
	_WrL3Addr(0x14 + 2);	//STATUS (000101xx+10)
	_WrL3Data(0xc1,0);		//1,0,0,0, 0,0,01 
							//Status 1,Gain of DAC 6 dB,Gain of ADC 0dB,ADC non-inverting,DAC non-inverting
							//,Single speed playback,ADC-Off DAC-On
    
}		
			

////使用正常传输模式，CPU轮询访问FIFO
void PlayMusic(const U8  *Wav, U32 Len)
{
	
	int nSamplesPerSec,wBitsPerSample;  
	unsigned char  i;
	
	
	U16 *p = (U16 *)Wav; ////一次只能写两个字节..

	nSamplesPerSec	= *(U32 *)((int)Wav+0x18);
	wBitsPerSample	= *(U16 *)((int)Wav+0x22);
	
	

	for(i=0; i<7; i++)
		if(nSamplesPerSec==CodecPara[i].Freq)
			break;

	PlayStatus.wBitsPerSample = wBitsPerSample;
	PlayStatus.FsIdx          = i;


	rCLKCON |= 0x20000;				//enable IIS clock	
	
		  rGPBCON = rGPBCON & ~(0x3f<<4) | (0x15<<4); //GPB[4:2]=Output(L3CLOCK):Output(L3DATA):Output(L3MODE)
      rGPECON = rGPECON & ~(0x3ff) | 0x2aa;   

	  Init1341_Play();
    
    ///再开IIS接口...
	rIISPSR  = CodecPara[PlayStatus.FsIdx].PreScaler;///IIS分频器..
	rIISCON  = (0<<5)+(0<<4)+(0<<3)+(1<<2)+(1<<1);	//Tx DMA enable[5], Rx idle[2], Prescaler enable[1]	
	rIISMOD  = (0<<8)+(2<<6)+(1<<5)+(1<<4)+((PlayStatus.wBitsPerSample>>4)<<3)+(CodecPara[PlayStatus.FsIdx].ofs<<2)+(1<<0);	
	rIISFCON = (0<<15) + (1<<13);    
  rIISCON |= 0x1;    	
	
	for(; (int)p<((int)Wav+Len) ? (p++) : 0;)
	  {
			
	     while((rIISCON&0x80)) ///判断FIFO寄存器缓冲是否空...    
			 {
          
			 }
			 
	     rIISFIFO=*p; ///写FIFO寄存器...
	  }
	
}

