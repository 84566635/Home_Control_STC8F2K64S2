#include "rs485.h"
#include "io.h"
#include "relay.h"
#include "delay.h"
#include "crc8.h"
#include <string.h>

uint8_t Rx_Buffer[10] = {0};      //串口中断接收数据的Buffer
uint8_t Rx_Cnt = 0;               //串口中断接收数据的数量

/******************************************************************************
 函数: void RS485_Init(void)
 描述: RS485_usart初始化函数
 参数: 无
 返回: 无
******************************************************************************/
void RS485_Init(void)
{ 
	 AUXR1 &= 0X3F;    // #00xx xxxx [S1_S1:S1_S0] = [0:0],将串口1的TXD RXD引脚切换至P3.1 P3.0
	
	 AUXR1 = 0x40;

   SCON = 0x50;      // #0101 0000 [SM0:SM1]=[0:0],串口1工作在模式1--10位串口通信，即1位起始位+8位数据位+1位停止位，无校验位
	
	 AUXR |= 0x05;     // #xxxx x1x1   定时器T2不分频，即工作在1T模式；
	                   //              串口1选择定时器T2作为波特率发生器                
	
	 AUXR &= 0xF7;     //#xxxx 0xxx  定时器T2用作定时器功能；
	    
   T2L = 0XC0;       //通过给T2装初值确定串口1的波特率    
   T2H =  0XFD;
	
	 AUXR |= 0x10;     //启动定时器T2
	
   ES = 1;           //开串口1中断
	
   EA = 1;           //开总中断
}
/******************************************************************************
 函数: void SerialPortInte(void)
 描述: 串口中断就收数据
 参数: 无
 返回: 无
******************************************************************************/
void UART1(void) interrupt 4 
{ 
	if(RI)
	{
		RI=0;						                         //清零接收标志位
		
		Rx_Buffer[Rx_Cnt++]=SBUF;				            	//读取缓冲区的数据
		
		if(Rx_Buffer[0] == 0xad)
		{
			if(Rx_Buffer[1] == 0xda)
			{
				if(Rx_Cnt == Rx_Buffer[2]+4)	
				{
					Analyse_Received_data(Rx_Buffer,Rx_Cnt);
					memset(Rx_Buffer,0,10);
					Rx_Cnt = 0;				
				}
				if(Rx_Cnt > (Rx_Buffer[2]+4))
				{
					memset(Rx_Buffer,0,10);
					Rx_Cnt = 0;						
				}				
			}
			else if(Rx_Cnt > 1)
			{
				memset(Rx_Buffer,0,10);
				Rx_Cnt = 0;				
			}
		}
		else
		{
			memset(Rx_Buffer,0,10);
			Rx_Cnt = 0;
		}
	}
}

/******************************************************************************
 函数: Send_data(u8 *buf,u16 len)
 描述: 将数据通过485串口发送出去
 参数: buf  需发送的数据数组
       len  需发送数据的长度
 返回: 无
******************************************************************************/
void Send_data(uint8_t *Send_Buffer,uint8_t Send_Cnt)
{
	uint8_t Cnt;

	for(Cnt =0;Cnt < Send_Cnt;Cnt++)
	{
     SBUF = Send_Buffer[Cnt]; 				       	//发送数据
     while(!TI); 					        //等待发送完成
     TI=0; 							          //清零发送标志位
  }
}
/******************************************************************************
 函数: int Analyse_Received_data(u8 *sub_string_buf,u16 len)
 描述: 解析收到的数据，是否帧头满足，是否crc满足
 参数: sub_string_buf  需解析的数据数组
       len             需解析数据的长度
 返回: 数据是否有效  1，有效；0，无效
******************************************************************************/
void  Analyse_Received_data(uint8_t *Analy_Buffer,uint8_t Analy_Cnt)
{
//  /*对收到的数据中附加的CRC码和本地根据收到的数据包算出的CRC码进行校验对比*/
//	if(CRC8_Check(Analy_Cnt-1-3,Analy_Buffer+3) != Analy_Buffer[Analy_Cnt-1])
//		return ;
	
	if(CRC8_Check(Analy_Cnt-1,Analy_Buffer) != Analy_Buffer[Analy_Cnt-1])
		 return ;
  /*开始根据RS485传输的指令进行比对和处理*/	 
	if(Analy_Buffer[3] == RS485_Addr)
	{							
		switch(Analy_Buffer[4])
		{ 						
			case 0x20:
				Device_Online_Count = 500;//5s
			  IO_State_Convert();
			  Relay_State_Conver();	
        Response_IO_Relay_State(Analy_Buffer[4]);
			  memset(IO_State_Buffer,0,2);
        memset(IO_Input_Lock,0,8);			
			  break;
			case 0x60:                                               //查询调光值			   
        UART_Cmd_Control_Relay(Analy_Buffer);	
				break;
     
      default:
        break;				
		}
	}
	else if(Analy_Buffer[3] == 0xff && Analy_Buffer[4] == 0xf0)       //查询本机地址的命令
	{
    Response_RS485_Adrress(Analy_Buffer[3],Analy_Buffer[4]); 
	}
}

