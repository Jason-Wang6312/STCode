#include "STC15F2K.H"

sbit DS3231_SDA = P32;
sbit DS3231_SCL = P33;


//DS3231开始信号
void IIC_Start(){
    DS3231_SCL = 1;
    DS3231_SDA = 1;
    DelayIIC();
    DS3231_SDA = 0;
    DelayIIC();
    DS3231_SCL = 0;
}

//DS3231停止信号
void IIC_Stop(){
    DS3231_SDA = 0;
    DelayIIC();
    DS3231_SCL = 1;
    DelayIIC();
    DS3231_SDA = 1;
    DelayIIC();
}

//ds3231应答
bit IIC_ACK(){
    bit ack;
    DS3231_SDA = 1;
    DelayIIC();
    DS3231_SCL = 1;
    DelayIIC();
    ack = DS3231_SDA;
    DelayIIC();
    DS3231_SCL = 0;
    DelayIIC();
    return ack;
}

//从IIC总线写一个字节
void IIC_Write_Byte(unsigned char dat){
    u8 i;
    for(i=0;i<8;i++){
        dat <<= 1;
        DS3231_SDA = CY;
        DelayIIC();
        DS3231_SCL = 1;
        DelayIIC();
        DS3231_SCL = 0;
        DelayIIC();
    }
}

//从IIC总线读一个字节
unsigned char IIC_Read_Byte(){
    unsigned char i;
    unsigned char dat;
    for(i=0;i<8;i++){
        DS3231_SCL = 1;
        DelayIIC();
        if(DS3231_SDA)
            dat |= 1 << i;
        DS3231_SCL = 0;
        DelayIIC();
    }
    return dat;
}

//写入一个字节到DS3231
unsigned char DS3231_Write_Single_Byte(unsigned char addr,unsigned char dat){
    //给IIC写入开始信号
    IIC_Start();
    //写入DS3231在总线上的设备地址
    IIC_Write_Byte(0xD0);
    //等待DS3231应答
    IIC_ACK();
    //写入DS3231操作地址
    IIC_Write_Byte(addr);
    //等待DS3231应答
    IIC_ACK();
    //给DS3231该地址位写入数据
    IIC_Write_Byte(dat);
    //等待DS3231应答
    IIC_ACK();
    //给IIC总线写入停止信号
    IIC_Stop();
}

//读取DS3231一个字节
unsigned char DS3231_Read_Single_Byte(unsigned char addr){
    unsigned char dat;
    //给IIC写入开始信号
    IIC_Start();
    //写入DS3231在总线上的设备地址
    IIC_Write_Byte(0xD0);
    //等待DS3231应答
    IIC_ACK();
    //写入DS3231操作地址
    IIC_Write_Byte(addr);
    //等待DS3231应答
    IIC_ACK();
    //给IIC总线写入停止信号
    IIC_Stop();
    //读取DS3231该地址上的值
    //给IIC写入开始信号
    IIC_Start();
    //写入DS3231在总线上的设备地址
    IIC_Write_Byte(0xD1);
    //等待DS3231应答
    IIC_ACK();
    //读取DS3231该地址的值
    dat = IIC_Read_Byte();
    //给IIC总线写入停止信号
    IIC_Stop();
    return dat;
}

//16进制数转成BCD码
unsigned char HEXToBCD(unsigned char val){
    unsigned char i,j,k;

    i = val/10;
    j = val%10;
    k = j+(i << 4);
    return k;
}

//BCD转成16进制数
unsigned char BCDToHEX(unsigned char val){
    unsigned char i;
    i = val&0x0F;
    val >> = 4;
    val &= 0x0F;
    val *= 10;
    i += val;
    return i;
}

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

//延时1微秒
void Delay1us(){  //@12MHz
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}


void main(){

    while(1){

    }
}
