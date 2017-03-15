#ifndef __Infrared_H
#define __Infrared_H

#include "conf.h"

/**
此程序需要占有定显示器1，和外部中断0
******/

/*******/

sbit IR_IO = P3^2;          // IR管脚 任意IO

void Timer0Init(void);

#endif
