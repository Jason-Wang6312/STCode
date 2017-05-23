#include "STC12C5A.H"
#include "lcd1602.h"

void Delay300ms()		//@12.000MHz
{
	unsigned char i, j, k;

	i = 14;
	j = 174;
	k = 224;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void main(){
    unsigned char str[] = "MaWei";
    
    
    InitLcd1602();
    Delay300ms();
    
    while(1){
        LcdShowStr(0,0,&str);
    }
}
