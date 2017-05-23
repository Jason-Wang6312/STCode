#include "STC15Wxx.H"
//#include "STC12C5A.H"
#include "INTRINS.H"

sbit DS1302_IO = P3^3;
sbit DS1302_CE = P5^5;
sbit DS1302_CL = P5^4;
sbit WIFI = P3^2;
//sbit DS1302_CL = P3^2;

//sbit DS1302_IO = P3^4;
//sbit DS1302_CE = P3^5;
//sbit DS1302_CL = P3^6;


void UartInit();
void Timer0Init();
void sendData(unsigned char dat);
void DS1302BurstWrite();
void DS1302BurstRead(unsigned char leg);
unsigned char DS1302SingleRead(unsigned char addr);

unsigned char time[7] = {0x50,0x15,0x20,0x23,0x04,0x06,0x17};
unsigned int sec;
unsigned char second;
bit busy;

void Delay300ms()		//@12.000MHz
{
	unsigned char i, j, k;

	i = 14;
	j = 174;
	k = 224;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main(){
    unsigned char i,k = 0x26;
    EA = 1;
    
    UartInit();
    Timer0Init();
    DS1302BurstWrite();
    
    Delay300ms();
    
    i = ((k > 0x25) && (k < 0x27));
    while(1){
        if(sec > 1000){
            sec = 0;
            DS1302BurstRead(3);
            sendData(time[0]);
            sendData(time[1]);
            sendData(time[2]);
            sendData(WIFI);
            if((time[2] > 0x01) && (time[2] < 0x08)){
                WIFI = 0;
            }else{
                WIFI = 1;
            }
        }
    }
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

void sendData(unsigned char dat){
    SBUF = dat;
    while(!TI);
    TI = 0;
}

void T0() interrupt 1{
    sec++;
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
