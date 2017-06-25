#include "Beep.h"

//beep 库函数初始化
void Beep_Init_Fun(void){
    GPIO_InitTypeDef GPIO_Beep_InitStruct;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
    
    GPIO_Beep_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_Beep_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Beep_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_Beep_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Beep_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    
    GPIO_Init(GPIOF,&GPIO_Beep_InitStruct);
//    GPIO_SetBits();
}
//beep 寄存器初始化
void Beep_Init_SRF(void){

    RCC->AHB1ENR = 1 << 5;//使能F组IO
    
    //设置输出
    GPIOF->MODER &= ~(3 << (2*8));//格式化F8
    GPIOF->MODER |= 1 << (2*8);//设置为普通输出模式
    //设置输出为推挽模式
    GPIOF->OTYPER &= ~(1 << 8);
    //设置上下拉
    GPIOF->PUPDR &= ~(3 << (2*8));//格式化F8
    GPIOF->PUPDR |= 2 << (2*8);//下拉
    //速度
    GPIOF->OSPEEDR &= ~(3 << (2*8));//格式化F8
    GPIOF->OSPEEDR |= 1 << (2*8);
    //
    GPIOF->ODR &= ~(1 << 8);
    GPIOF->ODR |= 1 << 8;
}

