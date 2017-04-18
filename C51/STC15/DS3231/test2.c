#include "init.h"

#define u8 unsigned char
#define u16 unsigned int

sbit DS3231_IIC_SDA=P0^5;
sbit DS3231_IIC_SCL=P0^6;
//DS3231寄存器地址
//器件写地址 0xD0, 器件读地址 0xD1
//日期时间
//秒 0x00,分0x01,时0x02,星期0x03,日0x04,月0x05,年0x06
//闹钟1
//秒 0x07,分0x08,时0x09,星期0x0a
//闹钟2
//没有秒  分0x0b,时0x0c,星期0x0d
//控制寄存器 0x0e,状态寄存器 0x0f,晶体老化寄存器0x10
//温度寄存器高字节(8位)0x11,     温度寄存器低字节(高2位)0x12

u8 code rtc_address[9]={0x00,0x01,0x02,0x04,0x05,0x03,0x06,0x11,0x12}; //秒分时日月周年  温度最后2位 最低位读写位
u8 timedate[9];

bit state=0;
u8 cnt=0;//定时器2 50ms计数

void UartInit();
void SendData(u8 data_buf);
void DelayMs();

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

void DelayMs(unsigned int i)  //延时N毫秒，0~65535ms
{
   while(--i)
   {
	 Delay1ms();
   }  
}

//-------------------------------------------------------------------
void delay_IIC(void)   
{//IIC总线限速延时函数 Delay10us。
//该函数是空函数，延时4个机器周期。
   //_nop_();_nop_();_nop_();_nop_();
	unsigned char i;

	_nop_();
	_nop_();
	i = 27;
	while (--i);
}
//-------------------------------------------------------------------
void DS3231_IIC_Init(void)
{//IIC总线初始化函数
//    DS3231_IIC_RST=1;
    DS3231_IIC_SDA=1;//释放IIC总线的数据线。
    DS3231_IIC_SCL=1;//释放IIC总线的时钟线。
}
//-------------------------------------------------------------------
void DS3231_IIC_start(void)
{//IIC总线产生起始信号函数
     
     DS3231_IIC_SDA=1;//拉高数据线
     DS3231_IIC_SCL=1;//拉高时钟线
     delay_IIC();
     DS3231_IIC_SDA=0;//在时钟线为高电平时，拉低数据线，产生起始信号。
     delay_IIC();
     DS3231_IIC_SCL=0;//拉低时钟线
}
//-------------------------------------------------------------------
void DS3231_IIC_stop(void)
{//IIC总线产生停止信号函数
    DS3231_IIC_SDA=0;//拉低数据线
    delay_IIC();
    DS3231_IIC_SCL=1;//拉高时钟线。
    delay_IIC();
    DS3231_IIC_SDA=1;//时钟时线为高电平时，拉高数据线，产生停止信号。
    delay_IIC();
}
//-------------------------------------------------------------------
bit DS3231_IIC_Tack(void)
{//接收应答信号函数
    bit ack;//定义一个位变量，来暂存应答状态。
    DS3231_IIC_SDA=1;//释放数据总线，准备接收应答信号。
    delay_IIC();
    DS3231_IIC_SCL=1;//拉高时钟线。
    delay_IIC();
    ack=DS3231_IIC_SDA;//读取应答信号的状态。
    delay_IIC();
    DS3231_IIC_SCL=0;//拉低时钟线。
    delay_IIC();
    return ack;//返回应答信号的状态，0表示应答，1表示非应答。
}
//-------------------------------------------------------------------
void DS3231_IIC_write_byte(unsigned char Data)
{//向IIC总线写入一个字节的数据函数
    unsigned char i;
     for(i=0;i<8;i++)//有8位数据
    {
        DS3231_IIC_SDA=Data&0x80;//写最高位的数据
        delay_IIC();
        DS3231_IIC_SCL=1; //拉高时钟线，将数写入到设备中。
        delay_IIC();
        DS3231_IIC_SCL=0;//拉低时钟线，允许改变数据线的状态
        delay_IIC();
        Data=Data<<1;//数据左移一位，把次高位放在最高位,为写入次高位做准备
    }
}
//-------------------------------------------------------------------
unsigned char DS3231_IIC_read_byte()
{//从IIC总线读取一个字节的数据函数
    unsigned char i;
    unsigned char Data;       //定义一个缓冲寄存器。
    for(i=0;i<8;i++)//有8位数据
    {
        DS3231_IIC_SCL=1;//拉高时钟线，为读取下一位数据做准备。
        delay_IIC();
        Data=Data<<1;//将缓冲字节的数据左移一位，准备读取数据。
        delay_IIC();
       
        if(DS3231_IIC_SDA)//如果数据线为高平电平。
            Data=Data|0x01;//则给缓冲字节的最低位写1。
        DS3231_IIC_SCL=0;//拉低时钟线，为读取下一位数据做准备。
        delay_IIC();
    }
    return Data;//返回读取的一个字节数据。
}
//-------------------------------------------------------------------
void DS3231_IIC_single_byte_write(unsigned char Waddr,unsigned char Data)
{//向任意地址写入一个字节数据函数
    DS3231_IIC_start();//产生起始信号
    DS3231_IIC_write_byte(0xd0);//写入设备地址（写）
    DS3231_IIC_Tack();//等待设备的应答
    DS3231_IIC_write_byte(Waddr);//写入要操作的单元地址。
    DS3231_IIC_Tack();//等待设备的应答。
    DS3231_IIC_write_byte(Data);//写入数据。
    DS3231_IIC_Tack();//等待设备的应答。
    DS3231_IIC_stop();//产生停止符号。
}
//-------------------------------------------------------------------
unsigned char DS3231_IIC_single_byte_read(unsigned char Waddr)
{//从任意地址读取一个字节数据函数
    unsigned char Data;//定义一个缓冲寄存器。
    DS3231_IIC_start();//产生起始信号
    DS3231_IIC_write_byte(0xd0);//写入设备地址（写）
    DS3231_IIC_Tack();//等待设备的应答
    DS3231_IIC_write_byte(Waddr);//写入要操作的单元地址。
    DS3231_IIC_Tack();//等待设备的应答。
    DS3231_IIC_stop();//产生停止符号。
    DS3231_IIC_start();//产生起始信号
    DS3231_IIC_write_byte(0xd1);//写入设备地址（写）
    DS3231_IIC_Tack();//等待设备的应答
    Data=DS3231_IIC_read_byte();//写入数据。
    delay_IIC();
    DS3231_IIC_stop();//产生停止符号。
    //-------------------返回读取的数据--------------------
    return Data;//返回读取的一个字节数据。
}

