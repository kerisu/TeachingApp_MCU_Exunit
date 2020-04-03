/**
  ******************************************************************************
  * @file    UartCom.c
  * @author  �Ŷ�
  * @version V1.0.0
  * @date    2019-7-16
  * @brief   �����������
  ******************************************************************************
  */
#include "UartCom.h"
#include "reg52.h"

#define STARTBT 					0x3c
#define ENDBT							0x3e

bit	B_TX1_Busy;																//����æ��־

USARTBufTypedef g_UsartRx;

/**********************************************************************/
/*							  ���ڳ�ʼ��
*			������=11059200 / { 32��[65536-(RCAP2H,RCAP2L)] }    
*			���е�RCAP2H,RCAP2LΪ�Զ���װֵ������ʽ�ã�
*			RCAP2H,RCAP2L=65536 11059200 / (32��������) 
*			�ò�����Ϊ115200ʱ��RCAP2H,RCAP2L=0xff
**********************************************************************/
void UART1_Init(void)   												//���ڳ�ʼ��115200������
{
	SCON=0x50; 															//���ڹ�����ʽ1��8λUART�������ʿɱ�  
	TH2=0xFF;           
	TL2=0xFD;    														//������:115200 ����=11.0592MHz 
	RCAP2H=0xFF;   
	RCAP2L=0xFD; 														//16λ�Զ���װ��ֵ
	TCLK=1;   
	RCLK=1;   
	C_T2=0;   
	EXEN2=0; 															//�����ʷ�����������ʽ
	TR2=1; 																//��ʱ��2��ʼ
	ES=1;
	EA=1;
	
	
	g_UsartRx.status = 0;
	g_UsartRx.len = 0;
}
/**********************************************************************/
/*							  ���ڷ����ڽ���
/**********************************************************************/
void UART1_SendData(uint8_t dat)
{
    while (B_TX1_Busy);               									//�ȴ�ǰ������ݷ������
    B_TX1_Busy = 1;
    SBUF = dat;                											//д���ݵ�Uart1���ݼĴ���
}
void UART1_SendString(uint8_t *s)
{
    while (*s)                  										//����ַ���������־
    {
        UART1_SendData(*s++);         									//���͵�ǰ�ַ�
    }
}


USARTMsgTypedef getUsart1Msg(void)
{
	USARTMsgTypedef msg;
	int i;
	
	if(g_UsartRx.status == 1)		//�������������
	{
		msg.ID = g_UsartRx.buf[1];								//��ȡ��Ϣ��ʶ��
		for(i = 0;i < g_UsartRx.len - 2;i++)	//��ȡ����
		{
			msg.msg[i] = g_UsartRx.buf[i + 2];
		}
		msg.len = g_UsartRx.len;									//������ݳ���
		g_UsartRx.status = 0;											//����������
	}
	else
	{
		msg.len = 0;
		msg.ID = 255;
	}
	return msg;																//������Ϣ����
}

//Usart 1 Receive Function
void Usart1_Receive(void)
{
	uint8_t bt; 
	
	bt = SBUF;//��ȡ���յ��ֽ�
	
	if(g_UsartRx.status == 0)//��ǰ��Ϣδ������
	{
		if(bt == ENDBT)				//��ǰ�ֽ�Ϊ������־
		{
			g_UsartRx.status = 1;//����������
		}
		else
		{
			if(bt == STARTBT)//��ǰ�ֽ�Ϊ��ʼ��־
			{
				g_UsartRx.len = 0;
			}
			//����ǰ�ֽ�д�뻺����
			g_UsartRx.buf[g_UsartRx.len] = bt;
			//�ƶ�д���±겢����
			g_UsartRx.len = (g_UsartRx.len + 1) % 64;
		}
	}
}


/**********************************************************************/
/*							  �����ж�
/**********************************************************************/
void UART1_Int() interrupt 4 											//�����ж�
{
	if(RI)		 														//�����жϱ�־λ
	{
		RI = 0;
		//SBUF
		Usart1_Receive();
	}
	if(TI)		 														//�����жϱ�־λ
	{
		TI = 0;
		B_TX1_Busy = 0;													//�������æ��־
	}
}
	
	
	
	