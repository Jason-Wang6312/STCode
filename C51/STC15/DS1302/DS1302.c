#include "ds1302.h"

bit flag200ms = 1;
struct sTime bufTime;

void DS1302Delay(){
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

void DS1302BrustRead(){

}

//写入一个字节
void DS1302ByteWrite(unsigned char dat)
{
	unsigned char i;
	for (i = 0; i < 8; i++)
	{
		dat >>= 1; 		//移出最后位
		DS1302_IO = CY; //把移出的位赋值给IO
		DS1302_SCLK = 1;  //然后拉高时钟
		_nop_();
        DS1302_SCLK = 0;  //再拉低时钟，完成一个位的操作
        _nop_();
	}
	DS1302_IO = 1; 		//保释放IO引脚
}

//读入一个字节
unsigned char DS1302ByteRead()
{
	unsigned char i,dat = 0x00;
	for (i = 0; i < 8; i++)
	{
		if(DS1302_IO){
			dat |= (1 << i);
		}
		DS1302_SCLK = 1;  //然后拉高时钟
		_nop_();
    	DS1302_SCLK = 0;  //再拉低时钟，完成一个位的操作
    	_nop_();
	}
	return dat;
}

//给ds1302的一个地址写入一个数据
void DS1302SingleWrite(unsigned char reg,unsigned char dat)
{
    DS1302_CE = 1;                   //使能片选信号
    DS1302Delay();
    DS1302ByteWrite((reg<<1)|0x80);  //发送写寄存器指令
    DS1302ByteWrite(dat);            //写入字节数据
    DS1302Delay();
    DS1302_CE = 0;                   //除能片选信号
}

//读ds1302的一个地址上个数据
unsigned char DS1302SingleRead(unsigned char reg)
{
	unsigned char dat;

	DS1302_CE = 1;                   //使能片选信号
	DS1302Delay();
    DS1302ByteWrite((reg<<1)|0x81);  //发送读寄存器指令
    dat = DS1302ByteRead();          //读取字节数据
    DS1302Delay();
    DS1302_CE = 0;                   //除能片选信号
    
    return dat;
}

//用突发模式连续写入8个寄存器数据，dat-待写入数据指针
void DS1302BurstWrite(unsigned char *dat)
{
	unsigned char i;
	
    DS1302_CE = 1;
    DS1302ByteWrite(0xBE);  //发送突发写寄存器指令
    for (i=0; i<8; i++)     //连续写入8字节数据
    {
        DS1302ByteWrite(dat[i]);
    }
    DS1302_CE = 0;
    DS1302_SCLK = 0;
}

//用突发模式连续读取8个寄存器的数据，dat-读取数据的接收指针
void DS1302BurstRead(unsigned char *dat)
{
    unsigned char i;
    
    DS1302_CE = 0;
	DS1302_SCLK = 0;
	_nop_();
	_nop_();
	_nop_();
    DS1302_CE = 1;
    DS1302Delay();
    DS1302ByteWrite(0xBF);  //发送突发读寄存器指令
    for (i=0; i<8; i++)     //连续读取8个字节
    {
        dat[i] = DS1302ByteRead();
    }
    DS1302Delay();
    DS1302_CE = 0;
    DS1302_SCLK = 0;
}

//设定实时时间，时间结构体格式的设定时间转换为数组并写入DS1302
void SetRealTime(struct sTime *time)
{
    unsigned char buf[8];
    
    buf[7] = 0;
    buf[6] = time->year;
    buf[5] = time->week;
    buf[4] = time->month;
    buf[3] = time->day;
    buf[2] = time->hour;
    buf[1] = time->minute;
    buf[0] = time->second;
    DS1302BurstWrite(buf);
}

//获取实时时间，即读取DS1302当前时间并转换为时间结构体格式
void GetRealTime(struct sTime *time)
{
    unsigned char buf[8];
    
    DS1302BurstRead(buf);
    time->year    = buf[6] + 0x2000;
    time->month   = buf[4];
    time->day     = buf[3];
    time->hour    = buf[2];
    time->minute  = buf[1];
    time->second  = buf[0];
    time->week    = buf[5];
}

void InitDS1302()
{
    unsigned char dat;
    struct sTime code InitTime[] = {  //2013年10月8日 12:30:00 星期二
        0x2017,0x04,0x09, 0x12,0x30,0x00, 0x02
    };
    
    DS1302_CE = 0;  //初始化DS1302通信引脚
	DS1302_SCLK = 0;
    dat = DS1302SingleRead(0);  //读取秒寄存器
    if ((dat & 0x80) != 0)      //由秒寄存器最高位CH的值判断DS1302是否已停止
    {
        DS1302SingleWrite(7, 0x00);  //撤销写保护以允许写入数据
        SetRealTime(&InitTime);      //设置DS1302为默认的初始时间
    }
}
