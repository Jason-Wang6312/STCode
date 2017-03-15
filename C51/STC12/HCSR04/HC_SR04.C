#include "HC_SR04.H"

u8 flag = 0;
u8 disCnt = 0;
u16 dist = 0;
u8 code ASCII[15] =    {'0','1','2','3','4','5','6','7','8','9','.','-','M'};
u8 distr[4]; 

u16 getDistance(){
    TL0 = TH0 = 0;
    disCnt = 0;
    
    Trig = 1;
    delay20us();
    Trig = 0;
    while(!Echo);
    TR0 = 1;
    while(Echo){
        TR0 = 0;
    }
    if(disCnt > 2){
    }
    dist = (TH0 * 256 + TL0) + (disCnt * 4000);
    dist /= 58;
    
    distr[0] = dist/100+'0';
    distr[1] = '.';
    distr[2] = dist%100/10+'0';
    distr[3] = dist%10%10+'0';
    sendStr(distr);
    return dist;
}

void Timer0Init(void){		//4毫秒@12.000MHz
    Trig = 0;
    Echo = 0;
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x80;		//设置定时初值
	TH0 = 0x44;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时
    IT0 = 1;
    EX0 = 0;
    ET0 = 1;
}

void T0() interrupt 1{
    TL0 = 0x80;		//设置定时初值
	TH0 = 0x44;		//设置定时初值
    disCnt++;
}

void delay20us(){
	u8 i = 57;
	while(--i);
}

