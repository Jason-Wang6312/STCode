#include "STC15Wxx.H"
#include "INTRINS.H"

sbit DHT11_SDA = P0^4;


//初始化
void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x8F;		//设定定时初值
	TH1 = 0xFD;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}

void sendData(unsigned char dat){
    SBUF = dat;
    while(!TI);
    TI = 0;
}

void delayXus(unsigned int x){//@12.000MHz
    while(x--){
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

void delayXms(unsigned int x){//@12.000MHz
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

bit DHT11Init(){   
    unsigned char retry;
    
    DHT11_SDA = 0;
    delayXms(20);
    DHT11_SDA = 1;
    delayXus(20);
    
    while(DHT11_SDA && retry++ < 5){
        delayXus(10);
    }
    if(retry > 5) return 0;
    retry = 0;
    while(!DHT11_SDA && retry++ < 10){
        delayXus(10);
    }
    if(retry > 10) return 0;
    
    return 1;
}

unsigned char DHT11ReadByte(){
    unsigned char i,dat,retry=0;
    
    for(i=0;i<8;i++){
        while(DHT11_SDA && retry++ < 10){
            delayXus(10);
        }
        retry = 0;
        while(!DHT11_SDA && retry++ < 10){
            delayXus(10);
        }
        delayXus(35);
        dat <<= 1;
        if(DHT11_SDA) dat |= 1;        
    }
    return dat;
}

void main(){
    
    EA = 1;
    UartInit();
    delayXms(500);

    while(1){
        DHT11Init();
        sendData(DHT11ReadByte());
        sendData(DHT11ReadByte());
        sendData(DHT11ReadByte());
        sendData(DHT11ReadByte());
        sendData(DHT11ReadByte());
        
        delayXms(2000);
    }
}
