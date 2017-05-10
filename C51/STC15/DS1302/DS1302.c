#include "ds1302.h"

//定义时间初始值
unsigned char time[7] = {0x00,0x16,0x18,0x19,0x04,0x06,0x17};

//iic通信开始信号
void IIC_Start(){
	DS1302_CE = 0;
    DS1302_CL = 0;
    DS1302_IO = 0;
    _nop_();
    _nop_();
    _nop_();
    DS1302_CE = 1;
}

//iic通信结束信号
void IIC_End(){
	DS1302_CE = 0;
    _nop_();
    DS1302_CL = 1;
    _nop_();
    DS1302_IO = 1;
}

//在iic信道上写入ds1302的寄存器地址
void DS1302ByteWrite(unsigned char dat){
    unsigned char i;

    for(i=0;i<8;i++){ //循环写入八位地址位
        dat >>= 1;
        DS1302_IO = CY;
        DS1302_CL = 1;
        DS1302_CL = 0;
    }
}

//在iic信道上读取ds1302的寄存器值
unsigned char DS1302ByteRead(){
    unsigned char i,dat = 0x00;   

    for(i=0;i<8;i++){   //循环读取八位数据位
        if(DS1302_IO){
            dat |= 1 << i;
        }
        DS1302_CL = 1;
        DS1302_CL = 0;
    }
    return dat;
}

//利用ds1302的突发模式一次读取八个字节
void DS1302BurstRead(unsigned char leg){
    unsigned char i;     

    IIC_Start(); //在iic信道上写入开始信号
    DS1302ByteWrite(0xBF);//写入ds1302突发读命令
    for(i=0;i < leg;i++){ //循环读取leg个字节，小于八个字节
        time[i] = DS1302ByteRead(); //读取ds1302的值
    }
    IIC_End(); //在iic信道上写入结束信号
}

//利用ds1302的突发模式一次写入八个字节
void DS1302BurstWrite(){
    unsigned char i;
       
    IIC_Start();//在iic信道上写入开始信号
    DS1302ByteWrite(0xBE);//写入ds1302突发写命令
    for(i=0;i < 8;i++){//循环写入八个字节
        DS1302ByteWrite(time[i]); //给ds1302的寄存器写入值
    }
    IIC_End();//在iic信道上写入结束信号
}

//读取ds1302上的某个寄存器地址值
unsigned char DS1302SingleRead(unsigned char addr){
    unsigned char dat;
    
    IIC_Start();//在iic信道上写入开始信号
    DS1302ByteWrite(addr);
    dat = DS1302ByteRead();//读取该寄存器地址的值
    IIC_End();//在iic信道上写入结束信号
    
    return dat;
}

//给ds1302上的某个寄存器地址写入值
void DS1302SingleWrite(unsigned char addr,unsigned char val){  
    IIC_Start();//在iic信道上写入开始信号
    DS1302ByteWrite(addr);//写入ds1302寄存器地址
    DS1302ByteWrite(val);//给该寄存器地址写入值
    IIC_End();//在iic信道上写入结束信号
}
