/*
	���׵��ӹ�����
	
ʵ��ʮ�� NOkia5110Һ��
db ��zliang 
11/29/2007 8:03PM
��վ��http://www.amy-studio.com/
*/

#include "nokia_5110.h"
#include "bmp_pixel.h"

//***********************************
#define		delay_time	25767


/********************************************************************/


/******************************************************************************/
void main(void) 
{
	LCD_init(); //��ʼ��Һ��    
	LCD_clear();
	
	while(1)  
	{
		LCD_write_english_string(0,0," Welcome to  ");
		LCD_write_english_string(0,1," MaWei  studio ");
		LCD_write_english_string(0,2,"amy-studio.com");
		LCD_write_english_string(0,3," Nokia5110 LCD");
		LCD_write_chinese_string(12,4,12,4,0,5);  

	}	  


}



