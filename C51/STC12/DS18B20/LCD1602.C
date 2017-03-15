#include "LCD1602.H"

void LCDReady(){
	u8 stat;
	LCD1602_DB = 0xFF;
	LCD1602_RS = 0;
	LCD1602_RW = 1;
	do{
		LCD1602_E = 1;
		stat = LCD1602_DB;
		LCD1602_E = 0;
	}while(stat & 0x80);
}

void LCDCmd(u8 cmd){
	LCDReady();
	LCD1602_RW = 0;
	LCD1602_RS = 0;
	LCD1602_DB = cmd;
	LCD1602_E = 1;
	LCD1602_E = 0;
}

void LCDWriteDat(u8 dat){
	LCDReady();
	LCD1602_RW = 0;
	LCD1602_RS = 1;
	LCD1602_DB = dat;
	LCD1602_E = 1;
	LCD1602_E = 0;
	
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
