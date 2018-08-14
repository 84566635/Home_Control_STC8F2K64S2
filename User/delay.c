#include "delay.h"
#include "io.h"

uint16_t Timing_Count = 0;
uint16_t Device_Online_Count = 0;


void Timer1_Init(void) //1ms
{
	AUXR &= 0xbf;		//?????1T??
	TMOD &= 0x0F;		//???????
	TL1 = 0x00;		//??????
	TH1 = 0xB8;		//??????
	
	TF1 = 0;		//??TF1??
	ET1 = 1;
	EA = 1;
	TR1 = 1;		//???1????
}

void Device_State_Jugement(void)
{
  if(Device_Online_Count)
  {
		Device_State = Online;
	  Device_Online_Count--;
  }
  else
  {
	  Device_State = Offline;	 
  }
}


void Timer1_Delay10ms(uint16_t Delay_ms)
{
  Timing_Count = Delay_ms;
	while(Timing_Count);
}


void TM1_Isr() interrupt 3 using 1
{
	Device_State_Jugement();
	if(Timing_Count)
	{
	  Timing_Count--;
	}
}


