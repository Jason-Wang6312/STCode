#include "init.h"
//导入外设驱动
#include "ds3231.h"
#include "DS18B20.H"

//---A--
//|F   |B
//|       |
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
//共阴极数码管的0 1 2 3 4 5 6 7 8 9 A b C d E F
u8 code Dig[17]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
//u8 code Dig[17]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,0x00};

//数码管定义
sbit L1 = P3^4; //数码管第一位共阴脚
sbit L2 = P3^5; //数码管第二位共阴脚
sbit L3 = P3^6; //数码管第三位共阴脚
sbit L4 = P3^7; //数码管第四位共阴脚
sbit LS = P5^4;
#define LD  P2
//无线路由控制IO
sbit wirelessRoute = P1^2;
                  
//变量定义
u16 cnt,changcode,second,num,temp; //时间
u16 readInterval;
u8 showIndex;
u8 LedNum = 3; //当前显示数码管序号
u8 TempVal[4] = {1,2,3,4};
u8 flashsec = 15;
u8 flashIndex = 1;
u8 dp = 0;

//定义闹钟
typedef struct{
    unsigned char sMinute; //定时开始分钟
    unsigned char shour; //定时开始小时
    unsigned char eMinute; //定时结束分钟
    unsigned char ehour; //定时结束小时
    unsigned char sWeek; //定时开始星期
    unsigned char eWeek; //定时结束星期
}alarm;
 
//声明函数
void Timer0Init(void);//定时器0寄存器初始化
void showNum(u8 dat,u8 num);//显示数据
void UartInit(void);//串口初始化
void SendData(u8 data_buf);//发送字符
void delayXms(u16 x);//延时x毫秒
void nowLedCode(u8 type);
 
void main(){
    u8 i = 0;
    EA = 1;
     
    //下面寄存器是开启强推挽输出，就是输出电流大，
    //开启了强推挽输出后LED是高亮
//    P1M0 = 0x3F;
//    P3M0 = 0xCE;
//    P5M0 = 0x10;
//    P2M0 = 0xFF;
     
    //初始化定时器0
    Timer0Init();
    //初始化串口
    UartInit();
    delayXms(1000);        
    Set_RTC();
    delayXms(1000);
     
    while(1){
        if(changcode > 5000){
            changcode = 0;    
            showIndex++;
            if(showIndex > 1) showIndex = 0;
        }
        if(second > 998){
            second = 0;
            if(showIndex == 0){
                LS = ~LS;
            }else{
                LS = 1;
            }
        }
        if(readInterval > 300){
            readInterval = 0;
            nowLedCode(showIndex);
        }
    }
}

void nowLedCode(u8 type){
    u8 sec,miute,hour,day,month,year;
    
    switch(type){
        case 0: //显示时间时、分
            miute = DS3231_Read_Single_Byte(0x01);
            hour = DS3231_Read_Single_Byte(0x02);
            TempVal[0] = hour >> 4;
            TempVal[1] = hour&0x0F;
            TempVal[2] = miute >> 4;
            TempVal[3] = miute&0x0F;
            if((hour > 0) && (hour < 7)){
                wirelessRoute = 1;
            }else
                wirelessRoute = 0;
            break;
        case 1: //显示温度
            temp = getTemp();
            temp = temp * 0.0625 * 100 + 0.5;
            TempVal[0] = (temp % 10000 / 1000);
            TempVal[1] = (temp % 1000 / 100);
            TempVal[2] = (temp % 100 / 10);
            TempVal[3] = 0x0C;
            break;
        case 2: //显示时间月、日
            day = DS3231_Read_Single_Byte(0x04);
            month = DS3231_Read_Single_Byte(0x05);
            TempVal[0] = (miute >> 4);
            TempVal[1] = miute&0x0F;
            TempVal[2] = sec >> 4;
            TempVal[3] = sec&0x0F;
            break;
        case 3: //年
            year = DS3231_Read_Single_Byte(0x06);
            TempVal[0] = 2;
            TempVal[1] = 0;
            TempVal[2] = year >> 4;
            TempVal[3] = year&0x0F;
            break;
        default:
            break;
    }
}

