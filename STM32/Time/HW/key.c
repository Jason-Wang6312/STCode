#include "key.h"

/****按键初始化**********/
void KeyFunInit(){
    GPIO_InitTypeDef GPIOE_KEY_Struct;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA,ENABLE);//全能E组时钟
    
    //Key0 Key1 Key2初始化
    GPIOE_KEY_Struct.GPIO_Mode  = GPIO_Mode_IN; //输入模式
    //GPIOE_KEY_Struct.GPIO_OType = 1; //读取不用配置
    GPIOE_KEY_Struct.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_4;
    GPIOE_KEY_Struct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIOE_KEY_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE,&GPIOE_KEY_Struct);
    
    GPIOE_KEY_Struct.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIOE_KEY_Struct.GPIO_Pin   = GPIO_Pin_0;
    GPIO_Init(GPIOA,&GPIOE_KEY_Struct);
}

/***按键扫描***/
u8 KeyScan(u8 mode){
    static u8 KeyStatus=1;
    if(mode) KeyStatus = 1;
    if(KeyStatus && (KeyUp || !KeyDown || !KeyLeft || !KeyRight)){
        delay_ms(10);
        KeyStatus = 0;
        if(KeyUp)
            return 1;
        else if(!KeyDown)
            return 2;
        else if(!KeyLeft)
            return 3;
        else if(!KeyRight)
            return 4;
    }else if(!KeyUp && KeyDown && KeyLeft && KeyRight)
        return 1;
    return 0;
}

