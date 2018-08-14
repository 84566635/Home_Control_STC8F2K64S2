#include "STC8F2K16S2.H"
#include "io.h"
#include "relay.h"
#include "rs485.h"
#include "watchdog.h"
#include "delay.h"


void main(void)
{
	RS485_Init();
	Timer1_Init();
	Relay_Init();
	WatchDog_Init();
	while (1)
	{	
		IO_Enable_Scan();
		IO_Input_Scan();
    IO_RS485_Addr_Scan();				
		WatchDog_Feed();		
	}
}


