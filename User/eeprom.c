#include "eeprom.h"
#include "intrins.h"

/******************************************************************************
 ����: void IapIdle(void)
 ����: �ر�IAP
 ����: ��
 ����: ��
******************************************************************************/
void IapIdle(void)
{
    IAP_CONTR = 0;                 
    IAP_CMD = 0;                    
    IAP_TRIG = 0;                  
    IAP_ADDRH = 0x80;               
    IAP_ADDRL = 0;
}


/******************************************************************************
 ����: void EraseSector(u16 addr)
 ����: ����Ŀ������
 ����: addr ��Ҫ�����ĵ�ַ
 ����: ��
******************************************************************************/
void EraseSector(u16 addr)
{
    IAP_CONTR = ENABLE_IAP;         
    IAP_CMD = CMD_ERASE;            
    IAP_ADDRL = addr;               
    IAP_ADDRH = addr >> 8;          
    IAP_TRIG = 0x5a;                
    IAP_TRIG = 0xa5;                
    _nop_();                       
    IapIdle();
}


/******************************************************************************
 ����: void WriteByte(u16 addr,u8 dat)
 ����: ��Ŀ������д������
 ����: addr ��Ҫд�����ݵĵ�ַ
       dat  ��Ҫд�������
 ����: ��
******************************************************************************/
void WriteByte(u16 addr,u8 dat)
{
    IAP_CONTR = ENABLE_IAP;         
    IAP_CMD = CMD_PROGRAM;         
    IAP_ADDRL =  addr;              
    IAP_ADDRH = addr >> 8;         
    IAP_DATA = dat;               
    IAP_TRIG = 0x5a;                
    IAP_TRIG = 0xa5;               
    _nop_();                        
    IapIdle();
}



/******************************************************************************
 ����: u8 ReadByte(u16 addr)
 ����: ��Ŀ�������������
 ����: addr ��Ҫ��ȡ���ݵĵ�ַ
 ����:  ��Ŀ�������ȡ������
******************************************************************************/
u8 ReadByte(u16 addr)
{
    u8 dat;                       

    IAP_CONTR = ENABLE_IAP;         
    IAP_CMD = CMD_READ;             
    IAP_ADDRL = addr;               
    IAP_ADDRH = addr >> 8;          
    IAP_TRIG = 0x5a;                
    IAP_TRIG = 0xa5;               
    _nop_();                       
    dat = IAP_DATA;                 
    IapIdle();                    

    return dat;                     
}


/*******************************END*********************************/
