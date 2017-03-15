#include "LCD12864.H"

//延时1us倍数
void lcdDelay1Xus(u16 t){
    while(t--){
        _nop_();
        _nop_();
    }
}

//检测忙状态
void isBusy(){
    LCD_Data = 0xFF;
    LCD_CS 	 = 0;
    LCD_RW   = 1;
   	LCD_E    = 1;
    lcdDelay1Xus(5);
   	while(LCD_Data & 0x80); //检测忙信号
   	LCD_E    = 0;
}

//命令写入
void LCDCmd(u8 cmd){
	isBusy();
	LCD_CS	 = 0;
	LCD_RW   = 0;
	LCD_Data = cmd;
	LCD_E    = 1;
	lcdDelay1Xus(100);
	LCD_E    = 0;
}

//显示定位写入
void LCD_Position(u8 x,u8 y){
	u8 position;

	switch(x){
		case 1:
			position = 0x80+y-1; //第一行
			break;
		case 2:
			position = 0x90+y-1; //第二行
			break;
		case 3:
			position = 0x88+y-1; //第三行
			break;
		case 4:
			position = 0x98+y-1; //第四行
			break;
		default:
			break;
	}

	LCDCmd(position); //命令光标定位
}

//写入字符
void LCDWriteChar(u8 str){
	isBusy();
	LCD_CS	 = 1;
	LCD_RW   = 0;
	LCD_Data = str;
	LCD_E    = 1;
	lcdDelay1Xus(100);
	LCD_E    = 0;
}

//字符串写入
void LCDWriteStr(u8 x,u8 y,u8 *str){
	LCD_Position(x,y);
	while(*str){
		LCDWriteChar(*str++);
	}
}

//LCD12864 初始化
void LCD_Init(){
	LCD_PSB = 1;
	LCDCmd(0x30); //显示模式设置,开始要求每次检测忙信号
	LCDCmd(0x01); //清屏
	LCDCmd(0x06); //光标移动设置
	LCDCmd(0x0C); //开及光标设置
}

//LCD 清屏
void LCD_flush(){
	LCDCmd(0x01); //清屏
	LCDCmd(0x34); //
	LCDCmd(0x30); //
}
