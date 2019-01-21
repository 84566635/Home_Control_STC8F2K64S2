#include "rs485.h"
#include "io.h"
#include "relay.h"
#include "delay.h"
#include "crc8.h"
#include <string.h>

uint8_t Rx_Buffer[10] = {0};      //�����жϽ������ݵ�Buffer
uint8_t Rx_Cnt = 0;               //�����жϽ������ݵ�����

/******************************************************************************
 ����: void RS485_Init(void)
 ����: RS485_usart��ʼ������
 ����: ��
 ����: ��
******************************************************************************/
void RS485_Init(void)
{ 
	 AUXR1 &= 0X3F;    // #00xx xxxx [S1_S1:S1_S0] = [0:0],������1��TXD RXD�����л���P3.1 P3.0
	
	 AUXR1 = 0x40;

   SCON = 0x50;      // #0101 0000 [SM0:SM1]=[0:0],����1������ģʽ1--10λ����ͨ�ţ���1λ��ʼλ+8λ����λ+1λֹͣλ����У��λ
	
	 AUXR |= 0x05;     // #xxxx x1x1   ��ʱ��T2����Ƶ����������1Tģʽ��
	                   //              ����1ѡ��ʱ��T2��Ϊ�����ʷ�����                
	
	 AUXR &= 0xF7;     //#xxxx 0xxx  ��ʱ��T2������ʱ�����ܣ�
	    
   T2L = 0XC0;       //ͨ����T2װ��ֵȷ������1�Ĳ�����    
   T2H =  0XFD;
	
	 AUXR |= 0x10;     //������ʱ��T2
	
   ES = 1;           //������1�ж�
	
   EA = 1;           //�����ж�
}
/******************************************************************************
 ����: void SerialPortInte(void)
 ����: �����жϾ�������
 ����: ��
 ����: ��
******************************************************************************/
void UART1(void) interrupt 4 
{ 
	if(RI)
	{
		RI=0;						                         //������ձ�־λ
		
		Rx_Buffer[Rx_Cnt++]=SBUF;				            	//��ȡ������������
		
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
 ����: Send_data(u8 *buf,u16 len)
 ����: ������ͨ��485���ڷ��ͳ�ȥ
 ����: buf  �跢�͵���������
       len  �跢�����ݵĳ���
 ����: ��
******************************************************************************/
void Send_data(uint8_t *Send_Buffer,uint8_t Send_Cnt)
{
	uint8_t Cnt;

	for(Cnt =0;Cnt < Send_Cnt;Cnt++)
	{
     SBUF = Send_Buffer[Cnt]; 				       	//��������
     while(!TI); 					        //�ȴ��������
     TI=0; 							          //���㷢�ͱ�־λ
  }
}
/******************************************************************************
 ����: int Analyse_Received_data(u8 *sub_string_buf,u16 len)
 ����: �����յ������ݣ��Ƿ�֡ͷ���㣬�Ƿ�crc����
 ����: sub_string_buf  ���������������
       len             ��������ݵĳ���
 ����: �����Ƿ���Ч  1����Ч��0����Ч
******************************************************************************/
void  Analyse_Received_data(uint8_t *Analy_Buffer,uint8_t Analy_Cnt)
{
//  /*���յ��������и��ӵ�CRC��ͱ��ظ����յ������ݰ������CRC�����У��Ա�*/
//	if(CRC8_Check(Analy_Cnt-1-3,Analy_Buffer+3) != Analy_Buffer[Analy_Cnt-1])
//		return ;
	
	if(CRC8_Check(Analy_Cnt-1,Analy_Buffer) != Analy_Buffer[Analy_Cnt-1])
		 return ;
  /*��ʼ����RS485�����ָ����бȶԺʹ���*/	 
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
			case 0x60:                                               //��ѯ����ֵ			   
        UART_Cmd_Control_Relay(Analy_Buffer);	
				break;
     
      default:
        break;				
		}
	}
	else if(Analy_Buffer[3] == 0xff && Analy_Buffer[4] == 0xf0)       //��ѯ������ַ������
	{
    Response_RS485_Adrress(Analy_Buffer[3],Analy_Buffer[4]); 
	}
}

/******************************************************************************
 ����: void Response_RS485_Adrress(uint8_t addr,uint8_t cmd)
 ����: �Բ�ѯRS485��ַ��������лظ�
 ����: uint8_t addr   ��ģ��RS485�ĵ�ַ
       uint8_t cmd    ��ַ��ѯ��������
 ����: ��
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
 ����: void UART_Cmd_Control_Relay(uint8_t *Cmd_Temp)
 ����: ͨ������Լ̵������п���
 ����: uint8_t *Cmd_Temp   �̵����Ŀ���ָ��buffer
 ����: ��
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
 ����: void Response_Cmd_Control_Msg(uint8_t cmd)
 ����: �Կ��Ƽ̵�����������лظ�
 ����: uint8_t cmd    �̵�������������
 ����: ��
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
 ����: void Response_IO_Relay_State(uint8_t cmd)
 ����: ��ѯģ��IO��IOʹ�ܲ��뿪���Լ��̵�����״̬
 ����: uint8_t cmd    ��ѯ״̬��������
 ����: ��
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

 