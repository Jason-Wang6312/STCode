#include "tm16xx.h"

void DelayXus(unsigned int x){
    while(x--){
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

void TMI2CStart(){
    TM_SCL = 1;
    TM_SDA = 1;
    DelayXus(5);
    TM_SDA = 0;
}

bit TMI2CAck(){
    bit ack;
    TM_SCL = 0;
    DelayXus(5);
    while(TM_SDA);
    TM_SCL = 1;
    DelayXus(5);
    TM_SCL = 0;
    return ack;
}

void TMI2CEnd(){
    TM_SCL = 1;
    TM_SDA = 1;
    DelayXus(5);
    TM_SDA = 0;
    TM_SDA = 1;
}

void TMI2CWriteByte(unsigned char dat){
    unsigned char i;
    for(i=0;i<8;i++){
        TM_SCL = 0;
        dat <<= 1;  
        TM_SDA = CY;        
        TM_SCL = 1;
        DelayXus(5);
    }
}

void TMxxShow(unsigned char addr,unsigned char dat){
    TMI2CStart();
    TMI2CWriteByte(addr);
    P33 = TMI2CAck();
    TMI2CWriteByte(dat);
    P32 = TMI2CAck();
    TMI2CEnd();
}
