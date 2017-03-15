#include "STC15F2K.H"

typedef unsigned char u8;
typedef unsigned int u16;

u16 cnt=0;

void Timer2Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x20;		//设置定时初值
	T2H = 0xD1;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 = 0x04;
}

void T2() interrupt 12{
	T2L = 0x20;		//设置定时初值
	T2H = 0xD1;		//设置定时初值
	cnt++;
}

void main(){
	EA = 1;
	Timer2Init();
	
	while(1){
		if(cnt > 200){
			P11 = !P11;
			cnt = 0;
		}
	}
}
