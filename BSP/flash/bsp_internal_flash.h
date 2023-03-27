#ifndef __INTERNAL_FLASH_H
#define __INTERNAL_FLASH_H

#include "stm32f1xx_hal.h"

/* FLASH��С��STM32F103ZET6:512K */
#define STM32FLASH_SIZE         0x00080000UL
/* FLASH��ʼ��ַ */
#define STM32FLASH_BASE         FLASH_BASE
/* FLASH������ַ */
#define STM32FLASH_END          (STM32FLASH_BASE | STM32FLASH_SIZE)
/* FLASHҳ��С��1K */
#define STM32FLASH_PAGE_SIZE    FLASH_PAGE_SIZE
/* FLASH��ҳ�� */
#define STM32FLASH_PAGE_NUM     (STM32FLASH_SIZE / STM32FLASH_PAGE_SIZE)

#define WRITE_START_ADDR        ((uint32_t)0x08060000)
#define WRITE_END_ADDR          ((uint32_t)0x0807FFFF)


uint32_t Internal_WriteFlash(uint32_t addrStart, const uint16_t *pData, uint32_t dataLen);
int Internal_ErasePage(uint32_t pageAddress, uint32_t nbPages);

#endif /* __INTERNAL_FLASH_H */

