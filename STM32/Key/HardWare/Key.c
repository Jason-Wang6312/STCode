#include "Key.h"

//Key 库函数初始化
void Key_Init_Fun(void){
    GPIO_InitTypeDef GPIO_Key_InitStruct;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA,ENABLE);
    //初始化KeyDown,KeyLeft,KeyRight
    GPIO_Key_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Key_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_Key_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Key_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Key_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;    
    GPIO_Init(GPIOE,&GPIO_Key_InitStruct);
    //初始化KeyUp
    GPIO_Key_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Key_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA,&GPIO_Key_InitStruct);
}

//Key 寄存器初始化
void Key_Init_SRF(void){
    

}

//按键扫描
u8 Key_Scan(u8 mode){
    static u8 key_status = 1;
    
    if(mode) key_status =1;
    if(key_status &&(KeyUp==1||KeyDown==0||KeyLeft==0||KeyRight==0)){
        delay_ms(10);
        if(KeyDown == 0){
            return 1;
        }else if(KeyUp == 1){
            return 2;
        }else if(KeyLeft == 0){
            return 3;
        }else if(KeyRight == 0){
            return 4;
        }
    }else if(KeyUp==0||KeyDown==1||KeyLeft==1||KeyRight==1)
        key_status = 1;
    return 0;
}

