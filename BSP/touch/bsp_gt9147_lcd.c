/**
  ******************************************************************************
  * @file    bsp_gt9147_lcd.c
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   ����������
  ******************************************************************************
  * @attention
  *
  * 
  * 
  * 
  *
  ******************************************************************************
  */ 

#include "bsp_gt9147_lcd.h"
#include "bsp.h"

/**
  * @brief  GT9147 ��ʼ������
  * @param  ��
  * @retval ��
  */	
void GT9147_Init( void )
{
    I2C_Touch_Init();
}


/**
  * @brief  ���������״̬��
  * @retval ����״̬
	*   �÷���ֵΪ����ֵ֮һ��
  *     @arg TOUCH_PRESSED :��������
  *     @arg TOUCH_NOT_PRESSED :�޴���
  */
uint8_t GT9147_TouchDetect()
{
	static enumTouchState touch_state = GT9147_STATE_RELEASE;
	static uint32_t i;
	uint8_t detectResult = TOUCH_NOT_PRESSED;
	
	switch(touch_state)
	{
		case GT9147_STATE_RELEASE:	
			if(GT9147_PENIRQ_Read() == GT9147_PENIRQ_ActiveLevel) //��һ�γ��ִ����ź�
			{
				touch_state = GT9147_STATE_WAITING;
				detectResult =TOUCH_NOT_PRESSED;
				}
			else	//�޴���
			{
				touch_state = GT9147_STATE_RELEASE;
				detectResult =TOUCH_NOT_PRESSED;
			}
			break;
				
		case GT9147_STATE_WAITING:
				if(GT9147_PENIRQ_Read() == GT9147_PENIRQ_ActiveLevel)
				{
					 i++;
					//�ȴ�ʱ�������ֵ����Ϊ����������
					//����ʱ�� = DURIATION_TIME * �����������õ�ʱ����
					//���ڶ�ʱ���е��ã�ÿ10ms����һ�Σ�������ʱ��Ϊ��DURIATION_TIME*10ms
					if(i > DURIATION_TIME)		
					{
						i=0;
						touch_state = GT9147_STATE_PRESSED;
						detectResult = TOUCH_PRESSED;
					}
					else												//�ȴ�ʱ���ۼ�
					{
						touch_state = GT9147_STATE_WAITING;
						detectResult =	 TOUCH_NOT_PRESSED;					
					}
				}
				else	//�ȴ�ʱ��ֵδ�ﵽ��ֵ��Ϊ��Ч��ƽ�����ɶ�������					
				{
				    i = 0;
            touch_state = GT9147_STATE_RELEASE; 
						detectResult = TOUCH_NOT_PRESSED;
				}
		
			break;
		
		case GT9147_STATE_PRESSED:	
				if(GT9147_PENIRQ_Read() == GT9147_PENIRQ_ActiveLevel)		//������������
				{
					touch_state = GT9147_STATE_PRESSED;
					detectResult = TOUCH_PRESSED;
				}
				else	//�����ͷ�
				{
					touch_state = GT9147_STATE_RELEASE;
					detectResult = TOUCH_NOT_PRESSED;
				}
			break;			
		
		default:
				touch_state = GT9147_STATE_RELEASE;
				detectResult = TOUCH_NOT_PRESSED;
				break;
	
	}		
	
	return detectResult;    
}


/**
  * @brief  ��ȡ�����������
  * @param  pDisplayCoordinate ����ָ���Ż�ȡ���Ĵ���������
  * @retval ��ȡ���
	*   �÷���ֵΪ����ֵ֮һ��
  *     @arg 1 :��ȡ�ɹ�
  *     @arg 0 :��ȡʧ��
  */
uint8_t GT9147_Get_TouchedPoint(strType_GT9147_Coordinate *pDisplayCoordinate)
{
    GTP_Execu(&(pDisplayCoordinate->x), &(pDisplayCoordinate->y));
}

/***************************end of file*****************************/


