#include "led.h"


u8 Cont = 0;

/*初始化定时器T0,用来给LED定时，让其以1s的频率闪烁*/
void LED_Timer0_Init(void) //10ms
{
  TMOD &= 0x0f;   //#0000 xxxx  当TR0置位时就启动定时器0
	                //            定时器T0用于内部定时
	                //            [M1M0]=00,定时器T0工作在16位自动重装定时器模式�
	
	TH0 = 0XB8;     //定时器T0初值0xB800,即10ms定时
	TL0 = 0;
	
	TR0 = 1;       //启动定时器T0
	
	ET0 = 1;       //开定时器T0中断
	
	EA = 1;        //开总中断

}

/*使LED灯500ms闪烁1次*/
void Timer0() interrupt 1   
{
	 Cont++;
	 if(Cont > 50)
	 {
		 Cont = 0;
		 LED = !LED;					
   }
}







/**********************************THE END**********************************/ 