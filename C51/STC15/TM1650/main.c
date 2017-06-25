#include "STC15Wxx.h"
#include "INTRINS.H"
#include "tm16xx.h"

typedef unsigned char u8;
typedef unsigned int u16;

/**************共阴数码管显示0-F**********************/
u8 display[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71}; //共阴极字段码

u16 sec = 0;

void delayXms(u16 x);

void main(){
    delayXms(500);
    TMxxShow(0x48,0x11);
    TMxxShow(0x68,display[2]);
    TMxxShow(0x6A,display[9]);
    TMxxShow(0x6C,display[7]);
    TMxxShow(0x6E,display[3]);
    
    while(1){
        
    };
}

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1;
}

void T0() interrupt 1{
    sec++;
}

//延时x ms
void delayXms(u16 x){
    unsigned char i, j;
    while(x--){
        i = 12;
        j = 169;
        do
        {
            while (--j);
        } while (--i);
    }
}

