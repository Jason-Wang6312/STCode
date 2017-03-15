#include "main.h"

void main(){

}

void delay(uint16 t){
	while(t--){
		_nop_();
	}
}