#ifndef __UARTCOM_H
#define __UARTCOM_H

#define uint16_t	unsigned int 
#define uint8_t 	unsigned char
	
typedef struct
{
	uint8_t buf[16];
	uint8_t len;
	uint8_t status;
	
}USARTBufTypedef;//���ڽ��ջ�������������

typedef struct
{
	uint8_t ID;
	uint8_t msg[16];
	uint8_t len;
	
}USARTMsgTypedef;//������Ϣ�洢��������



void UART1_Init(void);   												//���ڳ�ʼ��115200������

void UART1_SendString(uint8_t *s);							//�����ַ�������

USARTMsgTypedef getUsart1Msg(void);							//����ָ��Э����Ϣ���� ��ʼ��<�� ��ʶ��A�� ���ݡ�...�� ������>��

#endif /*__UARTCOM_H*/


