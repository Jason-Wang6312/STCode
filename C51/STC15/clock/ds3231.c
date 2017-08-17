#include "DS3231.h"

unsigned char code RTC_addresss[9] = {0x00,0x01,0x02,0x04,0x05,0x06,0x03,0x11,0x12};
unsigned timedata[9];

//IIC延时函数
void DelayIIC(){
    //IIC总线限速延时函数 Delay10us。
    //该函数是空函数，延时4个机器周期。
    //_nop_();_nop_();_nop_();_nop_();
	unsigned char i;

	_nop_();
	_nop_();
	i = 27;
	while (--i);
}

//DS3231开始信号
void IIC_Start(){
    DS3231_SCL = 1;//拉高数据线
    DS3231_SDA = 1;//拉高时钟线
    DelayIIC();
    DS3231_SDA = 0;//在时钟线为高电平时，拉低数据线，产生起始信号。
    DelayIIC();
    DS3231_SCL = 0;//拉低时钟线
}

//DS3231停止信号
void IIC_Stop(){
    DS3231_SDA = 0;//拉低数据线
    DelayIIC();
    DS3231_SCL = 1;//拉高时钟线。
    DelayIIC();
    DS3231_SDA = 1;//时钟时线为高电平时，拉高数据线，产生停止信号。
    DelayIIC();
}

//ds3231应答
bit IIC_ACK(){//接收应答信号函数
    bit ack;//定义一个位变量，来暂存应答状态。
    DS3231_SDA = 1;//释放数据总线，准备接收应答信号。
    DelayIIC();
    DS3231_SCL = 1;//拉高时钟线。
    DelayIIC();
    ack = DS3231_SDA;//读取应答信号的状态。
    DelayIIC();
    DS3231_SCL = 0;//拉低时钟线。
    DelayIIC();
    return ack;//返回应答信号的状态，0表示应答，1表示非应答。
}

//从IIC总线写一个字节
void IIC_Write_Byte(unsigned char dat){
    //向IIC总线写入一个字节的数据函数
    unsigned char i;
    for(i=0;i<8;i++){//一个字节8位数据
        DS3231_SDA = dat&0x80;//写最高位的数据，官入数据是高到低位
        DelayIIC();
        DS3231_SCL = 1;//拉高时钟线，将数写入到设备中。
        DelayIIC();
        DS3231_SCL = 0;//拉低时钟线，允许改变数据线的状态
        DelayIIC();
        dat <<= 1;//数据左移一位，把次高位放在最高位,为写入次高位做准备
    }
}

//从IIC总线读一个字节
unsigned char IIC_Read_Byte(){
    unsigned char i;
    unsigned char dat = 0x00;//定义一个存取变量。
    for(i=0;i<8;i++){
        DS3231_SCL = 1;//拉高时钟线，为读取下一位数据做准备。
        dat <<= 1;//数据左移一位，把次高位放在最高位,为写入次高位做准备
        DelayIIC();
        if(DS3231_SDA)//如果为高就写入到数据，为零跳过
            dat |= 0x01;//数据是从高到低写入，所以右移或上。
        DS3231_SCL = 0;//拉低时钟线，为读取下一位数据做准备。
        DelayIIC();
        
    }
    return dat;//返回读取的一个字节数据。
}

//写入一个字节到DS3231
void DS3231_Write_Single_Byte(unsigned char addr,unsigned char dat){
    IIC_Start();//给IIC写入开始信号
    IIC_Write_Byte(0xD0);//写入DS3231在总线上的设备地址
    IIC_ACK();//等待DS3231应答
    IIC_Write_Byte(addr);//写入DS3231操作地址
    IIC_ACK();//等待DS3231应答
    IIC_Write_Byte(dat);//给DS3231该地址位写入数据
    IIC_ACK();//等待DS3231应答
    IIC_Stop();//给IIC总线写入停止信号
}

//读取DS3231一个字节
unsigned char DS3231_Read_Single_Byte(unsigned char addr){
    unsigned char dat;
    
    IIC_Start();//给IIC写入开始信号
    IIC_Write_Byte(0xD0);//写入DS3231在总线上的设备地址
    IIC_ACK();//等待DS3231应答
    IIC_Write_Byte(addr);//写入DS3231操作地址
    IIC_ACK();//等待DS3231应答
    IIC_Stop();//给IIC总线写入停止信号
    //-------------------读取DS3231该地址上的值--------------------
    IIC_Start();//给IIC写入开始信号
    IIC_Write_Byte(0xD1);//写入DS3231在总线上的设备地址
    IIC_ACK();//等待DS3231应答
    dat = IIC_Read_Byte();//读取DS3231该地址的值
    DelayIIC();
    IIC_Stop();//给IIC总线写入停止信号
    return dat;//返回读取的一个字节数据。
}

//BCD转成16进制数
unsigned char BCDToHEX(unsigned char val){
    unsigned char i;
    i = val&0x0F;//取出高四位
    val >>= 4;//取出低四位
    val &= 0x0F;//
    val *= 10;
    i += val;
    return i;
}

//16进制数转成BCD码
unsigned char HEXToBCD(unsigned char val){
    unsigned char i,j,k;

    i = val/10;//取出高四位
    j = val%10;//取出低四位
    k = j+(i << 4);//合并成BCD码
    return k;
}

//读取DS3231时间
void Read_RTC(void){
    unsigned char i,*p;
    p = RTC_addresss;
    for(i=0;i<9;i++){
        timedata[i] = DS3231_Read_Single_Byte(*p++);
        if(i == 4) timedata[i] &= 0x7F;
        if(i < 7) timedata[i] = BCDToHEX(timedata[i]); 
    }

}

//设置DS3231初始时间
void Set_RTC(void){
    unsigned char i;
																	//秒-分-时-日-月-年-星期
    unsigned char initTime[7] = {0x00,0x56,0x11,0x13,0x08,0x17,0x01};
    for(i = 0; i < 7; i++){
        DS3231_Write_Single_Byte(RTC_addresss[i],initTime[i]);
    }
    DS3231_Write_Single_Byte(0x0E,0x0C);
}