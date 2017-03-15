#include "STC12C5A.H"
#include "STDIO.H"
#include "LCD1602.H"
#include "DS18B20.H"
//#include "HC_SR04.H"
#include "DS1302.H"

unsigned int cnt = 0;
void Time0Init();
void TempDisplay();
//void distance();
void displayTime();

void main(){
	unsigned char tempstr[] = "T:00.00";
	unsigned char disstr[] = "D:0.00M";

	Time0Init();
	LCDInit();
//	HCSR04Init();
	DS1302Init();
	
	EA = 1;
	
	LCDShowStr(8,1,tempstr);
	LCDShowStr(0,1,disstr);
	
	while(1){
		if((cnt%200) == 0){
//			displayTime();
		}
		if(cnt > 1000){
			cnt = 0;
			TempDisplay();
//			distance();
		}
	}
}

void displayTime(){
	unsigned char str[15];
	
	DS1302ReadTime();
	
	str[0] = (DS1302_Time[4] >> 4) + '0';
	str[1] = (DS1302_Time[4] & 0x0F) + '0';
	str[2] = '-';
	str[3] = (DS1302_Time[3] >> 4) + '0';
	str[4] = (DS1302_Time[3] & 0x0F) + '0';
	str[5] = ' ';
	str[6] = (DS1302_Time[2] >> 4) + '0';
	str[7] = (DS1302_Time[2] & 0x0F) + '0';
	str[8] = ':';
	str[9] = (DS1302_Time[1] >> 4) + '0';
	str[10] = (DS1302_Time[1] & 0x0F) + '0';
	str[11] = ':';
	str[12] = (DS1302_Time[0] >> 4) + '0';
	str[13] = (DS1302_Time[0] & 0x0F) + '0';
	
	LCDShowStr(0,0,str);
//	
////	str[0] = '2';
////	str[1] = '0';
////	str[2] = (DS1302_Time[6] >> 4) + '0';
////	str[3] = (DS1302_Time[6] & 0xF0) + '0';
////	str[4] = '-';
////	str[5] = (DS1302_Time[4] >> 4) + '0';
////	str[6] = (DS1302_Time[4] & 0xF0) + '0';
////	str[7] = '-';
////	str[8] = (DS1302_Time[3] >> 4) + '0';
////	str[9] = (DS1302_Time[3] & 0xF0) + '0';
////	str[10] = ' ';
////	str[11] = (DS1302_Time[2] >> 4) + '0';
////	str[12] = (DS1302_Time[2] & 0xF0) + '0';
////	str[13] = ':';
////	str[14] = (DS1302_Time[2] >> 4) + '0';
////	str[15] = (DS1302_Time[2] & 0xF0) + '0';
////	str[16] = ':';
////	str[17] = (DS1302_Time[2] >> 4) + '0';
////	str[18] = (DS1302_Time[2] & 0xF0) + '0';
}

//void distance(){
//	unsigned int distance;
//	unsigned char disstr[] = "0.00";
//	
//	distance = getDistance();
//	disstr[0] = (distance * 0.01) + '0';
//	disstr[2] = ((distance % 100) / 10) + '0';
//	disstr[3] = (distance % 10) + '0';
//	LCDShowStr(2,1,disstr);
//}

void TempDisplay(){
	unsigned int temp;
	unsigned char datas[] = "00.00";
		
	temp = getTemp();
	temp = temp * 0.0625*100+0.5;
	
	datas[0] = (temp % 10000 / 1000) + '0';
	datas[1] = (temp % 1000 / 100) + '0';
	datas[3] = (temp % 100 / 10) + '0';
	datas[4] = (temp % 10) + '0';
	
	LCDShowStr(10,1,datas);
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

