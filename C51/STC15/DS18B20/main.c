#include "init.h"

//共阴极数码管的0 1 2 3 4 5 6 7 8 9 A b C d E F
u8 code Dig[17]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
				 0x00};
//---A--
//|F   |B
//|	   |
//---G---
//|E   |C
//|    |
//---D-- .H
//
//4位共阴数码管脚图

//   L1----A----F----L2----L3----B
//                 
//   ---LED1--LED2--LED3--LED4---
//                 
//   E-----D----H----C-----G----L4
                 
//数码管定义
sbit L1 = P3^2; //数码管第一位共阴脚
sbit L2 = P3^7; //数码管第二位共阴脚
sbit L3 = P1^0; //数码管第三位共阴脚
sbit L4 = P1^2; //数码管第四位共阴脚
/*------------*/
sbit LA = P3^3; //数码管A段
sbit LB = P1^1; //数码管B段
sbit LC = P1^4; //数码管C段
sbit LD = P5^4; //数码管D段
sbit LE = P3^1; //数码管E段
sbit LF = P3^6; //数码管F段
sbit LG = P1^3; //数码管G段
sbit LH = P1^5; //数码管H段,小数点
                 
//变量定义
u16 cnt = 0,second = 0; //时间
u8 LedNum = 3; //当前显示数码管序号
u8 TempVal[5] = {1,2,3,4};
u8 dp = 0;


//声明函数
void Timer0Init();
void UartInit();
void SendData(u8 data_buf);
void showNum(u8 dat,u8 num);


void main(){
    u16 temp = 0;
    EA = 1;
    
//    P1M0 = 0x20;
//    P3M0 = 0x80;
//    P1M0 = 0x3F;
//    P3M0 = 0xCE;
//    P5M0 = 0x10;
    
    //串口初始化
    UartInit();
    //初始化定时器0
    Timer0Init();
    //初始化ds18b20
	
    
    while(1){
        if(second > 1000){
            second = 0;
            
            temp = getTemp();
            temp = temp * 0.0625 * 100 + 0.5;
            TempVal[0] = (temp % 10000 / 1000);
            TempVal[1] = (temp % 1000 / 100);
            TempVal[2] = (temp % 100 / 10);
            TempVal[3] = (temp % 10);
        }
    }
}

//显示数据
void showNum(u8 dat,u8 num){
    L1 = L2 = L3 = L4 = 1; //消抖

    //数据段位赋值
    dat >>= 1;
    LA = CY;
    dat >>= 1;
    LB = CY;
    dat >>= 1;
    LC = CY;
    dat >>= 1;
    LD = CY;
    dat >>= 1;
    LE = CY;
    dat >>= 1;
    LF = CY;
    dat >>= 1;
    LG = CY;
    dat >>= 1;
    LH = CY;
//    LH=dat&0x80;
//    dat=(dat<<1);
//    LG=dat&0x80;
//    dat=(dat<<1);
//    LF=dat&0x80;
//    dat=(dat<<1);
//    LE=dat&0x80;
//    dat=(dat<<1);
//    LD=dat&0x80;
//    dat=(dat<<1);
//    LC=dat&0x80;
//    dat=(dat<<1);
//    LB=dat&0x80;
//    dat=(dat<<1);
//    LA=dat&0x80;
    
    //显示数码管第几位
    switch(num){
        case 0:
            L1 = 0;
            break;
        case 1:
            L2 = 0;
            break;
        case 2:
            L3 = 0;
            break;
        case 3:
            L4 = 0;
            break;
        default:
            L1 = L2 = L3 = L4 = 1;
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

void SendData(u8 data_buf) //发送一个字符
{
	SBUF = data_buf;
	while(!TI);//TI是发送成功标志
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
    second ++;
    cnt ++;
    
    if(cnt > 3){
        cnt = 0;
        dp = Dig[TempVal[LedNum]];
        if(LedNum == 1) dp |= 0x80;
        showNum(dp,LedNum);
        LedNum ++ ;
        LedNum &= 0x03;
    }
}
