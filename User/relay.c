#include "relay.h"
#include "io.h"

uint8_t Relay_Temp[4] = {0};
uint8_t Relay_State = 0;

			
void Relay_Init(void)
{
	uint8_t Cnt;
	for(Cnt = 0;Cnt < 2;Cnt++)
	{
		Relay_Control(Cnt,OFF);	
	}
}


void Relay_Control(uint8_t Num,uint8_t Set)
{
   switch(Num)
	 {
	   case 0x00:
		   switch(Set)
			 {
			   case Keep:
				   break;
				 case OFF:
					 Relay1 = OFF;
				   Relay_Temp[Num] = OFF;
				   break;
				 case ON:
					 Relay1 = ON;
				   Relay_Temp[Num] = ON;
				   break;
				 case Toggle:
					 if(Relay_Temp[Num])
					 {
						 Relay1 = OFF;
						 Relay_Temp[Num] = OFF;					 
					 }
					 else
					 {
						 Relay1 = ON;
						 Relay_Temp[Num] = ON;					 
					 }
					 break;
					 
				 default:
					 break;
			 }
		   break;
		 case 0x01:
		   switch(Set)
			 {
			   case Keep:
				   break;
				 case OFF:
					 Relay2 = OFF;
				   Relay_Temp[Num] = OFF;
				   break;
				 case ON:
					 Relay2 = ON;
				   Relay_Temp[Num] = ON;
				   break;
				 case Toggle:
					 if(Relay_Temp[Num])
					 {
						 Relay2 = OFF;
						 Relay_Temp[Num] = OFF;					 
					 }
					 else
					 {
						 Relay2 = ON;
						 Relay_Temp[Num] = ON;					 
					 }
					 break;
					 
				 default:
					 break;
			 }			 
			 break;
		 
		 default:
			 break;	 
	 }
}



void Cmd_Control_Relay(uint8_t *Cmd)
{
   switch(Cmd[5])
	 {
	   case 0x01:
			 switch(Cmd[6])
			 {
			   case 0x00:
					 break;
				 case 0x01:
					 Relay_Control(0,OFF);
           break;		 
				 case 0x02:
					 Relay_Control(0,ON);					 
           break;
				 case 0x03:
					 Relay_Control(0,Toggle);
           break;				 
				 
				 default:
					 break;			 
			 }
			 break;
			 
			case 0x02:
				 switch(Cmd[6])
				 {
					 case 0x00:
						 break;
					 case 0x01:
					   Relay_Control(1,OFF);	
             break;					  
					 case 0x02:
					   Relay_Control(1,ON);
             break;					 
					 case 0x03:
					   Relay_Control(1,Toggle);								 
					   break;
					 
					 default:
						 break;			 
				 }				
				break;
			
			default:
				break; 
	 }
}

void IO_Control_Relay(uint8_t Num)
{
  if(Num < Device_Mode)
	{
		Relay_Control(Num,Toggle);				
	}
}

void Relay_State_Conver(void)
{
  Relay_State = (Relay_Temp[0] == 0x01?0x02:0x01)
               |(Relay_Temp[1] == 0x01?0x02:0x01)<<2
               |(Relay_Temp[2] == 0x01?0x02:0x01)<<4
               |(Relay_Temp[3] == 0x01?0x02:0x01)<<6; 
}




