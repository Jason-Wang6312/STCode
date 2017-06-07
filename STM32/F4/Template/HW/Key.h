#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx.h"
#include "delay.h"

#define KeyUp       GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KeyDown     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KeyLeft     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define KeyRight    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)

void Key_Init_Fun(void);
u8 Key_Scan(u8 mode);

#endif

