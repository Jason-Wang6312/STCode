#ifndef __KEY_H
#define __KEY_H
#include "sys.h"
#include "delay.h"

#define KeyU GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KeyR GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
#define KeyD GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define KeyL GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)

void KeyInit(void);
u8 KeyScan(u8 isK);

#endif
