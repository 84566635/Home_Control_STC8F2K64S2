#include "io.h"
#include "relay.h"
#include "delay.h"
#include "rs485.h"
#include <string.h>


uint8_t IO_Enable_Buffer[8] = {0};
uint8_t IO_Input_Buffer[8] = {0};
uint8_t IO_State_Buffer[2] = {0};
uint8_t IO_Input_Lock[8] = {0};
uint8_t Device_State = Offline;
uint8_t RS485_Addr = 0;
uint8_t Cnt1 = 0;

void IO_RS485_Addr_Scan(void)
{	
	for(Cnt1 = 0;Cnt1 < 6;Cnt1++)
	{
		if(IO_Input_Check(Addr_IO,Cnt1))
		{
			RS485_Addr |=(0x01<<Cnt1);   			
		}
		else
		{
			RS485_Addr &=~(0x01<<Cnt1);			
		}				
	}
}
									 
void IO_Enable_Scan(void)
{
	for(Cnt1 = 0;Cnt1 < 8;Cnt1++)
	{
		IO_Enable_Buffer[Cnt1] = IO_Input_Check(Enable_IO,Cnt1);		
	}
}
										

void IO_Input_Scan(void)
{	
	uint8_t Temp1 = 0,Temp2 = 0;
	
	for(Cnt1 = 0;Cnt1 < 8;Cnt1++)
	{
		Temp1 = IO_Input_Check(Input_IO,Cnt1);
		Timer1_Delay10ms(2);
		Temp2 = IO_Input_Check(Input_IO,Cnt1);
		
		if(Temp1 == Temp2)
		{
			if(Device_State == Offline)
			{
			   if(IO_Input_Buffer[Cnt1] != Temp2)
				 {
					 if(IO_Enable_Buffer[Cnt1])
					 {
	           IO_Control_Relay(Cnt1);
             Timer1_Delay10ms(20);   						 //此处的延时参数和按键翻转的频率有关 
					 }					        					 
				 }
			  IO_Input_Buffer[Cnt1] = IO_Input_Check(Input_IO,Cnt1);										 
			}
			else
			{
				if((IO_Input_Buffer[Cnt1] != Temp1) && (!IO_Input_Lock[Cnt1]))
				{
				   IO_Input_Lock[Cnt1] = 1;
					 IO_Input_Buffer[Cnt1] = Temp2;
				}
			}		
		}	
	}	
}



void IO_State_Convert(void)
{
  IO_State_Buffer[0] |=((IO_Enable_Buffer[0]==0x01?0x01:0x00)<<1
											 |(IO_Enable_Buffer[1]==0x01?0x01:0x00)<<3
											 |(IO_Enable_Buffer[2]==0x01?0x01:0x00)<<5
											 |(IO_Enable_Buffer[3]==0x01?0x01:0x00)<<7);
	
	
  
  IO_State_Buffer[1] |=((IO_Enable_Buffer[4]==0x01?0x01:0x00)<<1
											 |(IO_Enable_Buffer[5]==0x01?0x01:0x00)<<3              
											 |(IO_Enable_Buffer[6]==0x01?0x01:0x00)<<5
											 |(IO_Enable_Buffer[7]==0x01?0x01:0x00)<<7);
	
  IO_State_Buffer[0] |=((IO_Input_Buffer[0]==0x01?0x01:0x00)
											 |(IO_Input_Buffer[1]==0x01?0x01:0x00)<<2
											 |(IO_Input_Buffer[2]==0x01?0x01:0x00)<<4
											 |(IO_Input_Buffer[3]==0x01?0x01:0x00)<<6);
					
  IO_State_Buffer[1] |=((IO_Input_Buffer[4]==0x01?0x01:0x00)
											 |(IO_Input_Buffer[5]==0x01?0x01:0x00)<<2
											 |(IO_Input_Buffer[6]==0x01?0x01:0x00)<<4
											 |(IO_Input_Buffer[7]==0x01?0x01:0x00)<<6);	
}


uint8_t IO_Input_Check(uint8_t Scan_type,uint8_t Scan_Num)
{
	uint8_t temp = 0;

	switch(Scan_type)
	{
		case Input_IO:
			switch(Scan_Num+1)
			{
				case 0x01:
					temp = IO1;
					break;
				case 0x02:
					temp = IO2;					
					break;
				case 0x03:
					temp = IO3;
					break;
				case 0x04:
					temp = IO4;
					break;
				case 0x05:
					temp = IO5;
					break;
				case 0x06:
					temp = IO6;
					break;
				case 0x07:
					temp = IO7;
					break;
				case 0x08:
					temp = IO8;
					break;				
			}
			break;
		case Enable_IO:
			switch(Scan_Num+1)
			{
				case 0x01:
					temp = IO1_En;
					break;
				case 0x02:
					temp = IO2_En;					
					break;
				case 0x03:
					temp = IO3_En;
					break;
				case 0x04:
					temp = IO4_En;
					break;
				case 0x05:
					temp = IO5_En;
					break;
				case 0x06:
					temp = IO6_En;
					break;
				case 0x07:
					temp = IO7_En;
					break;
				case 0x08:
					temp = IO8_En;
					break;				
			}
			break;			
		case Addr_IO:
			switch(Scan_Num+1)				
			{
				case 0x01:
					temp = Addr1;
					break;
				case 0x02:
					temp = Addr2;					
					break;
				case 0x03:
					temp = Addr3;
					break;
				case 0x04:
					temp = Addr4;
					break;
				case 0x05:
					temp = Addr5;
					break;
				case 0x06:
					temp = Addr6;
					break;						
			}
			break;
		
	}
	temp = (temp == 0x01?0x00:0x01);
	return temp;
}

