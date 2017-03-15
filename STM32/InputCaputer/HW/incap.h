#ifndef __INCAP_H
#define __INCAP_H
#include "sys.h"

extern u8 TIM5InCapStatus;
extern u32 TIM5InCapVal;

void TIM5InCapInit(u32 arr,u16 psc);

#endif
