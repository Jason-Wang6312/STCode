#ifndef __DS1302_H
#define __DS1302_H
#include "INTRINS.H"
#include "STC15Wxx.H"
//#include "STC12C5A.H"

sbit DS1302_IO = P3^3;
sbit DS1302_CE = P5^5;
sbit DS1302_CL = P3^2;

//定义变量
extern unsigned char time[7];

//声明函数
void DS1302BurstWrite();//利用ds1302的突发模式一次读取八个字节
void DS1302BurstRead(unsigned char leg);//利用ds1302的突发模式一次写入八个字节
unsigned char DS1302SingleRead(unsigned char addr);//读取ds1302上的某个寄存器地址值
void DS1302SingleWrite(unsigned char addr,unsigned char val);//给ds1302上的某个寄存器地址写入值

#endif