/******************************************************************************
 函数: void Response_RS485_Adrress(uint8_t addr,uint8_t cmd)
 描述: 对查询RS485地址的命令进行回复
 参数: uint8_t addr   本模块RS485的地址
       uint8_t cmd    地址查询的命令字
 返回: 无
******************************************************************************/

void Response_RS485_Adrress(uint8_t addr,uint8_t cmd)
{
  uint8_t Send_Temp[15]={0};
  uint8_t CRC_Temp[10]={0};
	
  Send_Temp[0]=0xad;
  CRC_Temp[0]=Send_Temp[0];
  
  Send_Temp[1]=0xda;
  CRC_Temp[1]=Send_Temp[1];
  
  Send_Temp[2]=0x03;
  CRC_Temp[2]=Send_Temp[2];  
  
  Send_Temp[3]=addr;
  CRC_Temp[3]=Send_Temp[3];
  
  Send_Temp[4]=cmd;
  CRC_Temp[4]=Send_Temp[4];
  
  Send_Temp[5]=Device_Mode;
  CRC_Temp[5]=Send_Temp[5];
  
  Send_Temp[6]=RS485_Addr;
  CRC_Temp[6]=Send_Temp[6];
  
  Send_Temp[2]=4;
  
  Send_Temp[7]=CRC8_Check(7,CRC_Temp);

	Send_data(Send_Temp,8);
}

/******************************************************************************
 函数: void UART_Cmd_Control_Relay(uint8_t *Cmd_Temp)
 描述: 通过命令对继电器进行控制
 参数: uint8_t *Cmd_Temp   继电器的控制指令buffer
 返回: 无
******************************************************************************/
void UART_Cmd_Control_Relay(uint8_t *Cmd_Temp)
{	
 if(Cmd_Temp[5] <= Device_Mode)
 {	
	 Cmd_Control_Relay(Cmd_Temp);
   Response_Cmd_Control_Msg(Cmd_Temp[5]);	
 }
}


/******************************************************************************
 函数: void Response_Cmd_Control_Msg(uint8_t cmd)
 描述: 对控制继电器的命令进行回复
 参数: uint8_t cmd    继电器控制命令字
 返回: 无
******************************************************************************/
void Response_Cmd_Control_Msg(uint8_t cmd)
{
  uint8_t Send_Temp[15]={0};
  uint8_t CRC_Temp[10]={0};
  Send_Temp[0]=0xad;
  CRC_Temp[0]=Send_Temp[0];
  
  Send_Temp[1]=0xda;
  CRC_Temp[1]=Send_Temp[1];
  
  Send_Temp[2]=0x03;
  CRC_Temp[2]=Send_Temp[2];  
  
  Send_Temp[3]=RS485_Addr;
  CRC_Temp[3]=Send_Temp[3];
  
  Send_Temp[4]=cmd;
  CRC_Temp[4]=Send_Temp[4];
  
  Send_Temp[5]=Device_Mode;
  CRC_Temp[5]=Send_Temp[5];
  
  Send_Temp[2]=3;
  
  Send_Temp[6]=CRC8_Check(6,CRC_Temp);

	Send_data(Send_Temp,7);
}

/******************************************************************************
 函数: void Response_IO_Relay_State(uint8_t cmd)
 描述: 查询模块IO、IO使能拨码开关以及继电器的状态
 参数: uint8_t cmd    查询状态的命令字
 返回: 无
******************************************************************************/
void Response_IO_Relay_State(uint8_t cmd)
{
  uint8_t Send_Temp[15]={0};
  uint8_t CRC_Temp[10]={0};
  Send_Temp[0]=0xad;
  CRC_Temp[0]=Send_Temp[0];
  
  Send_Temp[1]=0xda;
  CRC_Temp[1]=Send_Temp[1];
  
  Send_Temp[2]=0x03;
  CRC_Temp[2]=Send_Temp[2];
  
  Send_Temp[3]=RS485_Addr;
  CRC_Temp[3]=Send_Temp[3];
  
  Send_Temp[4]=cmd;
  CRC_Temp[4]=Send_Temp[4];
  
  Send_Temp[5]=Device_Mode;
  CRC_Temp[5]=Send_Temp[5];
  
  Send_Temp[6]=IO_State_Buffer[1];
  CRC_Temp[6]=Send_Temp[6];
  
  Send_Temp[7]=IO_State_Buffer[0];
  CRC_Temp[7]=Send_Temp[7];
  
  Send_Temp[8]=Relay_State;
  CRC_Temp[8]=Send_Temp[8];
  
  Send_Temp[2]=6;
  
  Send_Temp[9]=CRC8_Check(9,CRC_Temp);

	Send_data(Send_Temp,10);
}
/**********************************THE END**********************************/ 

 