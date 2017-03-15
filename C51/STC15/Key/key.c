#include "STC15F2K.H"

typedef unsigned char u8;
typedef unsigned int u16;

sbit key = P3^3;
sbit motor = P3^2;
bit flag = 1;
bit keybackup = 1;
u8 pwm = 0;
u16 cnt = 0;

void Timer0Init(void);
void scankey();

void main(){
	EA = 1;
	Timer0Init();
	
	while(1){
		if(cnt < pwm){
			motor = 1;
			P12 = 0;
		}else{
			motor = 0;
			P12 = 1;
		}
		if(flag != keybackup){
			if(flag == 0){
				pwm += 10;
			}
			keybackup = flag;
		}
		if(cnt > 200){
			cnt = 0;
		}
		if(pwm > 180){
			pwm = 0;
		}
	};
}


//void Timer0Init(void){		//1毫秒@12.000MHz
//	AUXR |= 0x80;		//定时器时钟1T模式
//	TMOD &= 0xF0;		//设置定时器模式
//	TL0 = 0x20;		//设置定时初值
//	TH0 = 0xD1;		//设置定时初值
//	TF0 = 0;		//清除TF0标志
//	TR0 = 1;		//定时器0开始计时
//	ET0 = 1;
//}

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x20;		//设置定时初值
	T2H = 0xD1;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 = 0x04;
}

void scankey(){
	static u8 keybuf = 0xFF;
	
	keybuf = (keybuf << 1) | key;
	
	if(keybuf == 0x00){
		keybuf = 0xFF;
		flag = 1;
	}else if(keybuf == 0xff){
		flag = 0;
	}
}

void T0() interrupt 12{
	T2L = 0x20;		//设置定时初值
	T2H = 0xD1;		//设置定时初值
	cnt++;
	scankey();
}


