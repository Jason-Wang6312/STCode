/****************************************************
 * @doc IIC通信协议驱动
 * @author 马伟
 * @version V1.0
 * @date 2017-05-10
 * @homepage http://www.mawei.live
*****************************************************/

#include "iic.h"

//IIC延时，标准的IIC通信为100KHZ,高速的为400KHZ,延时自己调整
void IIC_Delay(){
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

//IIC初始化
void IIC_Init(){
    IIC_SDA = 1;
    IIC_SCL = 1;
}

//IIC通信开始信号： SCL为高电平时，SDA由高电平向低电平转变，开始传送数据。
void IIC_Start(){
    IIC_SDA = 1;
    IIC_SCL = 1;
    IIC_Delay();
    IIC_SDA = 0;
    IIC_Delay();
    IIC_SCL = 0;
}

//IIC通信停止信号： SCL为高电平时，SDA由低电平向高电平转变，结束传送数据
void IIC_Stop(){
    IIC_SDA = 0; 
    IIC_Delay();
    IIC_SCL = 1; 
    IIC_Delay();
    IIC_SDA = 1;
}

//IIC通信应答信号： 接收数据的IC在接收到8bit数据后，会拉低数据线，表示数据接收成功
bit IIC_ACK(){
    bit ack;//定义一个位变量，来暂存应答状态。
    IIC_SCL = 0; //只有在时钟线低时才能改变数据线电平状态
    IIC_Delay();
    IIC_SDA = 1; //释放数据总线，准备接收应答信号。
    IIC_Delay();
    IIC_SCL = 1; //拉高时钟线。
    IIC_Delay();
    ack = IIC_SDA; //读取状态
    IIC_SCL = 0;
    return ack; //返回应答信号的状态，0表示应答，数据接收成功。反之失败
}

//向IIC总线上写一个字节
void IIC_Write_Byte(unsigned char dat){
    unsigned char i;
    IIC_SCL = 0; //只有在时钟线低时才能改变数据线电平状态
    for(i=0;i<8;i++){
        IIC_SDA = (bit)(dat&0x80); //IIC通信高位再前，低位再后。写入高位，（bit）表示强制转换成位
        IIC_SCL = 1; //拉高让从机读
        IIC_Delay();
        IIC_SCL = 0; //拉低为下位数据线准备
        IIC_Delay();
        dat <<= 1; //数据左移一位，把次高位放在最高位,为写入次高位做准备
    }
}

//从IIC总线上读取一个字节
unsigned char IIC_Read_Byte(){
    unsigned char i;
    unsigned char dat = 0x00;//定义一个存取变量。
    IIC_SCL = 0; //只有在时钟线低时才能改变数据线电平状态    
    for(i=0;i<8;i++){
        IIC_SCL = 1;
        IIC_Delay();
        dat <<= 1;//数据左移一位，把次高位放在最高位,为写入次高位做准备
        if(IIC_SDA) //如果为高就写入到数据，为零跳过
            dat |= 0x01; //数据是从高到低写入，所以右移或上。
        IIC_SCL = 0;
        DelayIIC();
    }    
    return dat;//返回读取的一个字节数据。    
}

