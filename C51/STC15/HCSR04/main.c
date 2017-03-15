#include "STC15F2K.H"
#include "STDIO.H"

typedef unsigned char u8;
typedef unsigned int u16;

sbit Trig = P0^5;
sbit Echo = P0^6;

u8 disCnt = 0;
u16 time;
u16 distance;
u8 disStr[4];
u8 buffer[];
u8 i=0;
u8 h,l;
bit isOk = 0;


void DelayXms(u16 x);
void Delay1ms();
void Delay20us();
void sendData(u8 dat);
void UartInit();
void Timer0Init();
void sendStr(u8 *str);
void getDistance();
void intToStr(u16 val);

void  main(){
    EA = 1;
    UartInit();
    Timer0Init();
    sendData(0x0F);
    
    while(1){
        if(isOk){
            sendData(0xEF);
//        DelayXms(1000);
            getDistance();
            isOk = 0;
        }
    };
}

void getDistance(){
    u8 i=0;
    
    Trig = 0;
    Echo = 0;
    DelayXms(10);
    TL0 = 0x80;		//设置定时初值
	TH0 = 0x44;		//设置定时初值
    Trig = 1;
    Delay20us();
    Trig = 0;
    sendData(0xEE);
    while(!Echo);
    TR0 = 1;
    sendData(0xAA);
    while(Echo);
    sendData(0xAE);
    TR0 = 0;
    
    sendData(disCnt);
    
    time = disCnt * 4000 + TH0 * 256 + TL0;
    sendData(time >> 8);
    sendData(time << 8);
//    distance = time/58;
//    
//    disStr[0] = distance/1000;
//    disStr[1] = distance%1000/100;
//    disStr[2] = distance%100/10;
//    disStr[3] = distance%10;
//    
//    for(;i < 4;i++){
//        sendData(disStr[i]);
//    }
    TL0 = 0x80;		//设置定时初值
	TH0 = 0x44;		//设置定时初值
    Trig = 0;
    Echo = 0;
    disCnt = 0;
    isOk = 1;
}

void intToStr(u16 val){   
    sprintf(buffer, "%s", val);
    return ;
}

void Timer0Init(void)		//4毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x80;		//设置定时初值
	TH0 = 0x44;		//设置定时初值
	TF0 = 0;		//清除TF0标志
//	TR0 = 1;		//定时器0开始计时
    ET0 = 1;
    
}

void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}

void sendData(u8 dat){
    SBUF = dat;
    while(!TI);
    TI = 0;
}

void sendStr(u8 *str){
    while(*str){
        sendData(*str++);
    }
}

void T0() interrupt 1{
    TL0 = 0x80;		//设置定时初值
	TH0 = 0x44;		//设置定时初值
    disCnt++;
}

void Delay20us()		//@12.000MHz
{
	unsigned char i;

	i = 57;
	while (--i);
}

void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}

void DelayXms(u16 x){
    while(x++){
        Delay1ms();
    }
}

