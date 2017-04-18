#include "STC12C5A.H"
#include "ds1302.h"

sbit K1=P1^4;
sbit K2=P1^5;
sbit K3=P1^6;
sbit K4=P1^7;
sbit K5=P3^5;
sbit LED1=P1^0;
sbit LED2=P1^1;
sbit LED3=P1^2;
sbit LED4=P1^3;

typedef unsigned char u8;
typedef unsigned int u16;

void Timer0Init();

u8 Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};
u8 sec;
u8 ledstr[4] = {0x00,0x00,0x00,0x00};
//struct sTime rtime; 
u8 ledflash = 6;
u8 ledTime;
u8 ledIndex;
u8 flashsec;
bit flash;
u8 rtime[8];

void main(){
    u8 second,minute;
    
    EA = 1;
    
    Timer0Init();
    InitDS1302();
    
    while(1){
        if(sec > 200){
            sec = 200;
            
//            second = DS1302SingleRead(0x81);
//            minute = DS1302SingleRead(0x83);
//            ledstr[0] = second & 0x0F;
//            ledstr[1] = second >> 4;
//            ledstr[2] = minute & 0x0F;
//            ledstr[3] = minute >> 4;
            
            DS1302BurstRead(rtime);
            ledstr[0] = rtime[0] & 0x0F;
            ledstr[1] = rtime[0] >> 4;
            ledstr[2] = rtime[1] & 0x0F;
            ledstr[3] = rtime[1] >> 4;
            ledstr[0] += 1;
        }
    }
}

void ledshow(u8 dat,u8 index){
    LED1 = LED2 = LED3 = LED4 = 0;
    P0 = dat;
    switch(index){
        case 0:
            LED1 = 1;
            break;
        case 1:
            LED2 = 1;
            break;
        case 2:
            LED3 = 1;
            break;
        case 3:
            LED4 = 1;
            break;
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

void T0() interrupt 1{
    TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
    
    sec++;
    ledTime++;
    if(ledTime > 3){
        ledTime=0;
        if(ledIndex == ledflash){
            flashsec++;
            if(flashsec > 15){
                flashsec = 0;
                flash = !flash;
            }
            if(flash){                
                ledshow(Disp_Tab[ledstr[ledIndex]],ledIndex);
            }else{
                LED1 = LED2 = LED3 = LED4 = 0;
            }
            
        }else
            ledshow(Disp_Tab[ledstr[ledIndex]],ledIndex);
        ledIndex++;
        ledIndex &= 0x03;
    }
}
