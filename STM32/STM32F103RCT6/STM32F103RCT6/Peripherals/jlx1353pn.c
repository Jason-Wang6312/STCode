#include "jlx1353pn.h"
#include "ASCII.h"

//写入命令
void JLXCmdByte(u8 cmd){
    u8 i = 0;
    
    JLX_CS = 0;
    JLX_RS = 0;
    for(;i<8;i++){
        JLX_SLK = 0;
        if(cmd&0x80){
            JLX_SDA = 1;
        }else{
            JLX_SDA = 0;
        }
        delay_us(2);
        JLX_SLK = 1;
        delay_us(2);
        cmd <<= 1;
    }  
}

//写入数据
void JLXWriteByte(u8 dat){
    u8 i = 0;
    
    JLX_CS = 0;
    JLX_RS = 1;
    for(;i<8;i++){
        JLX_SLK = 0;
        if(dat&0x80){ //写入从高到低
            JLX_SDA = 1;
        }else{
            JLX_SDA = 0;
        }
        delay_us(2);
        JLX_SLK = 1;
        delay_us(2);
        dat <<= 1;
    } 
}

//写入地址
void JLXWriteAddr(u8 page,u8 column){
    JLX_CS = 0;//拉低片选
    
    JLXCmdByte((0xB0+page)); //写入页码,每8行为一页，总共64行，共八页，从0页到7页，范围：0xB0~0xB7。
    JLXCmdByte(((column >> 4)&0x0F)+0x10); //写入列地址高4位
    JLXCmdByte(column&0x0F); //写入列地址低4位
    
    //JLX_CS = 1;
}

void JLXClearScreen(){
    u8 i=0,j=0;
    JLX_CS = 0;
    for(;i<8;i++){
        JLXWriteAddr(i,i);
        for(;j<132;j++){
            JLXWriteByte(0x00);
        }
    }
}

void displayStr(u8 page,u8 col,u8 *str){
    u8 i,j,k;
    
    JLX_CS = 0;
    while(str[i] > 0){
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

//晶联讯初始设置
void JLX1353PNInit(){
    
    //GPIOB初始化结构体声明
    GPIO_InitTypeDef GPIO_InitStruct;
    
    //使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    //GPIOB初始化设置
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
    GPIO_Write(GPIOB,0XFFFF);
    
    //JLX初始化设置
    JLX_CS = 0;
    JLX_RST = 0;
    //JLX硬复位
    delay_ms(100);
    JLX_RST = 1;
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
    JLX_CS = 1;
}


int main(){
    delay_init();
    
    JLX1353PNInit();
    
    delay_ms(1000);
    
    displayStr(0,0,"displayStr_8x16");
    
    while(1);
}

