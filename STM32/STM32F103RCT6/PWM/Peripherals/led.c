#include "led.h"

//led初始化设置
void LedInit(){
    
    //定义GPIOD结构体声明
    GPIO_InitTypeDef GPIOA_InitStruct;
    
    //初始化GPIOD时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    //GPIO参数初始化设置
    GPIOA_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_2; //初始化PD02 IO口
    GPIOA_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //设置速度
    GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; //设置IO为推挽输出
    GPIO_Init(GPIOA,&GPIOA_InitStruct);
    GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_2);
}


void LedInitSFR(){

}
