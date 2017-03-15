#include "DS1302.H"

u8 code DS1302_Read_Addr[7] = {0x81,0x83,0x85,0x87,0x89,0x8B,0x8D};
u8 code DS1302_Write_Addr[7] = {0x80,0x82,0x84,0x86,0x88,0x8A,0x8C};
u8 DS1302_Time[7] = {0x16,0x07,0x12,0x01,0x30,0x04,0x16};
/***************************
* DS1302 Write Time,Write Commend
***********************/
void DS1302WriteCmd(u8 addr,u8 dat){
	u8 n = 0;
	
	DS1302_CE = 0;
	DS1302_SCLK = 0;
	_nop_();
	_nop_();
	_nop_();
	
	DS1302_CE = 1;
	_nop_();
	
	for(;n < 8;n++){
		DS1302_IO = addr & 0x01;
		addr >>= 1;
		DS1302_SCLK = 1;
		_nop_();
		DS1302_SCLK = 0;
		_nop_();
	}
	
	for(n = 0;n < 8;n++){
		DS1302_IO = dat & 0x01;
		dat >>= 1;
		DS1302_SCLK = 1;
		_nop_();
		DS1302_SCLK = 0;
		_nop_();
	}
	
	DS1302_CE = 0;
	_nop_();
	_nop_();
	_nop_();
}

/***************************
* DS1302 Read Time
***********************/
u8 DS1302Read(u8 addr){
	u8 n=0,dat=0x00;
	
	DS1302_CE = 0;
	DS1302_SCLK = 0;
	_nop_();
	_nop_();
	_nop_();
	
	DS1302_CE = 1;
	_nop_();
	
	for(;n < 8;n++){
		DS1302_IO = addr & 0x01;
		addr >>= 1;
		DS1302_SCLK = 1;
		_nop_();
		DS1302_SCLK = 0;
		_nop_();
	}
	_nop_();
	for(n=0;n < 8;n++){
		if(DS1302_IO)
			dat |= 1 << n;
		DS1302_SCLK = 1;
		_nop_();
		DS1302_SCLK = 0;
		_nop_();
	}
	
	DS1302_CE = 0;
	_nop_();
	DS1302_SCLK = 1;
	_nop_();
	DS1302_IO = 0;
	_nop_();
	DS1302_IO = 1;
	_nop_();
	
	return dat;
}


/***************************
* DS1302 Init
***********************/
void DS1302Init(){
	u8 n=0;
	DS1302WriteCmd(0x8E,0x00);
	for(;n < 7;n++){
		DS1302WriteCmd(DS1302_Write_Addr[n],DS1302_Time[n]);
	}
	DS1302WriteCmd(0x8E,0x80);
}

void DS1302ReadTime(){
	u8 n=0;
	for(;n < 7;n++){
		DS1302_Time[n] = DS1302Read(DS1302_Read_Addr[n]);
	}
	
//	return DS1302_Time;
}
