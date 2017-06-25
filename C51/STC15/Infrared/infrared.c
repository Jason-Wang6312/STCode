#include "STC15Wxx.h"

typedef unsigned char u8;
typedef unsigned int u16;

u16 InfTime;
u8 InfCode[4] = {0xFF,0xFF,0xFF,0xFF};
u8 InfBitNum;


void main(){
    EA = 1;

    while(1){
        if(InfBitNum == 0x1F){
            send
        }
    }
}

//0口外部中断函数
void T0EXINT() interrupt 0{
    TR0 = 1;
    InfTime = ((int)TH0 << 4) + TL0;
    if(InfTime > 80 & (InfTime < 205)){ //判断两次下降沿之间的时间在零 560us+560us = 1.120ms 和一 560us+1.68ms = 2.24ms,大概在 900us~2.5ms之间才执行
        InfBitNum++;
        InfBitNum &= 0x1f; //等效if(InfBitNum>0x1f) InfBitNum=0x00; 这种操作比if判断更节约时间 0x1f=31
        InfCode[InfBitNum>>3] <<= 1;//InfBitNum>>3的范围(0~3)，等效于IR_data[i]向左移动1位 (i范围0~3)
        if(InfTime > 150) InfCode[InfBitNum>>3]++;//大于1.8ms以上的间隔为数据1 ，1就+1，0就不变。
    }else{
        TR0 = 0;
//        InfBitNum = 0;
    }
    TL0 = TH0 = 0;
    if(InfBitNum == 0x1f) TR0 = 0; //红外解码完成后关闭定时器0
}


void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
//	TL0 = 0x20;		//设置定时初值
//	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
//	TR0 = 1;		//定时器0开始计时
    IT0 = 1;                    //设置INT0的中断类型 (1:仅下降沿 0:上升沿和下降沿)
    EX0 = 1;                    //使能INT0中断
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

void SendData(u8 data_buf) //发送一个字符
{
	SBUF = data_buf;
	while(!TI);//TI是发送成功标志
	TI = 0;
}