#include "conf.h"
#include "infrared.h"
#include "LCD1602.H"

//LED共阳极
u8 code LEDCode[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
u8 LedDisplayCode[4] = {0x00,0x00,0x02,0x03};

//红外变量定义
extern u8 IR_data[4]; // 接收数据缓存
extern u8 codeCnt;//红外检测码
u8 InfCode;//红外按键码


//main定义
//void Timer1Init();
void displayInf();
void UartInit();
void SendData(u8 data_buf);
//u16 cnt=0;

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


void delay_ms(unsigned int i)
{
   while(i--)Delay1ms();
}

void main(){

    EA = 1;
    
    UartInit();
    Timer0Init();
//    Timer1Init();
    LCDInit();
    
    
    while(1){
        SendData(0xFA);
        if(codeCnt == 31){
            if(IR_data[2] == IR_data[3]){
                InfCode = IR_data[2];
                displayInf();
                delay_ms(30);//延时30ms等待数据超时，遥控器一次发3组相同的数据，这里用延时只接收1组就好了。
            }
        }
    }
}

void displayInf(){
    u8 str[2];
    
    str[0] = (InfCode >> 4) + '0';
    str[1] = (InfCode & 0x0F) + '0';
    LCDShowStr(0,0,str);
}

void UartInit(void)		//4800bps@12.000MHz
{
    SCON=0x52;
	AUXR &= 0xFE;
    TMOD=0x20;				
	TH1=TL1=0xf3;
    PCON=0x80; 	 
	TR1=1;	
    ES = 1;
    
//	SCON = 0x50;		//8位数据,可变波特率
//	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
//	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
//	T2L = 0x8F;		//设定定时初值
//	T2H = 0xFD;		//设定定时初值
//	AUXR |= 0x10;		//启动定时器2
}

void SendData(u8 data_buf) //发送一个字符
{
	SBUF = data_buf;
	while(!TI);//TI是发送成功标志
	TI = 0;
}

//void Timer1Init(void)		//1毫秒@12.000MHz
//{
//	AUXR |= 0x40;		//定时器时钟1T模式
//	TMOD &= 0x0F;		//设置定时器模式
//	TMOD |= 0x10;		//设置定时器模式
//	TL1 = 0x20;		//设置定时初值
//	TH1 = 0xD1;		//设置定时初值
//	TF1 = 0;		//清除TF1标志
//	TR1 = 1;		//定时器1开始计时
//    ET1 = 1;
//}

//void T1() interrupt 3{
//    cnt++;
//    TL1 = 0x20;		//设置定时初值
//	TH1 = 0xD1;		//设置定时初值
//}
