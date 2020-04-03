#ifndef __KEYAPP_H
#define __KEYAPP_H


#define uint16_t	unsigned int 
#define uint8_t 	unsigned char

typedef struct
{
	uint8_t MK_Port;
	uint8_t SK_Port;
	uint8_t SK_Pin[4];
	
}SysSetTypedef;//ϵͳ������Ϣ���ݽṹ����


typedef enum
{
	WITTING,
	SCANING,
}StatusTypedef;


void setup(void);
void loop(void);


#endif /*_KEYAPP_H*/
