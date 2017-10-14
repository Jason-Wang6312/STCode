#include "jlx.h"
#include "ASCII.h"


#define CS_H    GPIO_SetBits(GPIOB,GPIO_Pin_3);
#define CS_L    GPIO_ResetBits(GPIOB,GPIO_Pin_3);
#define RST_H   GPIO_SetBits(GPIOB,GPIO_Pin_4);
#define RST_L   GPIO_ResetBits(GPIOB,GPIO_Pin_4);
#define RS_H    GPIO_SetBits(GPIOB,GPIO_Pin_5);
#define RS_L    GPIO_ResetBits(GPIOB,GPIO_Pin_5);
#define SDA_H   GPIO_SetBits(GPIOB,GPIO_Pin_6);
#define SDA_L   GPIO_ResetBits(GPIOB,GPIO_Pin_6);
#define SLK_H   GPIO_SetBits(GPIOB,GPIO_Pin_7);
#define SLK_L   GPIO_ResetBits(GPIOB,GPIO_Pin_7);

u8 num9[]={
/*-- 文字: 9 --*/
/*-- 宋体 12; 此字体下对应的点阵为：宽 x 高=8x16 --*/
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00
};

//写入命令
void JLXCmdByte(u8 cmd){
    u8 i = 0;
    
    CS_L;
    RS_L;
    for(;i<8;i++){
        SLK_L;
        if(cmd&0x80){
            SDA_H;
        }else{
            SDA_L;
        }
        SLK_H;
        delay_us(5);
        cmd = cmd <<1;
    }  
}

//写入数据
void JLXWriteByte(u8 dat){
    u8 i = 0;
    
    CS_L;
    RS_H;
    for(;i<8;i++){
        SLK_L;
        if(dat&0x80){
            SDA_H;
        }else{
            SDA_L;
        }
        SLK_H;
        delay_us(5);
        dat = dat << 1;
    }
}

//写入地址
void JLXWriteAddr(u8 page,u8 column){
    CS_L;//拉低片选
    
    JLXCmdByte((0xB0+page)); //写入页码,每8行为一页，总共64行，共八页，从0页到7页，范围：0xB0~0xB7。
    JLXCmdByte(((column >> 4)&0x0F)+0x10); //写入列地址高4位
    JLXCmdByte(column&0x0F); //写入列地址低4位
}

void JLXClearScreen(){
    u8 i=0,j=0;
    
    CS_L;
    for(;i<8;i++){
        JLXWriteAddr(i,0);
        for(;j<132;j++){
            JLXWriteByte(0x00);
        }
    }
}

void displayStr(u8 page,u8 col,u8 *str){
    u8 i=0,j,k;
    
    CS_L;
    while(str[i] > 0x00){
        if((str[i] >= 0x20)&&(str[i] <= 0x7E)){
            j=str[i]-0x20;
            JLXWriteAddr(page,col);
            for(k=0;k<5;k++){
                JLXWriteByte(ASCII_5x7[j][k]);
            }
            i++;
            col += 6;
        }else
            i++;
    }
}

void display_graphic_8x16(u8 page,u8 column,u8 *dp)
{
    u8 i,j;
    
    CS_L; 
    for(j=0;j<2;j++)
    {
        JLXWriteAddr(page+j,column);
        for (i=0;i<8;i++)
        { 
            JLXWriteByte(*dp);  /*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
            dp++;
        }
    }
    CS_H;
}

void GPIOInit(){
    //GPIOB初始化结构体声明
    GPIO_InitTypeDef GPIO_InitStruct;
    
    //使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    //GPIOB初始化设置
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
//    GPIO_Write(GPIOB,0XFFFF);
}

//晶联讯初始设置
void JLX1353PNInit(){
    //JLX初始化设置
    CS_L;
    RST_L;
    //JLX硬复位
    delay_ms(100);
    RST_H;
    delay_ms(100);
    //JLX软复位
    JLXCmdByte(0xE2);
    //JLX升压
    JLXCmdByte(0x2C);
    delay_ms(5);
    JLXCmdByte(0x2E);
    delay_ms(5);
    JLXCmdByte(0x2F);
    delay_ms(5);
    //JLX对比度设置
    JLXCmdByte(0x23);//粗调 0x20~0x27
    JLXCmdByte(0x81);//微调
    JLXCmdByte(0x28);//设置 0x00~0x3F
    JLXCmdByte(0xA2);//偏压比设置
    JLXCmdByte(0xC8);//行扫描顺序：从上到下
    JLXCmdByte(0xA0);//列扫描顺序：从左到右
    JLXCmdByte(0x40);//设置第一行开始
    JLXCmdByte(0xAF);//开启显示
    //关闭片选
    CS_H;
}


int main(){
    delay_init();
    
    GPIOInit();
    delay_ms(1000);
    
    JLX1353PNInit();   
    delay_ms(1000);
    
    JLXClearScreen();
    delay_ms(1000);
//    displayStr(0,0,"display");
    
    display_graphic_8x16(2,1,num9);
    
    while(1){
        GPIO_SetBits(GPIOB,GPIO_Pin_2);
        delay_ms(500);
        GPIO_ResetBits(GPIOB,GPIO_Pin_2);
        delay_ms(500);
    };
}

