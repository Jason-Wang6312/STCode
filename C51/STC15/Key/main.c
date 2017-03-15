#include "STC15F2K.H"

typedef unsigned char u8;
typedef unsigned int u16;

bit busy = 0;
bit keysta = 0;
sbit key1 = P3^3;
u8 keybuf = 0xFF;

void UartInit();
void sendData(u8 dat);
void sendStr(u8 *str);
void Timer0Init();

void main(){
	u8 str[] = "MaWei";
	
	EA = 1;
	UartInit();
	Timer0Init();
	
	sendStr(str);
	while(1){		
		P30 = P31;
	}
}

void UartInit()		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xE0;		//设定定时初值
	T2H = 0xFE;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	ES = 1;
}

void Timer0Init()		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

void scankey(){
	keybuf <<= 1;
	keybuf |= key1;
	
	if(keybuf == 0x00){
		P31 = !P31;
		sendData(P31);
	}
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

void T0() interrupt 1{
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	scankey();
}


