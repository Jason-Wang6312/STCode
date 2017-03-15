#include "HC595.H"
//共阴极
unsigned char code LEDCode[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//共阳极
//unsigned char code LEDCode[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

//HC595 1us 延时
void HCDelay1us(){		//@12.000MHz
	_nop_();
	_nop_();
	_nop_();
	_nop_();
    _nop_();
    _nop_();
}

//HC595 输入
void HC595In(unsigned char dat){
	unsigned char i = 0;
    //LED display code
    unsigned char LedC = 0x00;
    
    LedC = LEDCode[dat];    
    SH_CP = 0;
	for(;i<8;i++){
		LedC <<= 1;
        DS = CY;
        SH_CP = 1;
        HCDelay1us();
        SH_CP = 0;
	}
}

//HC595 输出
void HC595Out(){
    ST_CP = 0;
    HCDelay1us();
    ST_CP = 1;
    HCDelay1us();
    ST_CP = 0;
}

void Delay100ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 5;
	j = 144;
	k = 71;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main(){
    unsigned char i=0,x=0,j=10;
    P1 = 0x00;
    P3 = 0x00;
    
    
    while(1){
        for(i=0;i < 10;i++){
            HC595In(i);
            HC595Out();
//            switch(x){
//                case 0:
//                    LED1 = 0;
//                    break;
//                case 1:
//                    LED2 = 0;
//                    break;
//                case 2:
//                    LED3 = 0;
//                    break;
//                case 3:
//                    LED4 = 0;
//                    break;
//            }
//            x++;
//            if(x > 3) x=0;
            while(j--){
                Delay100ms();
            }
            j = 10;
//            LED1 = LED2 = LED3 = LED4 = 1;
//            P36 = ~P36;
        }
    }
}



