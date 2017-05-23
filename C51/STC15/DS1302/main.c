#include "STC15Wxx.h"
#include "ds1302.h"

//定义变量
unsigned int sec;


//声明函数
void UartInit(void);//串口初始化
void Timer0Init(void);//定时器0初始化
void sendData(unsigned char dat);//发送串口数据

void main(){
    EA = 1;//开启总中断
    
    //初始化功能模块
    UartInit();
    Timer0Init();

    while(1){
        if(sec > 200){//如果做时钟，最好200左右
            sec = 0;
            DS1302BurstRead(3);//看你显示大小，如果只显示秒、分、时，只读三位就好
            sendData(time[0]);//发送秒到串口
        }
    }
}

//串口初始化
void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}

//发送一个字节到串口函数
void sendData(unsigned char dat){
    SBUF = dat;//写入发送值到串口缓存中
    while(!TI);//等待数据发送完闭
    TI = 0;//清除发送寄存器状态
}

//定时器0初始化
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1; //开启定时器中断
}

//定时器0中断
void T0() interrupt 1{
    sec++; //中断次数
}

