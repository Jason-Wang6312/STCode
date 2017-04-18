#include "STC15Wxx.h"

#define u8 unsigned char
#define u16 unsigned int

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
u16 cnt = 0,second = 0,num; //时间
u8 LedNum = 3; //当前显示数码管序号
u8 TempVal[5] = {1,2,3,4};
u8 dp = 0;

//声明函数
void Timer0Init(void);//定时器0寄存器初始化
void showNum(u8 dat,u8 num);//显示数据

void main(){
    u16 temp = 0;
    EA = 1;
    
    //下面寄存器是开启强推挽输出，就是输出电流大，
    //开启了强推挽输出后LED是高亮
    P1M0 = 0x3F;
    P3M0 = 0xCE;
    P5M0 = 0x10;
    
    //初始化定时器0
    Timer0Init();
    //初始化ds18b20
	
    
    while(1){
        if(second > 1000){
            second = 0;
            num ++;
            
            TempVal[0] = (num % 10000 / 1000);
            TempVal[1] = (num % 1000 / 100);
            TempVal[2] = (num % 100 / 10);
            TempVal[3] = (num % 10);
        }
    }
}

//显示数据
void showNum(u8 dat,u8 num){
    L1 = L2 = L3 = L4 = 1; //消抖

    //数据段位赋值
    //此方式为位移缓存操作文件
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
    //此方式为位操作，这两促都可以，上面这种方式更优化，更快
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

//定时器0寄存器初始化
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

//定时器0中断函数
void T0() interrupt 1{
    second ++;
    cnt ++;
    
    if(cnt > 3){ //显示时间，如果这个设置大了，就出现闪烁，太小了也不
        cnt = 0;
        showNum(Dig[TempVal[LedNum]],LedNum);//显示字符，先选择显示的位，然后转换成LED显示数码位
        LedNum ++ ; //显示那位数码管
        LedNum &= 0x03;//位操作，相当于0-3;
    }
}
