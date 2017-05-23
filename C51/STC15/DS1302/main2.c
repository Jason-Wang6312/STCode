#include "STC12C5A.H"
#include "INTRINS.H"

sbit DS1302_IO = P3^4;
sbit DS1302_CE = P3^5;
sbit DS1302_CL = P3^6;

sbit LED1=P1^0;
sbit LED2=P1^1;
sbit LED3=P1^2;
sbit LED4=P1^3;
unsigned char Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};

typedef unsigned char u8;
typedef unsigned int u16;

void Timer0Init();
void DS1302BurstRead(unsigned char leg);
void DS1302BurstWrite();
void DS1302Time();
unsigned char DS1302SingleRead(unsigned char addr);

u16 sec;
u8 ledIndex;
u8 ledTime;
u8 ledstr[4] = {0x01,0x02,0x03,0x04};
u8 second;
u8 minute;
u8 time[7] = {0x00,0x00,0x19,0x21,0x04,0x04,0x16};

void main(){
    
    DS1302_CE = 0;
    DS1302_CL = 0;
    
    EA = 1;
    Timer0Init();
    DS1302BurstWrite();
    
    while(1){
        if(sec > 200){
            sec = 0;
            DS1302BurstRead(4);
            ledstr[0] = time[1] & 0x0F;
            ledstr[1] = time[1] >> 4;
            ledstr[2] = time[2] & 0x0F;
            ledstr[3] = time[2] >> 4;
        }
        if(time[1] == 0x50){
            P2 = 0x00;
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
    if(ledTime > 4){
        ledTime = 0;
        ledshow(Disp_Tab[ledstr[ledIndex]],ledIndex);
        ledIndex++;
        ledIndex &= 0x03;
    }
}

void DS1302Start(){
    DS1302_CE = 0;
    DS1302_CL = 0;
    DS1302_IO = 0;
    _nop_();
    _nop_();
    _nop_();
    DS1302_CE = 1;
}

void DS1302End(){
    DS1302_CE = 0;
    _nop_();
    DS1302_CL = 1;
    _nop_();
    DS1302_IO = 1;
}

void DS1302ByteWrite(unsigned char dat){
    unsigned char i;
    for(i=0;i<8;i++){
        dat >>= 1;
        DS1302_IO = CY;
        DS1302_CL = 1;
        DS1302_CL = 0;
    }
}

unsigned char DS1302ByteRead(){
    unsigned char i,dat = 0x00;
    
    for(i=0;i<8;i++){             
        if(DS1302_IO){
            dat |= 1 << i;
        }
        DS1302_CL = 1;
        DS1302_CL = 0;
    }
    
    return dat;
}

void DS1302BurstRead(unsigned char leg){
    unsigned char i;
       
    DS1302Start();
    DS1302ByteWrite(0xBF);
    for(i=0;i < leg;i++){
        time[i] = DS1302ByteRead();
    }
    DS1302End();
    
//    return dat;
}

void DS1302BurstWrite(){
    unsigned char i;
       
    DS1302Start();
    DS1302ByteWrite(0xBE);
    for(i=0;i < 8;i++){
        DS1302ByteWrite(time[i]);
    }
    DS1302End();
}

unsigned char DS1302SingleRead(unsigned char addr){
    unsigned char dat;
    
    DS1302Start();
    DS1302ByteWrite(addr);
    dat = DS1302ByteRead();
    DS1302End();
    
    return dat;
}
