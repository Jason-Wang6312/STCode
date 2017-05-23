#include <reg52.h>

void main(){

}


void INT0(){
    TL0 = TH0 = 0;
    TMOD = 0x06;
    TR0 = 0;
    IT0 = 1;
}

