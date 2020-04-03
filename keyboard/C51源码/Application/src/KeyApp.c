/**
  ******************************************************************************
  * @file    UartCom.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-7-16
  * @brief   �����������
  ******************************************************************************
  */
	
/*Include start --------------------------------------------------------------*/
#include "keyApp.h"
#include "reg52.h"


#include "UartCom.h"
#include "keyboard.h"
#include "stdio.h"

/*Include end ----------------------------------------------------------------*/

/*Global Data Space start ----------------------------------------------------*/

SysSetTypedef g_SysConfig; //ϵͳ����

StatusTypedef g_State = WITTING;

/*Global Data Space end ------------------------------------------------------*/

//�����ʱ����
void delay_ms(uint16_t ms)
{
	uint16_t i,j;
	for(i = 0;i < ms;i++)
	{
		for(j = 0;j < 115;j++);
	}
}
/*Communication And System Config---------------------------------------------*/

/*MCU to PC------------------------------------------------------------*/

uint8_t HexToChar(uint8_t hex)
{
	uint8_t ch[16] = { '0','1','2','3','4','5','6','7',
										 '8','9','A','B','C','D','E','F', };
	return ch[hex];
}

//PC��Ϣ���Ͷ���5�ֽ� <IXX>
void SendMsg(uint8_t ID,uint8_t dat)
{
	uint8_t str[6];
	
	str[0] = '<';														//��ʼ
	str[1] = ID;														//��ʶ
	str[2] = HexToChar((dat & 0xf0) >> 4);	//����1
	str[3] = HexToChar(dat & 0x0f);					//����2
	str[4] = '>';														//����
	str[5] = 0;
	
	UART1_SendString(str);
}

//�ϴ����ݵ�PC
void UpdataToPC(void)
{
	static uint16_t div = 0;
	uint8_t temp;
	
	div++;
	if(div > 50)//	���50��Ƶ
	{
		div = 0;
		
		//�ϴ������������
		temp = MatrixKeyboardScan(g_SysConfig.MK_Port);
		SendMsg('M',temp);
		
		delay_ms(10);
		//�ϴ�������������
		temp = IndependentKeyboardScan(g_SysConfig.SK_Port);
		SendMsg('S',temp);
	}
}

/*PC to MCU------------------------------------------------------------*/
//�������port����
void MatrixKeyboardPortSet(uint8_t * cmd)
{
	switch(cmd[0])
	{
		case '0':
		{
			g_SysConfig.MK_Port = 0;
			SendMsg('P',0x00);
			break;
		}		
		case '1':
		{
			g_SysConfig.MK_Port = 1;
			SendMsg('P',0x01);
			break;
		}		
		case '2':
		{
			g_SysConfig.MK_Port = 2;
			SendMsg('P',0x02);
			break;
		}		
		default:
		{
			SendMsg('P',0x01);
			break;
		}
	}
}
//��������port����
void SKeyboardPortSet(uint8_t * cmd)
{
	switch(cmd[0])
	{
		case '0':
		{
			g_SysConfig.SK_Port = 0;
			SendMsg('D',0x00);
			break;
		}		
		case '1':
		{
			g_SysConfig.SK_Port = 1;
			SendMsg('D',0x01);
			break;
		}		
		case '2':
		{
			g_SysConfig.SK_Port = 2;
			SendMsg('D',0x02);
			break;
		}		
		default:
		{
			SendMsg('D',0x01);
			break;
		}
	}
}


//����ͨ�Ž�������
void Packet_Analysis(void)																			
{
	USARTMsgTypedef usartMsg; //��������
	
	//��ô�����Ϣ
	usartMsg = getUsart1Msg();
	
	
	//��ʼ��Ϣ���ݽ���
	switch(usartMsg.ID)
	{
		case 'P'://�趨�������port
		{
			MatrixKeyboardPortSet(usartMsg.msg);
			break;
		}
		case 'D'://�趨�������̶˿�
		{
			 SKeyboardPortSet(usartMsg.msg);
			break;
		}
		
		// null  msg
		case 255:break;
		
		default:
		{
			//func error feedback
			//UART1_SendString("Error\n");
			break;
		}
	}
}




/*setup and loop function ----------------------------------------------------*/
void setup(void)
{
	//hard ware init
	UART1_Init();
	
	g_SysConfig.MK_Port = 0;
	g_SysConfig.SK_Port = 1;
	
	SendMsg('P',0x00);
	SendMsg('D',0x01);
}


void loop(void)
{	
	Packet_Analysis();	
	
	UpdataToPC();
	
}
