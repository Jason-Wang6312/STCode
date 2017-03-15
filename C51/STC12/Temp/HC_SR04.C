#include "HC_SR04.H"

void HCSR04Init(){
//	AUXR |= 0x80;
	AUXR &= 0x7F;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TL0 = 0x00;
	TH0 = 0x00;
	Trig = 0;
}

void delay1us(u16 t){
	while(t--){
		_nop_();
		_nop_();
	}
}

u16 getDistance(){
	u16 distance=0;
	
	TL0 = TH0 = 0x00;
	Trig = 1;
	delay1us(20);
	Trig = 0;
	while(!Echo);
	TR0 = 1;
	while(Echo);
	TR0 = 0;
	
//	if(TF0){
//		return distance;
//	}
	distance = TH0 * 256 + TL0;
	distance *= 58;
	if(distance > 700){
		distance = 0;
		P24 = 0;
	}
	
	return distance;
}

