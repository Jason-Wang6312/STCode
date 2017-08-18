#include "STC15Wxx.h"
#include "INTRINS.H"

sbit DHT11IO = P0^5;

typedef unsigned char u8;
typedef unsigned int u16;

u8 DumidTemp[5];
u16 sec;

void UartInit();
void Timer0Init();
bit DHT11Start();
bit DHT11ReadData();
void sendData(unsigned char dat);

void main(){
    u8 i;
    EA = 1;
    
    UartInit();
    Timer0Init();

    while(1){
        if(sec > 2000){
            sec = 0;
            DHT11Start();
            DHT11ReadData();
            sendData(DumidTemp[0]);
            sendData(DumidTemp[1]);
            sendData(DumidTemp[2]);
            sendData(DumidTemp[3]);
            sendData(DumidTemp[4]);
            for(i = 0;i<5;i++){
                DumidTemp[i] = 0x00;
            }
        }
    }
}

//初始化
void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x8F;		//设定定时初值
	TH1 = 0xFD;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}

void sendData(unsigned char dat){
    SBUF = dat;
    while(!TI);
    TI = 0;
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


void DelayXus(unsigned int x)		//@12.000MHz
{
    while(x--){
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

void DelayXms(unsigned int x)		//@12.000MHz
{
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

bit DHT11Start(){
    unsigned char xus;
    
    DHT11IO = 0;
    DelayXms(20); //T1 (>18ms)
    DHT11IO = 1;
    DelayXus(30);
    
    while(DHT11IO && (xus < 100)){ //T2 (40~50us) 等等T2拉低
        DelayXus(1);
        xus++;
    }
    if(xus > 100) return 0;
    xus = 0;
    while(!DHT11IO && xus < 100){ //T3 (40~50us) 等等T3拉高
        DelayXus(1);
        xus++;
    }
    if(xus > 100) return 0;
    return 1;
}

bit DHT11ReadData(){
    unsigned char i,j,xus;
    
    for(i=0;i<5;i++){
        for(j=0;j<8;j++){
            while(DHT11IO && xus < 100){              
                DelayXus(1);
                xus++;
            }
            xus = 0;
            while(!DHT11IO && xus < 100){
                DelayXus(1);
                xus++;
            }
            DelayXus(45);           
            if(DHT11IO)  DumidTemp[i] |= 1;
            DumidTemp[i] <<= 1;
        }
    }
    DHT11IO = 1;
    return 1;
}
