#include "STC15Wxx.h"

void UartInit(void);
void sendData(unsigned char dat);

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
    unsigned char i,str[] = "MaWei";
    
    UartInit();
    Delay300ms();
    while(1){
        for(i=0;i<5;i++){
            sendData(str[i]);
        }
    }
}

//void UartInit(void)		//9600bps@11.0592MHz
//{
//	SCON = 0x50;		//8位数据,可变波特率
//	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
//	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
//	T2L = 0xE0;		//设定定时初值
//	T2H = 0xFE;		//设定定时初值
//	AUXR |= 0x10;		//启动定时器2
//}


void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}



void sendData(unsigned char dat){
    
    SBUF = dat;
    while(!TI);
    TI = 0;
}

