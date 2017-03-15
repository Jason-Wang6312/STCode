//#include "STC15F2K.H"
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
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	BRT = 0xF3;		//设定独立波特率发生器重装值
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
    ES = 1;
    
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
