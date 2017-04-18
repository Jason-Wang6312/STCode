#include <STC12C5A.H>
#include <intrins.h>
#include "DS1302.h"

typedef unsigned char u8;
typedef unsigned int u16;

//DS1302管脚
sbit SCLK = P3^6;       //时钟
sbit CE = P3^5;         //片选
sbit databus = P3^4;    //双向IO
sbit LED1=P1^0;
sbit LED2=P1^1;
sbit LED3=P1^2;
sbit LED4=P1^3;
unsigned char Disp_Tab[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};


u16 sec;
u8 ledIndex;
u8 ledTime;
u8 ledstr[4] = {0x01,0x02,0x03,0x04};
u8 second;
u8 minute;
u8 time[7] = {0x16,0x07,0x12,0x01,0x30,0x04,0x16};

//写模式寄存器
#define WRITE_CONTROL_SECONDS   0x80    //秒 //位7=1暂停    =0运行    
#define WRITE_CONTROL_MINUTE    0x82    //分
#define WRITE_CONTROL_HOUR      0x84    //时 位7 1——12小时模式 0——24小时模式  当位7为1时位5 0——am 1——pm    
#define WRITE_CONTROL_DAY       0x86    //日
#define WRITE_CONTROL_MONTH     0x88    //月
#define WRITE_CONTROL_WEEK      0x8a    //周
#define WRITE_CONTROL_YEAR      0x8e    //年

//读模式寄存器
#define READ_CONTROL_SECONDS    0x81    //秒
#define READ_CONTROL_MINUTE     0x83    //分
#define READ_CONTROL_HOUR       0x85    //时
#define READ_CONTROL_DAY        0x87    //日
#define READ_CONTROL_MONTH      0x89    //月
#define READ_CONTROL_WEEK       0x8b    //周
#define READ_CONTROL_YEAR       0x8d    //年

//控制读写寄存器
#define READ_CONTROL    0x8f    //读 
#define WRITE_CONTROL   0x8e    //写

#define OPEN_PROTECT    0x80    //开写保护
#define CLOSE_PROTECT   0x00    //关写保护

//突发模式寄存器
#define READ_COLOCK_BURST   0xbf    //读时钟突发模式寄存器
#define WRITE_COLOCK_BURST  0xbe    //写时钟突发模式寄存器

#define READ_RAM_BURST  0xFF    //读RAM突发模式寄存器
#define WRTIE_RAM_BURST 0xFE    //写RAM突发模式寄存器

//初始值存放     秒,分,时,日,月,周,年,关写保护
unsigned char write_Data_array[8]={0,0,1,1,1,1,15,00};

//BCD转十进制值存放
unsigned char BCD_to_TEN[8] = {0};

//读取值存放
unsigned char read_Data_array[8] = {0};

//十进制转BCD
unsigned char BCD_preservation[8]={0};

void start(void);   //开始
void end(void);     //结束
void write_data(unsigned char Data);    //写一个字节数据
unsigned char read_data(void);  //读一个字节数据
void ten_to_BCD_transformation(unsigned char *pvalue,unsigned char length,unsigned char *preturn);  //十进制转BCD
void BCD_to_ten_transformation(unsigned char *pvalue,unsigned char length,unsigned char *preturn);  //BCD转十进制
void init_ds1302_BURST(void);       //突发模式初始化
void get_ds1302_data(unsigned char length); //突发模式读取数据

//初始化ds1302
//DS1302突发模式的读写包含开关写保护
void init_ds1302_BURST(void)
{
    unsigned char i = 0;
    //十进制转BCD
    ten_to_BCD_transformation(write_Data_array,sizeof(write_Data_array),BCD_preservation);

    //关写保护
    start();
    write_data(WRITE_CONTROL);
    write_data(CLOSE_PROTECT);
    end();

    //突发模式写入数据
    start();
    write_data(WRITE_COLOCK_BURST);
    for(i = 0; i<8; i++)
    {
        write_data(BCD_preservation[i]);
    }
    end();

    //开写保护
    start();
    write_data(WRITE_CONTROL);
    write_data(OPEN_PROTECT);
    end();

}

