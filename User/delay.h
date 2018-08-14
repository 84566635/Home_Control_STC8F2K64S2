#ifndef _DELAY_H
#define _DELAY_H


#include "MyConfig.h"

extern uint16_t Device_Online_Count;

void Timer1_Delay10ms(uint16_t Delay_ms);
void Device_State_Jugement(void);
void Timer1_Init(void);

#endif