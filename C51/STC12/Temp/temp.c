#include "STC12C5A.h"
#include "intrins.h"
#include "STDIO.H"

sbit DS18B20IO = P3^7;

void dealyX1us(unsigned int t){
	while(t--){
		_nop_();
	}
}

bit DS18B20Check(){
	bit ack = 1;
	
	DS18B20IO = 0;
	dealyX1us(500);
	DS18B20IO = 1;
	dealyX1us(60);
	ack = DS18B20IO;
	while(!DS18B20IO);
	
	return !ack;
}

void DS18B20Write(unsigned char dat){
	unsigned char i=0;

	EA = 0;
	for(;i<8;i++){
		DS18B20IO = 0;
		dealyX1us(2);
		if(0x01 & (dat >> i))
			DS18B20IO = 1;
		dealyX1us(60);
		DS18B20IO = 1;
		dealyX1us(1);
	}
	EA = 1;
}

unsigned char DS18B20Read(){
	unsigned char dat=0x00,i = 0;
	
	EA = 0;
	for(;i<8;i++){
		DS18B20IO = 0;
		dealyX1us(2);
		DS18B20IO = 1;
		dealyX1us(2);
		if(DS18B20IO)
			dat |= 1 << i;
		dealyX1us(60);
	}
	EA = 1;
	
	return dat;
}

bit Init18B20(){
	bit ack = 0;
	
	if(DS18B20Check()){
		DS18B20Write(0xCC);
		DS18B20Write(0x44);
		P27 = 0;
		ack = 1;
	}
	
	return ack;
}

int getTemp(){
	unsigned char LSB,MSB;
	int temp;
	
	DS18B20Write(0xCC);
	DS18B20Write(0xBE);
	
	LSB = DS18B20Read();
	MSB = DS18B20Read();
	
	temp = ((int)MSB << 8) + LSB;
	printf("\n Temp:%f5.2",LSB * 0.0625);
	
	return temp;
}


