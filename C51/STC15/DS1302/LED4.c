#include "reg52.h"

typedef unsigned char u8;
typedef unsigned int u16;

//共阳极
unsigned char code LEDCode[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

void Delay100ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 195;
	j = 138;
	do
	{
		while (--j);
	} while (--i);
}

void delayXms(u16 i){
    while(i--){
        Delay100ms(); 
    }
}

void main(){
    u8 i;
    
    P0 = 0x00;
    while(1){
        
        for(i=0;i<9;i++){
            P1 = LEDCode[i];
            delayXms(10);
        }
            
    }
}


