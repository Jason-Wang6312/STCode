#include "STC15F2K.H"

typedef unsigned char u8;
typedef unsigned int u16;

bit busy = 0;

void sendData(u8 dat);
void sendStr(u8 *str);
void UartInit();

void main() {
	EA = 1;
	
	UartInit();
	
	while(1){
		sendStr("MaWei\n");
	}
}

//void UartInit(void)		//9600bps@11.0592MHz
//{
//	SCON = 0x50;		//8???,?????
//	AUXR |= 0x40;		//???1???Fosc,?1T
//	AUXR &= 0xFE;		//??1?????1???????
//	TMOD &= 0x0F;		//?????1?16???????
//	TL1 = 0xE0;		//??????
//	TH1 = 0xFE;		//??????
//	ET1 = 0;		//?????1??
//	TR1 = 1;		//?????1
//	ES = 1;
//}


void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xE0;		//设定定时初值
	T2H = 0xFE;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	ES = 1;
}

void sendData(u8 dat){
	while(busy);
	busy = 1;
	SBUF = dat;
}

void sendStr(u8 *str){
	while(*str){
		sendData(*str++);
	}
}

void Uart() interrupt 4{
	if(RI){
		RI = 0;
	}
	
	if(TI){
		TI = 0;
		busy = 0;
	}
}
