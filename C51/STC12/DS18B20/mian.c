#include "STC12C5A.H"
#include "DS18B20.H"
#include "LCD1602.H"

unsigned int temp;
unsigned int cnt = 0;

void Time0Init();
void displayTemp();

void mian(){
    unsigned char Tstr[] = "T:00.00";
    
    Time0Init();
    DS18B20Init();
    LCDInit();

    EA = 1;
    while(cnt < 1000) cnt=0;
    
    LCDShowStr(0,1,Tstr);
    
    while(1){
        if(cnt > 1000){
			cnt = 0;
//            LCDShowStr(0,1,Tstr);
//			displayTemp();
		}
    }
}

void displayTemp(){
    unsigned int temp;
    unsigned char Tstr[] = "00.00";

    temp = getTemp();
    temp = temp * 0.0625*100+0.5;
    
    Tstr[0] = (temp % 10000 / 1000) + '0';
	Tstr[1] = (temp % 1000 / 100) + '0';
	Tstr[3] = (temp % 100 / 10) + '0';
	Tstr[4] = (temp % 10) + '0';
	
	LCDShowStr(10,1,Tstr);
}

void Time0Init(){
	AUXR |= 0x40;
	TMOD &= 0x0F;
	TMOD |= 0x10;
	TL1 = 0x20;
	TH1 = 0xD1;
	TF1 = 0;
	TR1 = 1;
	ET1 = 1; 
}

void time0() interrupt 3{
	TL1 = 0x20;
	TH1 = 0xD1;
	cnt++;
}

