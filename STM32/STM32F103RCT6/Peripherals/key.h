#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

//定义按键IO口
#define Key0 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);
#define Key1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);

//声明函数
void KeyInit(void); //按键初始化设置
u8 KeyScan(void); //按键扫描

#endif