//显示数据
void showNum(u8 dat,u8 num){
    L1 = L2 = L3 = L4 = 1; //消抖
 
    //温度加点处理
    if((showIndex == 1)&&(num == 1)){
        dat &= 0x7F; 
    }

    //数据段位赋值
    LD = dat;
     
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

////定时器0中断函数 125us
//void Timer0() interrupt 1
//{
//   irTime++; //计时增加125us         	
//   if(irTime==240) {irTime--;codeCnt=0x3f;} // ir解码后码值存放时间， 240*125us = 30ms  0x3f=64
//   if(IR_IO)   Irprot_LastState=1; // 记录IO状态
//   else if(Irprot_LastState)       // 有下降沿，并且上个状态是高电平，表示红外管收到数据
//   {
//      Irprot_LastState = 0;        // 下降沿后IO状态记录为0
//      if(irTime<24) // 小于24*125us=3ms的间隔才进行处理	因为红外线的0的周期1.125ms，1的周期2.25ms
//      {
//         codeCnt++; //数据码位计数+1
//		 codeCnt &= 0x1f; //等效if(codeCnt>0x1f) codeCnt=0x00; 这种操作比if判断更节约时间 0x1f=31
//         IR_data[codeCnt>>3] <<= 1; //codeCnt>>3的范围(0~3)，等效于IR_data[i]向左移动1位 (i范围0~3)
//         if( irTime>15 )  IR_data[codeCnt>>3]++;  //大于15*125us=1.875ms的间隔为数据1 ，1就+1，0就不变。
//      }
//      irTime = 0;                  // 下降沿处理完成，将时间清0
//	  if(codeCnt==31) TR0=0,EX0=1;//解码完成后，掉定时器0，顺便打开中断0
//   }	
//}

//void INT0_ISR(void) interrupt 0
//{
//	irTime=239;//给irTime赋予初始值，定时器0无法持续中断了，所以这里赋予默认值
//	EX0=0; // INT0 暂时关闭
//	TR0=1; //定时器0只有检测到有红外数据，才开始进行解码。
//}

//void INT0Init(void)//多了一个中断0
//{
//	IT0 = 0;	//INT0中断类型：1仅下降沿中断 0上下都中断
//	EX0 = 1;	// 使能INT0中断
//}

//定时器0寄存器初始化
void Timer0Init(void)        //1毫秒@12.000MHz
{
    AUXR |= 0x80;        //定时器时钟1T模式
    TMOD &= 0xF0;        //设置定时器模式
    TL0 = 0x20;        //设置定时初值
    TH0 = 0xD1;        //设置定时初值
    TF0 = 0;        //清除TF0标志
    TR0 = 1;        //定时器0开始计时
    ET0 = 1;
}
 
//定时器0中断函数
void T0() interrupt 1{
    second ++;
    changcode++;
    readInterval++;
    cnt ++;
     
    if(cnt > 3){ //显示时间，如果这个设置大了，就出现闪烁，太小了也不
        cnt = 0;
        
//        if(flashIndex == LedNum){
//            flashsec = 0;
//            showNum(0xFF,5);
//        }else{
//            showNum(Dig[TempVal[LedNum]],LedNum);//显示字符，先选择显示的位，然后转换成LED显示数码位
//        }
        showNum(Dig[TempVal[LedNum]],LedNum);//显示字符，先选择显示的位，然后转换成LED显示数码位
        LedNum ++ ; //显示那位数码管
        LedNum &= 0x03;//位操作，相当于0-3;
        flashsec++;
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

//延时x ms
void delayXms(u16 x){
    unsigned char i, j;
    while(x--){
        i = 12;
        j = 169;
        do
        {
            while (--j);
        } while (--i);
    }
}
