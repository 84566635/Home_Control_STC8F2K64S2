#ifndef __EEPROM_H
#define	__EEPROM_H

#include "STC15W408AS.H"
#include "MyConfig.h"

#define CMD_IDLE    0               
#define CMD_READ    1               
#define CMD_PROGRAM 2               
#define CMD_ERASE   3              

#define IAP_ADDRESS   0x1300


#define ENABLE_IAP 0x80          


void IapIdle(void);
void EraseSector(u16 addr);
void WriteByte(u16 addr,u8 dat);
u8 ReadByte(u16 addr);
void Write_address(void);

#endif
