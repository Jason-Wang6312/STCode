#include "INTRINS.H"
#include "STC12C5A.H"
//#include "STC15F2K.H"

typedef unsigned char u8;
typedef unsigned int u16;

sbit K1=P1^4;
sbit K2=P1^5;
sbit K3=P1^6;
sbit K4=P1^7;
sbit K5=P3^5;

//函数声明
void Timer0Init();//定时器0初始化
void DelayXms(u16 n);//软件延时
void keyDriver();//按键驱动

//全局变量声明
u8 keySta[] = {1,1,1,1,1};//按键状态
u8 keyIndex;//按键索引

void main(){
    EA = 1;
    //初始化定时器0
    Timer0Init();
    
    while(1){
        keyDriver();//按键驱动
        if(keyIndex == 1){
            P20 = ~P20;//LED点亮
            keyIndex = 0;
        }
        
        //key();
    }
}

//按键驱动
void keyDriver(){
    static keybak[] = {1,1,1,1,1};//按键上次状态备份
    u8 i;
    
    for(i=0;i<5;i++){//循环按键状态处理
        if(keybak[i] != keySta[i]){//如果这次状态和上次状态不一样，说明有按键动作
            if(keybak[i] == 1){//如果上次是弹起，又与上次状态不一样，说明按下了
                keyIndex = i+1;//输出按键索引
            }
        }
        keybak[i] = keySta[i];//把这次状态保存
    }
}

//按键扫描程序
void keyScan(){
    u8 i;
    static keybuf[] = {0xFF,0xFF,0xFF,0xFF,0xFF};//按键扫描缓存
    
    //将当前的状态移入缓存
    keybuf[0] = (keybuf[0] << 1) | K1;//按键上
    keybuf[1] = (keybuf[1] << 1) | K2;//按键右
    keybuf[2] = (keybuf[2] << 1) | K3;//按键下
    keybuf[3] = (keybuf[3] << 1) | K4;//按键左
    keybuf[4] = (keybuf[4] << 1) | K5;//按键中
    
    for(i=0;i<5;i++){//循环按键读取状态，消抖处理
    	if(((keybuf[i] & 0x0F) == 0x00)){//循环读取按键状态，如果4次都读取的是低电低，就说明按下了
    		keySta[i] = 0;//缓存状态
		}else if(keybuf[i] & 0x0F == 0x0F){//循环读取按键状态，如果4次都读取的是低电低，就说弹起了
			keySta[i] = 1;//缓存状态
		}
    }
}

//定时器0初始化
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1;    //开启定时器0中断
}

//定时器按键扫描
void T0() interrupt 1{
    keyScan();//扫描按键
}

//软件1ms延时
void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 12;
	j = 168;
	do
	{
		while (--j);
	} while (--i);
}

//延时n秒
void DelayXms(u16 n){
    while(n--){
        Delay1ms();
    }
}
