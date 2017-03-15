#include "config.h"
#include "lcd1602.h"
#include "ds18b20.h"
#include "ds1302.h"

struct sTime {  //日期时间结构体定义
    unsigned int  year;
    unsigned char mon;
    unsigned char day;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    unsigned char week;
};

void Time0Init();
void TempDisplay();


u16 cnt=0;

void main(){
    u8 tempstr[] = "T:00.00";
    
    //初始化LCD1602
    InitLcd1602();
    //初始化DS1302
    InitDS1302();
    //初始化定时器0
    Time0Init();
    
    
    LcdShowStr(8,1,tempstr);
    
    while(1){
        if(cnt > 1000){
			cnt = 0;
			TempDisplay();
//			distance();
		}
    }
}

//时钟显示
void TimeDisplay(){
    
}

//温度显示
void TempDisplay(){
	unsigned int temp;
	unsigned char datas[] = "00.00";
		
	temp = getTemp();
	temp = temp * 0.0625*100+0.5;
	
	datas[0] = (temp % 10000 / 1000) + '0';
	datas[1] = (temp % 1000 / 100) + '0';
	datas[3] = (temp % 100 / 10) + '0';
	datas[4] = (temp % 10) + '0';
	
	LcdShowStr(10,1,datas);
}
//定时器0初始化配置
void Time0Init(){
	AUXR |= 0x40;
	TMOD &= 0x0F;
	TMOD |= 0x10;
	TL1 = 0x20;
	TH1 = 0xD1;
	TF1 = 0;
	TR1 = 1;
	ET1 = 1; 
}
//定时器0中断
void time0() interrupt 3{
	TL1 = 0x20;
	TH1 = 0xD1;
	cnt++;
}

//延时1ms
void Delay1ms(){		//@12.000MHz
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 12;
	j = 168;
	do
	{
		while (--j);
	} while (--i);
}
//延时xms
void DelayXms(u16 n){
    while(n--){
        Delay1ms();
    }
}

//延时1us
void Delay1us(){		//@12.000MHz
	_nop_();
	_nop_();
}

//延时xus
void DelayXus(u16 n){
    while(n--){
        Delay1us();
    }
}
