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

/*写指令到 LCD 模块*/
void transfer_command(int data1)
{
    char i;
    CS_L;
    RS_L;
    for(i=0;i<8;i++)
    {
        SLK_L;
        if(data1&0x80){
            SDA_H;
        }else{
            SDA_L;
        }
        SLK_H;
        data1=data1<<=1;
    }
}

/*写数据到 LCD 模块*/
void transfer_data(int data1)
{
    char i;
    CS_L;
    RS_H;
    for(i=0;i<8;i++)
    {
        SLK_L;
        if(data1&0x80){
            SDA_H;
        }else{
            SDA_L;
        }
        SLK_H;
        data1=data1<<=1;
    }
}


void lcd_address(u8 page,u8 column)
{
    CS_L;
    column=column-1; //我们平常所说的第 1 列，在 LCD 驱动 IC 里是第 0 列。所以在这里减去1
    page=page-1;
    transfer_command(0xb0+page); //设置页地址。每页是 8 行。一个画面的 64 行被分成 8 个页。我们平常所说的第 1 页，在 LCD 驱动 IC 里是第 0 页，所以在这里减去 1*/
    transfer_command(((column>>4)&0x0f)+0x10);  //设置列地址的高 4 位
    transfer_command(column&0x0f);  //设置列地址的低 4 位
}

//==================display a piture of 128*64 dots================
void full_display(u8 data_left,u8 data_right)
{
    int i,j;
    for(i=0;i<8;i++)
    {
        CS_L;
        lcd_address(i+1,1);
        for(j=0;j<64;j++)
        {
            transfer_data(data_left);
            transfer_data(data_right);
        }
    }
}

/*全屏清屏*/
void clear_screen()
{
    unsigned char i,j;
    CS_L;
    for(i=0;i<9;i++)
    {
        lcd_address(1+i,1);
        for(j=0;j<132;j++)
        {
            transfer_data(0xFF);
        }
    }
    CS_H;
}

void displayStr(u8 page,u8 col,u8 *str){
    u8 i=0,j,k;
    
    CS_L;
    while(str[i] > 0x00){
        if((str[i] >= 0x20)&&(str[i] <= 0x7E)){
            j=str[i]-0x20;
            lcd_address(page,col);
            for(k=0;k<5;k++){
                transfer_data(ASCII_5x7[j][k]);
            }
            i++;
            col += 6;
        }else
            i++;
    }
}

void initial_lcd()
{
    CS_L;
    RST_L; /*低电平复位*/
    delay_ms(100);
    RST_H;  /*复位完毕*/
    delay_ms(20);
    transfer_command(0xe2);  /*软复位*/
    delay_ms(5);
    transfer_command(0x2c); /*升压步聚 1*/
    delay_ms(5); 
    transfer_command(0x2e); /*升压步聚 2*/
    delay_ms(5);
    transfer_command(0x2f); /*升压步聚 3*/
    delay_ms(5);
    transfer_command(0x23); /*粗调对比度，可设置范围 0x20～0x27*/
    transfer_command(0x81); /*微调对比度*/
    transfer_command(0x28); /*0x1a,微调对比度的值，可设置范围 0x00～0x3f*/
    transfer_command(0xa2); /*1/9 偏压比（bias）*/
    transfer_command(0xc8); /*行扫描顺序：从上到下*/
    transfer_command(0xa0); /*列扫描顺序：从左到右*/
    transfer_command(0x40); /*起始行：第一行开始*/
    transfer_command(0xaf); /*开显示*/
    CS_H;
}

void display_graphic_8x16(u8 page,u8 column,u8 *dp)
{
    u8 i,j;
    
    CS_L; 
    for(j=0;j<2;j++)
    {
        lcd_address(page+j,column);
        for (i=0;i<8;i++)
        { 
            transfer_data(*dp);  /*写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1*/
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


int main(){
    delay_init();
    
    GPIOInit();
    delay_ms(1000);
    
    initial_lcd();   
    delay_ms(1000);
    
    clear_screen();
    delay_ms(1000);
//    displayStr(0,0,"display");
    
//    display_graphic_8x16(2,1,num9);
    
    while(1){
        GPIO_SetBits(GPIOB,GPIO_Pin_2);
        delay_ms(500);
        GPIO_ResetBits(GPIOB,GPIO_Pin_2);
        delay_ms(500);
    };
}

