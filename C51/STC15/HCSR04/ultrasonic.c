#include "STC12C5A.H"

typedef unsigned char u8;
typedef unsigned int u16;

sbit Trig = P0^6;
sbit Echo = P0^5;

//声明函数
void getDistance();//获取距离
void Timer0Init();//初始化定时器0
void UartInit();//初始化串口
void SendData(u8 data_buf); //发送数据到串口
void DelayXms(u16 n); //延时N ms

//定义全局变量
u8 cnt=0; //定时器0中断次数
u16 time; //计时
u16 distance; //距离


void main(){
	EA = 1;//开启总中断

	UartInit();//初始化串口
	Timer0Init();//初始化定时器0

	while(1){
		getDistance(); //获取距离
		DelayXms(800); //延时
	}

}

//获取距离
void getDistance()
{
	u8 i,j=60;
	u8 str[] = "D:-.--M";

	TH0 = TL0 = cnt = 0; //初始化定时器0
	Trig = 1; //拉高超声波模块触发IO
	while(j--); // 延时20us
	Trig = 0; //拉低超声波模块触发IO
	while(!Echo);//等待超声波模块输出IO拉高 
	TR0 = 1; //开启定时器计时
	while(Echo); //等待超声波模块输出IO拉低
 	TR0 = 0; //关闭定时器计时

 	//判断是否超出模块最大测距 4m
 	if(cnt < 5){
	 	//计算出时间
		time = (TH0 * 256 + TL0 + cnt * 65536) * (1 / 12.000) + 0.5;
		//计算出距离 340m/s 或 us/58cm
		distance = time / 58 + 0.5;
	}else
		distance = 0;

    str[2] = (distance % 1000 / 100) + '0';
    str[4] = (distance % 100 / 10) + '0';
    str[5] = (distance % 10) + '0';
    //从串口发出，也可以用显示器显示，自己接显示驱动就可以了
    for(i=0;i<7;i++){
        SendData(str[i]);
    }
    SendData('\n');
    //复位定时器
    cnt = 0;
    TH0 = TL0 = 0;
}
//定时器0寄存器初始化
void Timer0Init(void)		//100微秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x50;		//设置定时初值
	TH0 = 0xFB;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时
	ET0 = 1; //开启定时器中断
}

void T0() interrupt 1
{
	cnt++;
}
//串口初始化定时器1
void UartInit(void)		//4800bps@12.000MHz
{
    SCON=0x52;              	
	AUXR &= 0xFE;
    TMOD=0x20;	
	TH1=TL1=0xf3;
    PCON=0x80; 	
	TR1=1;
}

//发送一个字符
void SendData(u8 data_buf)
{
	SBUF = data_buf;
	while(!TI);//TI是发送成功标志
	TI = 0;
}

//延时1毫秒
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
//延时n毫秒
void DelayXms(u16 n)
{
    while(n--){
        Delay1ms();
    }
}
