#include "STC12C5A.H"
#include "STDIO.H"

typedef unsigned char u8;
typedef unsigned int u16;

void UartInit(void){		//4800bps@12.000MHz
	PCON |= 0x80;
	SCON |= 0x52;
	AUXR &= 0xFE;
	TL1 = TH1 = 0xF3;
	TMOD &= 0x0F;
	TMOD |= 0x20;
	ES = 1;
	TR1 = 1;
	ET1 = 0;
}

void main(){
	UartInit();
	while(1){
		printf("MaWei\n");
	}
}
