#include "lcd1602.h"

void delayXns(){

}

/* 等待液晶准备好 */
void LcdWaitReady(){
    unsigned char sta;
    
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do {
        LCD1602_E = 1;
        sta = LCD1602_DB; //读取状态字
        LCD1602_E = 0;
    } while (sta & 0x80); //bit7等于1表示液晶正忙，重复检测直到其等于0为止
}

/* 向LCD1602液晶写入一字节命令，cmd-待写入命令值 */
void LcdWriteCmd(unsigned char cmd){
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    if(LCD1602_4PIN){ //如果是四线制，再写入低四位
        LCD1602_DB7 = cmd&0x80;
        LCD1602_DB6 = cmd&0x40;
        LCD1602_DB5 = cmd&0x20;
        LCD1602_DB4 = cmd&0x10;
        LCD1602_E  = 1; //使能
        LCD1602_E  = 0; //关闭全能
        LCD1602_DB7 = cmd&0x08;
        LCD1602_DB6 = cmd&0x04;
        LCD1602_DB5 = cmd&0x02;
        LCD1602_DB4 = cmd&0x01;
        LCD1602_E  = 1; //使能
        LCD1602_E  = 0; //关闭全能
    }else{
        LCD1602_DB = cmd; //写入命令
        LCD1602_E  = 1; //使能
        LCD1602_E  = 0; //关闭全能
    }
}

/* 向LCD1602液晶写入一字节数据，dat-待写入数据值 */
void LcdWriteDat(unsigned char dat){
    LcdWaitReady(); //检查LCD1602状态，并准备
    LCD1602_RS = 1; //开启写数据
    LCD1602_RW = 0; //开启写数据
    if(LCD1602_4PIN){
        LCD1602_DB7 = dat&0x80;
        LCD1602_DB6 = dat&0x40;
        LCD1602_DB5 = dat&0x20;
        LCD1602_DB4 = dat&0x10;
        LCD1602_E  = 1; //使能
        LCD1602_E  = 0; //关闭全能
        LCD1602_DB7 = dat&0x08;
        LCD1602_DB6 = dat&0x04;
        LCD1602_DB5 = dat&0x02;
        LCD1602_DB4 = dat&0x01;
        LCD1602_E  = 1; //使能
        LCD1602_E  = 0; //关闭全能
    }else{
        LCD1602_DB = dat; //写入数据
        LCD1602_E  = 1; //使能
        LCD1602_E  = 0; //关闭全能
    }
}

/* 设置显示RAM起始地址，亦即光标位置，(x,y)-对应屏幕上的字符坐标 */
void LcdSetCursor(unsigned char x, unsigned char y){
    unsigned char addr;
    
    if (y == 0)  //由输入的屏幕坐标计算显示RAM的地址
        addr = 0x00 + x;  //第一行字符地址从0x00起始
    else
        addr = 0x40 + x;  //第二行字符地址从0x40起始
    LcdWriteCmd(addr | 0x80);  //设置RAM地址
}

/* 在液晶上显示字符串，(x,y)-对应屏幕上的起始坐标，str-字符串指针 */
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str){
    LcdSetCursor(x, y);   //设置起始地址
    while (*str != '\0'){  //连续写入字符串数据，直到检测到结束符
        LcdWriteDat(*str++);
    }
}

void ledWriteSingeByte(unsigned char x, unsigned char y, unsigned char dat){
	LcdSetCursor(x, y);   //设置起始地址
	LcdWriteDat(dat);
}

/* 打开光标的闪烁效果 */
void LcdOpenCursor(){
	LcdWriteCmd(0x0F);
}

/* 关闭光标显示 */
void LcdCloseCursor(){
	LcdWriteCmd(0x0C);
}

/* 初始化1602液晶 */
void InitLcd1602(){
    if(LCD1602_4PIN){
        LcdWriteCmd(0x28);  //16*2显示，5*7点阵，4位数据接口
    }else{
        LcdWriteCmd(0x38);  //16*2显示，5*7点阵，8位数据接口
    }
    LcdWriteCmd(0x0C);  //显示器开，光标关闭
    LcdWriteCmd(0x06);  //文字不动，地址自动+1
    LcdWriteCmd(0x01);  //清屏
}

