#include "STC15Wxx.H"
#include "INTRINS.H"

/********************定义数据*************************/
unsigned char code aa[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8, 0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e}; //共阳显示数据0-F

sbit SLK = P5^4;
sbit DIO = P5^5;

void main(){

	while(1);
}