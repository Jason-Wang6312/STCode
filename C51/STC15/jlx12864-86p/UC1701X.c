#include "UC1701X.h"
#include "ASCII.h"

//SPI延时
void SPI_Daley(unsigned int n){
    unsigned char i, j;
    while(n--){
        i = 12;
        j = 170;
        do
        {
            while (--j);
        } while (--i);
    }
}

//SPI写入指令到LCD12874
void SPI_WriteCmd(unsigned char cmd){
    unsigned char i;
    
    SPI_CS = 0; //拉低片选
    SPI_RS = 0; //拉低数据、指令控制线， H:写指令，L:写数据
    for(i = 0;i < 8;i++){
        SPI_SCLK = 0; //拉低时钟、准备数据
        cmd <<= 1;
        SPI_SDA = CY; //写入命令当前位状态
        SPI_SCLK = 1; //拉高时钟线让UC1702读取数据    
    }
}

//SPI写数据到LCD12864
void SPI_WriteData(unsigned char dat){
    unsigned char i;
    
    SPI_CS = 0;//拉低片选
    SPI_RS = 1;//拉高数据、指令控制线， H:写指令，L:写数据
    for(i = 0;i < 8;i++){ //写入数据
        SPI_SCLK = 0; //拉低时钟、准备数据
        dat <<= 1; 
        SPI_SDA = CY; //写入数据当前位状态
        SPI_SCLK = 1; //拉高时钟线让UC1702读取数据        
    }  
}

//设置LCD地址
void location(unsigned char page,unsigned char col){
    SPI_CS = 0;//拉低片选
    SPI_WriteCmd((0xB0 + page)); //每8行为一页，总共64行，共八页，从0页到7页，范围：0xB0~0xB7。
    SPI_WriteCmd((0x10+((col >> 4)&0x0F))); //列高位
    SPI_WriteCmd(col&0x0F); //列低位
}

//清屏
void clearScreen(){
    unsigned char i,j;
    SPI_CS = 0;
    for(i = 0;i < 8;i++){
        location(i,0);
        for(j = 0;j < 132;j++){
            SPI_WriteData(0x00);
        }
    }
    SPI_CS = 1;
}

//全屏填充
void fullDisplay(){
    unsigned char i,j;
    SPI_CS = 0;
    for(i = 0;i < 8;i++){
        location(i,i);
        for(j = 0;j < 128;j++){
            SPI_WriteData(0xFF);
        }
    }
    SPI_CS = 1;
}

//显示8x16 点阵图像、汉字
void graphic_8x16(unsigned char page,unsigned char col,unsigned char *dp){
    unsigned char i,j;
    SPI_CS = 0;
    for(j = 0;j < 2;j++){
        location((page+j),col);
        for(i = 0;i < 8;i++){
            SPI_WriteData(*dp);
            dp++;
        }
    }
    SPI_CS = 1;
}

//显示16x16 点阵图像、汉字
void graphic_16x16(unsigned char page,unsigned char col,unsigned char *dp){
    unsigned char i,j;
    SPI_CS = 0;
    for(j = 0;j < 2;j++){
        location((page+j),col);
        for(i = 0;i < 16;i++){
            SPI_WriteData(*dp);
            dp++;
        }
    }
    SPI_CS = 1;
}

//显示32x32 点阵图像、汉字
void graphic_32x32(unsigned char page,unsigned char col,unsigned char *dp){
    unsigned char i,j;
    SPI_CS = 0;
    for(j = 0;j < 4;j++){
        location((page+j),col);
        for(i = 0;i < 31;i++){
            SPI_WriteData(*dp);
            dp++;
        }
    }
    SPI_CS = 1;
}

//显示ascii字符 5x7
void displayStr_5x7(unsigned char page,unsigned char col,unsigned char *str){
    unsigned char i,j,k;
    SPI_CS = 0;
    while(str[i] > 0x00){
        if((str[i] >= 0x20)&&(str[i] <= 0x7E)){
            j=str[i]-0x20;
            location(page,col);
            for(k=0;k<5;k++){
                SPI_WriteData(ASCII_5x7[j][k]);
            }
            i++;
            col += 6;
        }else
            i++;
    }
}

//显示ascii字符 8x16
void displayStr_8x16(unsigned char page,unsigned char col,unsigned char *str){
    unsigned char i,j,k,n;
    SPI_CS = 0;
    while(str[i] > 0x00){
        if((str[i] >= 0x20)&&(str[i] <= 0x7E)){
            j=str[i]-0x20;
            for(n = 0;n < 2;n++){
                location(page+n,col);
                for(k = 0;k < 8;k++){
                    SPI_WriteData(ASCII_8x16[j][(k + 8*n)]);
                }
            }
            i++;
            col += 8;
        }else
            i++;
    }
}

//uc1701x 显示屏初始化
void initial_LCD(){
    LEDA = 1;
    SPI_REST = 0; //硬件复位开始
    SPI_Daley(100);
    SPI_REST = 1; //硬件复位完成 
    
    SPI_Daley(20);
    SPI_WriteCmd(0xE2); //升压1
    SPI_Daley(5);
    SPI_WriteCmd(0x2C); //升压1
    SPI_Daley(5);
    SPI_WriteCmd(0x2E); //升压1
    SPI_Daley(5);
    SPI_WriteCmd(0x2F); //升压1
    SPI_Daley(5);
    SPI_WriteCmd(0x23); //粗调对比度，可设置范围 0x20 ~ 0x27
    SPI_WriteCmd(0x81); //微调对比度
    SPI_WriteCmd(0x28); //0x1A,微调对比度的值，可设置范围 0x00～0x3F
    SPI_WriteCmd(0xA2); //1/9 偏压比（bias）
    SPI_WriteCmd(0xC8); //行扫描顺序：从上到下
    SPI_WriteCmd(0xA0); //列扫描顺序：从左到右
//    if(LCDSize)
//        SPI_WriteCmd(0x60); //起始行：第二行开始
//    else
        SPI_WriteCmd(0x40); //起始行：第一行开始
    SPI_WriteCmd(0xAF); //开显示
    SPI_Daley(100);
    clearScreen();
//    clearXYArea(0,0,128,64); //清屏
    SPI_Daley(10);
    
}
