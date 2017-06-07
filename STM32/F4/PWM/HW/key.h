#ifndef __KEY_H
#define __KEY_H
#include "stm32f4xx.h"
#include "delay.h"

#define KeyUp       GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KeyDown     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define KeyLeft     GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KeyRight    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)

void KeyFunInit(void);
u8 KeyScan(u8 mode);

#endif
