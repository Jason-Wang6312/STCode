#include "oled-ssd1306.h"

//在MCU里开辟一个OLED的显存空间
//以二维数组方式存放
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
u8 OLED_GRAM[128][8];

/**
 * OLED写数据命令
 * Author:MaWei
 * date:2017-10-12
 * url:http:://www.mawei.live
**/
void OLED_WR_Byte(u8 dat,u8 cmd){
    u8 i=0;
    
    OLED_DC = cmd;//写命令
    OLED_CS = 0; //拉低片选，准备写入
    for(;i<8;i++){
        OLED_SCLK = 0;
        if(dat&0x80)  //高位在前，低位在后
            OLED_SDIN = 1;
        else 
            OLED_SDIN = 0;
        OLED_SCLK = 1;
        dat <<= 1;
    }
    OLED_CS = 1;//拉高片选，写入结束
    OLED_DC = 1;//复位数据命令IO口
}

/**
 * 在指定位置写入点
 * x:0~127 y:0~7 st: 0->清除 1->点亮
 * Author:MaWei
 * date:2017-10-12
 * url:http:://www.mawei.live
**/
void OLED_WR_Point(u8 x,u8 y,u8 st){
    u8 pos,bx,temp=0;
    
    if(x>127 || y>63) return ;//检查是否超出范围
    pos = 7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(st)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

void OLED_Refresh_Gram(){

}

/**
 * 在指定位置对角方形填充
 * x1,y1,x2,y2 方形对角坐标
 * 确保 x1<=x2,y1<=y2, 0<=x1<=127 0<=y1<=63
 * st: 0->清除 1->点亮
 * Author:MaWei
 * date:2017-10-12
 * url:http:://www.mawei.live
**/
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 st){
    u8 x,y;
    for(x=x1;x<=x2;x++){
        for(y=y1;y<=y2;y++){
            OLED_WR_Point(x,y,st);
        }
    }
    OLED_Refresh_Gram();//更新显示
}

/**
 * OLED 初始化
 * Author:MaWei
 * date:2017-10-12
 * url:http:://www.mawei.live
**/
void OLED_Init(){
    //初始化结构体变量
    GPIO_InitTypeDef GPIOB_InitStruct;
    
    //GPIO 时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    //GPIO IO初始化
    GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
    GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOB_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIOB_InitStruct);
    
    
}

