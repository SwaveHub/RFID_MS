#ifndef __PROTOCOL_H
#define	__PROTOCOL_H

/******************************** ͷ�ļ� *************************************/
#include "stm32f1xx_hal.h"

/******************************** �궨�� *************************************/
#define FORMAT_SET  "SET:"
#define FORMAT_SET_LEN  (strlen(FORMAT_SET))

#define PARM_NUM  5

/***************************** ȫ�ֱ������� **********************************/
extern int *adj_parm[PARM_NUM]; /* ���ڵ�����ָ������ */
extern int parma, parmb;

/* ����Э�鴦��ģʽ */
typedef enum {
    PROTOCOL_MODE_DEFAULT,
}PROTOCOL_MODETypeDef ;

/******************************* �������� ************************************/
int Protocol_Decode(PROTOCOL_MODETypeDef mode, uint8_t *buf, uint16_t len);
void Protocol_AdjParm_Decode(uint8_t Buf[]);
void Protocol_AdjParm_SetPointer(void);
    
#endif /* __PROTOCOL_H */
