#include "STC12C5A.H"

#define u8 unsigned char

#define LCD16_DB P0
sbit LCD16_RW = P2^5;
sbit LCD16_RS = P2^6;
sbit LCD16_E = P2^7;

void LCDReady(){
	u8 stat;
	LCD16_DB = 0xFF;
	LCD16_RS = 0;
	LCD16_RW = 1;
	do{
		LCD16_E = 1;
		stat = LCD16_DB;
		LCD16_E = 0;
	}while(stat & 0x80);
}

void LCDCmd(u8 cmd){
	LCDReady();
	LCD16_RW = 0;
	LCD16_RS = 0;
	LCD16_DB = cmd;
	LCD16_E = 1;
	LCD16_E = 0;
}

void LCDWriteDat(u8 dat){
	LCDReady();
	LCD16_RW = 0;
	LCD16_RS = 1;
	LCD16_DB = dat;
	LCD16_E = 1;
	LCD16_E = 0;
	
}

void LCDSetCursor(u8 x,u8 y){
	u8 addr;
	if(y > 0)
		addr = 0x00 + x;
	else
		addr = 0x40 + x;
	LCDCmd(0x80 | addr);
}

void LCDShowStr(u8 x,u8 y,u8 *str){
	EA = 0;
	LCDSetCursor(x,y);
	while(*str){
		LCDWriteDat(*str++);
	}
	EA = 1;
}

void LCDInit(){
	LCDCmd(0x38);
	LCDCmd(0x0C);
	LCDCmd(0x06);
	LCDCmd(0x01);
}

