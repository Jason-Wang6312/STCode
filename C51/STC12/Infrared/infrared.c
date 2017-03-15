#include "infrared.h"

/*
	本程序是检测低电平解析
*/


bit guideFlag = 0; //引导状态标记
bit doneFlag = 0; //完成标记
u8 dataCharFlag = 0; //数据字节标记
u8 dataBitFlag = 0; //数据位标记
extern u8 dataCode[4]; //最后解码数据

//NEC红外解码器
void InfInit(){
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xC0;		//设置定时初值
	TH0 = 0xE5;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时
	EX0 = 1; //外部中断开启
	ET0 = 1; //中断溢出开启
	EA = 1; //总中断开启
}

//检测引导码是否正确
void guideCheck(){
	u16 time = 0;

	TH0 = TL0 = 0x00;
	TR0 = 1;
	while(InfIO){
		TR0 = 0;
		time = (TH0 * 256) + TL0;
		//判断引导码，低电平在 4.2ms－4.5ms之间
		//算法：4.2*(1/12)*1000
		if(time > 50400 && time < 57600){ 
			guideFlag = 1;
			dataCharFlag = 0;//复位字节标记
			dataCode[4] = {0x00,0x00,0x00,0x00};//初始化
		}else{
			guideFlag = 0;
		}
	}
	TH0 = TL0 = 0x00;
	return ;
}

//解析红外数据
void analysisData(){
	u16 time = 0;
	if(dataCharFlag < 4){ //字节数判断
		if(dataBitFlag < 7){ //位判断
			TH0 = TL0 = 0x00;
			TR0 = 1;
			while(InfIO){ //判断低电平
				TR0 = 0;
				time = (TH0 * 256) + TL0;
				//数据判断，0：高电平560US+低电平560US 1：高电平560US+低电平1.68MS
				//只判断1
				if(time > 19200 && time < 21600){ 
					dataCode[dataCharFlag] |= 1 << dataBitFlag;
				}
				dataBitFlag++;
				if(dataBitFlag > 7){
					dataBitFlag = 0;
					dataCharFlag ++;
				}
			}
		}else
	}else if(dataCharFlag > 4){
		guideFlag = 0;
		doneFlag = 1;
		dataCharFlag = 0;
	}
	TH0 = TL0 = 0x00;
	return ;
}

//检测数据正确性
u8 checkData(u8 _userCode){
	if(doneFlag){
		if(dataCode[0] == dataCode[1] && dataCode[0] == _userCode){
			if(dataCode[2] == dataCode[3] && dataCode[2] > 0){
				return dataCode[2];
			}
		}else{
			return 0;
		}
	}
	return 0;
}

//中断
void T0() interrupt 0{
	//判断引导码是否检测
	if(guideFlag == 0){
		guideCheck(); //检测引导码
	}elseif(guideFlag){
		analysisData(); //数据解析
	}

}


