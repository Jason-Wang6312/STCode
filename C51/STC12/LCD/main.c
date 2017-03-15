#include "STC12C5A.H"

#define u8 unsigned char

extern void LCDInit();
extern void LCDShowStr(u8 x,u8 y,u8 *str);
extern void UartInit();
extern void sendStr(u8 *str);

u8 receive[15] = "MaWei";
u8 receiveNum = 0;
bit busy = 0;
bit show = 1;
u8 st[9] = "a";

void main(){
	
	EA = 1;
	LCDInit();
	UartInit();
	LCDShowStr(1,0,receive);
	
	while(1){
		if(show){
			st[receiveNum] = 'a';
			LCDShowStr(1,0,st);
		}
		P22 = show;
	};
}

void sendDat(u8 dat){
	while(busy);
	busy = 1;
	SBUF = dat;
}

void sendStr(u8 *str){
	while(*str){
		sendDat(*str++);
	}
}

void UartInit(){
//	PCON &= 0x7F;
//	AUXR |= 0x40;
//	AUXR &= 0xFE;
	SCON = 0x50;
	TMOD &= 0xF0;
	TMOD |= 0x20;
	TH1 = TL1 = 0xF3;
	ES = 1;
	TR1 = 1;
}

void UartInterrupt() interrupt 4{
	if(RI){
		RI = 0;
	}
	if(TI){
		TI = 0;
		busy = 0;
	}
}

