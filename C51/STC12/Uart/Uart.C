#include "STC12C5A.H"

typedef unsigned char u8;
typedef unsigned int u16;

bit busy = 0;

void UartInit();
void SendStr(u8 *str);

void main(){
	EA = 1;
	UartInit();
	
	while(1){
		SendStr("Test");
	}
}

void UartInit(void){		//4800bps@12.000MHz
	PCON |= 0x80;
	SCON |= 0x52;
	AUXR &= 0xFE;
	TL1 = TH1 = 0xF3;
	TMOD &= 0x0F;
	TMOD |= 0x20;
	ES = 1;
	TR1 = 1;
	ET0 = 0;
}

void SendData(u8 dat){
	while(busy);
	busy = 1;
	SBUF = dat;
}

void SendStr(u8 *str){
	while(*str){
		SendData(*str++);
	}
}

void UartIT() interrupt 4{
	if(RI){
		RI = 0;
	}
	if(TI){
		busy = 0;
		TI = 0;
	}
}

//void UartInit(){
//	AUXR &= 0xF7;
//	S2CON = 0x50;
//	AUXR |= 0x14;
//	BRT = 0xF3;
//	IE2 = 1;
//}

//void SendData(u8 dat){
//	while(busy);
//	busy = 1;
//	S2BUF = dat;
//}

//void SendStr(u8 *str){
//	while(*str){
//		SendData(*str++);
//	}
//}

//void UartIs() interrupt 8 {
//	if(S2CON & S2RI){
//		S2CON &= ~S2RI;
//	}
//	if(S2CON & S2TI){
//		S2CON &= ~S2TI;
//		busy = 0;
//	}
//}
