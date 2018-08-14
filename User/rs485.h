#ifndef _rs485_H_
#define _rs485_H_

#include "STC8F2K16S2.H"
#include "MyConfig.h"


void RS485_Init(void);					           //串口485初始化设置
void Send_data(uint8_t *Send_Buffer,uint8_t Send_Cnt);
void  Analyse_Received_data(uint8_t *Analy_Buffer,uint8_t Analy_Cnt);
void Response_IO_Relay_State(uint8_t cmd);
void UART_Cmd_Control_Relay(uint8_t *Cmd_Temp);
void Response_Cmd_Control_Msg(uint8_t cmd);
void Response_RS485_Adrress(uint8_t addr,uint8_t cmd);



#endif


