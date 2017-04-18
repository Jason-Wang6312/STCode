#include "init.h"
#include "ds3231.h"

void UartInit();
void SendData(u8 data_buf);

void main(){
    u8 str[] = "STC12MaWei";
    u8 i;
    
    EA = 1; 
    UartInit();

    while(1){
        Read_RTC();
    }
}

//uart初始化
void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}

//发送一个字符
void SendData(u8 data_buf){
	SBUF = data_buf;
	while(!TI);//TI是发送成功标志
	TI = 0;
}

//延时1微秒
void Delay1us(){  //@12MHz
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

void DelayXus(u8 i){
    while(i--){
        Delay1us();
    }
}