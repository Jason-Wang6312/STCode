#include "STC12C5A.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 data LEDCode[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
u8 LEDShow[4] = {0x00,0x00,0x00,0x00};

u8 cnt=0,i=0;
u16 send = 0;

void time0Init(){ //5ms@12M
	AUXR |= 0x80;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0 = 0x15;
	TL0 = 0xA0;
	TR0 = 1;
	ET0 = 1;
}

void scanLed(){
	P0 = 0x00;
	cnt++;
	if(cnt >= 200){
		cnt = 0;
		send++;
		LEDShow[0] = send%10;
		LEDShow[1] = send/10%10;
		LEDShow[2] = send/100%10;
		LEDShow[3] = send/1000%10;
	}
	P0 = LEDCode[LEDShow[i]];
	P1 = 1 << i;
	i++;
	i &= 0x03;
}

void main(){
	u8 u;
	u = (567%10);
	
	EA = 1;
	time0Init();
	while(1);
}

void tr0() interrupt 1{
	TH0 = 0x15;
	TL0 = 0xA0;
	scanLed();
}