//以突发模式获取ds1302时钟数据
void get_ds1302_data(unsigned char length)
{
    unsigned char i = 0;
    //关写保护
//    start();
//    write_data(WRITE_CONTROL);
//    write_data(CLOSE_PROTECT);
//    end();

    //开始读取数据
    start();
    write_data(READ_COLOCK_BURST);
    for(i=0; i<length; i++)
    {
        time[i] = read_data();
    }
    end();

    //开写保护
//    start();
//    write_data(WRITE_CONTROL);
//    write_data(OPEN_PROTECT);
//    end();

    //BCD转十进制
//    BCD_to_ten_transformation(read_Data_array,sizeof(read_Data_array),BCD_to_TEN);
}

//十进制转换为BCD
void ten_to_BCD_transformation(unsigned char *pvalue,unsigned char length,unsigned char *preturn)
{
    unsigned char i = 0,high = 0,low = 0;

    for(i=0;i<length;i++)
    {
        high = *(pvalue+i)/10%10;   //获取十进制十位   
        low = *(pvalue+i)%10;       //获取十进制个位

        *(preturn+i) = (high<<4)|low;   //十进制转换成BCD
    }   
}

//BCD转换为十进制
void BCD_to_ten_transformation(unsigned char *pvalue,unsigned char length,unsigned char *preturn)
{
    unsigned char i = 0,high = 0,low = 0;

    for(i=0;i<length;i++)
    {
        high = *(pvalue+i)/16%16;   //十六进制转十进制十位
        low = *(pvalue+i)%16;       //十六进制转十进制个位

        *(preturn+i) = (high*10)+low;   //十进制组合
    }
} 

//开始
void start(void)    
{
    CE = 0;
    SCLK = 0;
    databus = 0;
    CE = 1;
}

//结束
void end(void)  
{
    CE = 0;
    SCLK = 1;
    databus = 1;
}

//写一个字节数据
void write_data(unsigned char Data)
{
    unsigned char i = 0;
    for(i=0; i<8; i++)
    {
        SCLK = 0;
        databus = Data&0x01;
        SCLK = 1;
        Data>>=1;
    }
}

//读一个字节数据
unsigned char read_data(void)
{
    unsigned char i = 0,value = 0;
    for(i=0; i<8; i++)
    {
        SCLK = 1;
        if(databus == 1)
        {
            value|=0x80;
        }
        SCLK = 0;
        value>>=1;
    }
    return value;
}

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0 = 1;
}

void UartInit(void)		//4800bps@12.000MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xF3;		//设定定时初值
	TH1 = 0xF3;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
//    ES = 1;
}

void SendData(u8 data_buf) //发送一个字符
{
	SBUF = data_buf;
	while(!TI);//TI是发送成功标志
	TI = 0;
}

void main(void)
{
   
    EA = 1;
    UartInit();
    Timer0Init();
    
    init_ds1302_BURST();    //初始化DS1302
    
    
    while(1)
    {
        if(sec > 200){
            sec = 0;
            get_ds1302_data(8); //获DS1302值
            
            ledstr[0] = time[0] & 0x0F;
            ledstr[1] = time[0] >> 4;
            ledstr[2] = time[1] & 0x0F;
            ledstr[3] = time[1] >> 4;
            if(time[0] == 10){
                SendData(time[0]);  
            }
        }
    }
}

void ledshow(u8 dat,u8 index){
    LED1 = LED2 = LED3 = LED4 = 0;
    P0 = dat;
    switch(index){
        case 0:
            LED1 = 1;
            break;
        case 1:
            LED2 = 1;
            break;
        case 2:
            LED3 = 1;
            break;
        case 3:
            LED4 = 1;
            break;
    }
}

void T0() interrupt 1{
    TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
    sec++;
    ledTime++;
    if(ledTime > 1){
        ledTime = 0;
        ledshow(Disp_Tab[ledstr[ledIndex]],ledIndex);
        ledIndex++;
        ledIndex &= 0x03;
    }
}

