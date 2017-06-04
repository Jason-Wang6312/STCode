#include "STC15Wxx.H"
#include "eeprom.h"

void DelayXms(unsigned int x);
void SendData(unsigned char data_buf);
void UartInit();

unsigned int sector1 = 0x0000;
unsigned int sector2 = 0x0200;

void main(){
    unsigned char i;
    unsigned char mw[] = "mawei12345";
    unsigned char mw2[] = "abcdefef12";
    
    UartInit();
    DelayXms(500);
    DelayXms(500);
    EepromEraseSector(sector1);
    DelayXms(500);
   
    
    for(i=0;i<10;i++){
        EepromWriteByte(i,mw[i]);
    }
    
    EepromEraseSector(sector2);
    DelayXms(500);
    
    for(i=0;i<10;i++){
        EepromWriteByte((sector2|i),mw2[i]);
    }
    DelayXms(500);
    
    while(1){
        for(i=0;i<10;i++){
            SendData(EepromReadByte(i));
        }
        for(i=0;i<10;i++){
            SendData(EepromReadByte((sector2|i)));
        }   
        DelayXms(500);
        DelayXms(500);
        DelayXms(500);
    }
}

void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x8F;		//设定定时初值
	T2H = 0xFD;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
}

void SendData(unsigned char data_buf) //发送一个字符
{
	SBUF = data_buf;
	while(!TI);//TI是发送成功标志
	TI = 0;
}

void DelayXms(unsigned int x){ //@12.000MHz
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
