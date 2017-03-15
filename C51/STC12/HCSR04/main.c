#include "STC12C5A.H"
#include "LCD1602.H"
#include "HC_SR04.H"

unsigned int cnt = 0;
bit busy = 0;
//void Time1Init();
void distance();
void UartInit();
extern void sendStr(unsigned char *str);
extern void sendData(unsigned char dat);
void DelayXms(unsigned int t);

void main(){
	unsigned char diststr[] = "D:0.00M";
	
//	LCDInit();
        Timer0Init();
	UartInit();
    sendStr("MaWei\r\n");
    DelayXms(800);

	EA = 1;
	
//	LCDShowStr(0,1,diststr);
	
	while(1){
        sendStr("MaWei\r\n");
        DelayXms(800);
        getDistance();
	}
}

void distance(){
	unsigned int distance;
	unsigned char disstr[] = "0.00";
	
	distance = getDistance();
	disstr[0] = (distance * 0.01) + '0';
	disstr[2] = ((distance % 100) / 10) + '0';
	disstr[3] = (distance % 10) + '0';
	LCDShowStr(2,1,disstr);
}


void UartInit(void){		//4800bps@12.000MHz
	PCON |= 0x80;
	SCON |= 0x52;
	AUXR &= 0xFE;
	TL1 = TH1 = 0xF3;
	TMOD &= 0x0F;
	TMOD |= 0x20;
	ES = 1;
	TR1 = 1;
	ET0 = 0;
}

void T1() interrupt 4{
    if(RI){
        RI = 0;
    }
    if(TI){
        TI = 0;
        busy = 0;
    }
}

void sendData(unsigned char dat){
    while(busy);
    busy = 1;
    SBUF = dat;
}

void sendStr(unsigned char *str){
    while(*str){
        sendData(*str++);
    }
}

void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 12;
	j = 168;
	do
	{
		while (--j);
	} while (--i);
}

void DelayXms(unsigned int t){
	while(t--){
		Delay1ms();
	}
}
