#include "STC12C5A.H"
#include "LCD12864.H"

#define u8 unsigned char
#define u16 unsigned int

u8 str[] = "MaWei TEST";
u16 sec = 1000;

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

void main(){
    LCD_Init();
    
    while(1){
        LCDWriteStr(1,1,str);
        while(sec--){
            Delay1ms();
        }
        sec = 1000;
    }
}