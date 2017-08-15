#include "led.h"

//led初始化设置
void LedInit(){
    
    //定义GPIOD结构体声明
    GPIO_InitTypeDef GPIOD_InitStruct;
    
    //初始化GPIOD时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
    
    //GPIO参数初始化设置
    GPIOD_InitStruct.GPIO_Pin = GPIO_Pin_2; //初始化PD02 IO口
    GPIOD_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //设置速度
    GPIOD_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //设置IO为推挽输出
    GPIO_Init(GPIOD,&GPIOD_InitStruct);
    GPIO_SetBits(GPIOD,GPIO_Pin_2);
}


void LedInitSFR(){

}
