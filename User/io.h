#ifndef _io_H_
#define _io_H_


#include "STC8F2K16S2.H"
#include "MyConfig.h"

extern uint8_t RS485_Addr;
extern uint8_t Device_State;
extern uint8_t IO_State_Buffer[2];
extern uint8_t IO_Input_Lock[8];


void IO_Input_Scan(void);
void IO_Enable_Scan(void);
void IO_RS485_Addr_Scan(void);
void IO_State_Convert(void);
uint8_t IO_Input_Check(uint8_t Scan_type,uint8_t Scan_Num);






















#endif

