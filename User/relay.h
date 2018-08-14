#ifndef _relay_H_
#define _relay_H_

#include "STC8F2K16S2.H"
#include "MyConfig.h"

extern uint8_t Relay_State;


void Relay_Init(void);


void Cmd_Control_Relay(uint8_t *Cmd);

void IO_Control_Relay(uint8_t Num);

void Relay_State_Conver(void);

void Relay_Control(uint8_t Num,uint8_t Set);















#endif