unsigned char BCD2HEX(unsigned char val)   // 0x20转换成20
{
    unsigned char i;
    i= val&0x0f;
    val >>= 4;
    val &= 0x0f;
    val *= 10;
    i += val;
    
    return i;
}

unsigned char HEX2BCD(unsigned char val)//10进制的20转换成16进制的0x20
{
  unsigned char i,j,k;

  i=val/10;
  j=val%10;
  k=j+(i<<4);
  return k;
}

void Read_RTC(void)
{
    unsigned char i,*p;
    p=rtc_address;         //地址传递
    for(i=0;i<9;i++)            //分7次读取 秒分时日月周年
    {
      timedate[i]=DS3231_IIC_single_byte_read(*p);
	  //时间和日期采用BCD编码，温度不采用。
	  //BCD编码，需要转换一下，比如0x12转成12，就是把16进制的数，当10进制的看。
	  if (i==4)
	 //去掉月份第一位的世纪位。
	 //先左移一位，再移回来。
	  {
			timedate[4]=timedate[4]<<1;
			timedate[4]=timedate[4]>>1;
	  }
	  if (i<7) timedate[i]=BCD2HEX(timedate[i]);
      p++;
    } 
}
void Set_RTC(void)
{
    unsigned char i;//,tmp;
    for(i=0;i<7;i++)
    {       
		//把10进制的数，转换成BCD编码，也就是把12转成0x12
		timedate[i]=HEX2BCD(timedate[i]);//BCD处理
    } 
	for(i=0;i<7;i++)        //7次写入 秒分时日月周年
	{
		DS3231_IIC_single_byte_write(rtc_address[i],timedate[i]);
	}
    DS3231_IIC_single_byte_write(0x0e,0x0c);
}

//主函数
void main()
{

    u8 i;
    u8 dat;

    EA = 1; 
    UartInit();
    
    DelayMs(200);
	DS3231_IIC_Init();//时钟芯片初始化
    
	while(1)
	{
	  DelayMs(300);
	  

	  //晚上1点以后关掉，6点打开
	  Read_RTC();

        for(i=0;i<9;i++){
            dat = timedate[i];
            SendData(dat);
        }
        SendData(0x0A);  

	}
}

//uart初始化
void UartInit(void)     //4800bps@12.000MHz
{
    SCON = 0x50;        //8位数据,可变波特率
    AUXR |= 0x01;       //串口1选择定时器2为波特率发生器
    AUXR |= 0x04;       //定时器2时钟为Fosc,即1T
    T2L = 0x8F;     //设定定时初值
    T2H = 0xFD;     //设定定时初值
    AUXR |= 0x10;       //启动定时器2
}

//发送一个字符
void SendData(u8 data_buf){
    SBUF = data_buf;
    while(!TI);//TI是发送成功标志
    TI = 0;
}
