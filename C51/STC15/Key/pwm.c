#include "STC15F2K.H"

typedef unsigned char u8;
typedef unsigned int u16;

sbit key = P3^3;
u16 cnt = 0;
u8 speed = 0;

void Timer0Init(void);
void Timer2Init(void);

void main(){
	EA = 1;
	Timer0Init();
	Timer2Init();
	
	while(1){
		if(cnt > 500){
			P11 = !P11;
			cnt++;
		}
	}
}

void scankey(){
	static u8 keybuf = 0xFF;
	
	keybuf = (keybuf << 1) | key;
	if(keybuf == 0x00){
		P12 = 0;
		speed += 5;
	}else{
		P12 = 1;
	}
}

void Timer0Init(void){		//1毫秒@12.000MHz
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
}

void Timer2Init(void)		//10微秒@12.000MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x88;		//设置定时初值
	T2H = 0xFF;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 = 0x04;
}


void T0() interrupt 1{
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	scankey();
}

void T2() interrupt 12{
	T2L = 0x88;		//设置定时初值
	T2H = 0xFF;		//设置定时初值
	cnt++;
}

