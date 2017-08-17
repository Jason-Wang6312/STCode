#include "STC15Wxx.H"
#include "INTRINS.H"

/********************定义数据*************************/
unsigned char code CODE[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8, 0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e}; //共阳显示数据0-F

sbit SLK = P5^4;
sbit DIO = P5^5;

void TMxxCmd(unsigned char cmd);
void TMxxShow(unsigned char addr,unsigned char dat);
void DelayXms(unsigned int x);
void DelayXus(unsigned int x);

void main(){

    DelayXus(111);
    DelayXms(500);
    TMxxCmd(0x44);
    DelayXms(500);
    
    while(1){
        TMxxShow(0xC0,0x3f);
        TMxxShow(0xC1,0x06);
        TMxxShow(0xC2,0x5b);
        TMxxShow(0xC3,0x4f);
		TMxxCmd(0x8a);
        DelayXms(1500);
    }
}


void DelayXus(unsigned int x)		//@12.000MHz
{
    while(x--){
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

void I2CStart(){
	SLK = 1;
    DIO = 1;
    DelayXus(5);
    DIO = 0;
	SLK = 0;
}

void I2CEnd(){
    SLK = 0;
    DelayXus(5);
    DIO = 0;
    DelayXus(5);
    SLK = 1;
    DelayXus(5);
    DIO = 1;    
}

bit I2CAck(){
    bit ack;
    
    SLK = 0;
    DelayXus(5);
    ack = DIO;
    SLK = 1;
    DelayXus(5);
    SLK = 0;
    return ack;
}

void I2CWriteByte(unsigned char dat){
    unsigned char i;
    
    for(i=0;i<8;i++){
        SLK = 0;
        dat >>= 1;
        DIO = CY;
        DelayXus(5);
        SLK = 1;
        DelayXus(5);
    }
}


void TMxxCmd(unsigned char cmd){
    I2CStart();
    I2CWriteByte(cmd);
    I2CEnd();
}

void TMxxShow(unsigned char addr,unsigned char dat){
    I2CStart();
    I2CWriteByte(addr);
    I2CAck();
    I2CWriteByte(dat);
    I2CAck();
    I2CEnd();
}

void DelayXms(unsigned int x)		//@12.000MHz
{
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
