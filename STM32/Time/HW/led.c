#include "led.h"

/** LED 库函数初始化 **/
void LedFunInit(){
    GPIO_InitTypeDef GPIOF_LED_Struct;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//开启GPIOF时钟
    
    //GPIO初始化
    GPIOF_LED_Struct.GPIO_Mode  = GPIO_Mode_OUT; //输出模式
    GPIOF_LED_Struct.GPIO_OType = GPIO_OType_PP; //推挽输出
    GPIOF_LED_Struct.GPIO_Pin   = GPIO_Pin_9|GPIO_Pin_10; //LED IO 口
    GPIOF_LED_Struct.GPIO_PuPd  = GPIO_PuPd_UP; //上拉
    GPIOF_LED_Struct.GPIO_Speed = GPIO_Speed_50MHz; //时钟频率
    GPIO_Init(GPIOF,&GPIOF_LED_Struct);
}

/** LED 寄存器初始化****/
void LedSrfInit(){
    
    RCC->AHB1ENR = 1 << 5; //开启GPIOF 时钟全能
    
    GPIOF->MODER    &= ~(8 << (2*9)); //输入模式
    GPIOF->OTYPER   &= ~(3 << 9); //推挽输出
    GPIOF->PUPDR    &= ~(8 << (2*9));//复位 F9，F10 口的上下拉
    GPIOF->PUPDR    |= 5 << (2*9); //设置为上拉
    GPIOF->OSPEEDR  &= ~(8 << (2*9));//复位 F9，F10 口时钟
    GPIOF->OSPEEDR  |= 0x0A << (2*9);//设置时钟为 50MHZ
    GPIOF->ODR      |= 3 << 9; //输出使能
}

//led开关
void LedPower(u8 number,u8 Power){
    u16 LedCode = 0;
    switch(number){
        case 0:
            LedCode = GPIO_Pin_9;
            break;
        case 1:
            LedCode = GPIO_Pin_10;
            break;
        case 3:
            LedCode = GPIO_Pin_9|GPIO_Pin_10;
            break;
    }
    if(Power){
        //GPIOF->ODR |= 3 << 8;
        GPIO_SetBits(GPIOF,LedCode);
    }else{
        GPIO_ResetBits(GPIOF,LedCode);
        //GPIOF->ODR &= ~(3 << 8); 
    }
}
