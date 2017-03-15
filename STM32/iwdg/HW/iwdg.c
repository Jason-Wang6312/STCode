#include "iwdg.h"

//看门狗初始化
void IwdgInit(u8 per,u16 rlr){
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //去掉写保护
    IWDG_SetPrescaler(per); //设置分频系数
    IWDG_SetReload(rlr);//设置重载值
    IWDG_ReloadCounter();//重载
    IWDG_Enable();//开启开门狗
}


