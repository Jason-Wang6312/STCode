#include "STC12C5A.H"

typedef unsigned char u8;
typedef unsigned int u16;

sbit key = P1^4;
bit keysta = 1;
u8 speed = 50;
u8 cnt = 0;


void Timer0Init(void);
void Timer1Init(void);

void main(){
	EA = 1;
	Timer0Init();
	Timer1Init();
	
	while(1){
		P26 = P15;
		P25 = keysta;
	};
}

void scankey() {
	static u8 keybuf = 0xFF;
	
	keybuf = (keybuf << 1) | key;

	if(keybuf == 0x00){
		speed += 5;
	}
	if(speed >= 100){
		speed = 5;
	}
}

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}

void Timer1Init(void)		//10微秒@12.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0x88;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET0 = 1;
}


void T0() interrupt 1{
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	scankey();
	cnt = 0;
}

void T1() interrupt 3{
	TL1 = 0x88;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	cnt ++;
	if(cnt < speed)
		P20 = 1;
	else
		P20 = 0;
}

