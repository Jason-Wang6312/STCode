#include "STC15F2K.H"
#include "INTRINS.H"

typedef unsigned char u8;
typedef unsigned int u16;

unsigned char data adcData _at_ 0x31;
bit busy = 0;
bit adc_ok = 0;

void sendData(u8 dat);
void sendStr(u8 *str);
void UartInit();
void delayX1ms(u16 t);
void delay1us(u16 t);
void sendData(u8 dat);

void main () {
    unsigned long i;
    unsigned char status;
    
    EA = 1;
    UartInit();   

	P1ASF = 0x10 ;              //设置Pl.4为模拟量输入功能
	ADC_CONTR= 0x84;        		//打开A/D转换电源，设置输入通道
	for (i = 0;i <10000; i++ );	//延时
	while(1){
		ADC_CONTR=0x8c;        		//启动A/D转换
		_nop_();_nop_();_nop_();_nop_();
		status = 0;
		while(status==0){        	//等待A/D转换结束
			status=ADC_CONTR&0x10;
		}
		ADC_CONTR = 0x84;        	//清A/D转换结束标志位
		//adc_data = ADC_RES;    		//保存A/D转换结果
		sendData(ADC_RES);        		//送P2口LED灯显示
	}
}

void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}

void delayX1ms(u16 t){
    while(t--){
        Delay1ms();
    }
}

void delay1us(u16 t){
    while(t--){
        _nop_();
        _nop_();
        _nop_();
    }
}

void UartInit(void)		//9600bps@18.432MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x20;		//设定定时初值
	TH1 = 0xFE;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
    ES = 1;
}

void sendData(u8 dat){
    while(busy);
    busy = 1;
    SBUF = dat;
}

void sendStr(u8 *str){
    while(*str){
        sendData(*str++);
    }
}

void Uart() interrupt 4{
	if(RI){
		RI = 0;
	}
	
	if(TI){
		TI = 0;
        busy = 0;
	}
}
