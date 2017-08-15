#include "LED.h"

//LED初始化配置
void LED_Init(void){
    GPIO_InitTypeDef GPIO_LED_InitStruct;//设置结构体 
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//使能外设
    
    GPIO_LED_InitStruct.GPIO_Mode = GPIO_Mode_OUT; //输出模式
    GPIO_LED_InitStruct.GPIO_OType = GPIO_OType_PP;//输出类型为推挽输出
    GPIO_LED_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10; //操作的IO
    GPIO_LED_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//上下拉
    GPIO_LED_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;//速度设置
    
    GPIO_Init(GPIOF,&GPIO_LED_InitStruct);
}

//LED寄存器初始化配置
void LED_SFR(void){
    RCC->AHB1ENR = 1 << 5;//使能外设
    
    GPIOF->MODER &= ~(3 << (2 * 9));//格式化IO9口
    GPIOF->MODER |= 1 << (2 *9);//设置IO9口为输出模式
    GPIOF->OSPEEDR &= ~(3 << (2 * 9));//格式化IO9口
    GPIOF->OSPEEDR |= 3 << (2 * 9); //配置速度为100MHZ 
    GPIOF->PUPDR &= ~(3 << (2 * 9));//格式化IO9口
    GPIOF->PUPDR |= 1 << (2 *9);;//配置为下拉
    GPIOF->OTYPER &= ~(1 << 9);
    GPIOF->OTYPER |= 0 << 9;
    
    GPIOF->ODR |= 1<<9;
}



