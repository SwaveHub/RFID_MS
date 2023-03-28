#ifndef __INTERNAL_FLASH_H
#define __INTERNAL_FLASH_H

#include "stm32f1xx_hal.h"
#include "linked_list.h"

/* FLASH大小：STM32F103ZET6:512K */
#define STM32FLASH_SIZE         0x00080000UL
/* FLASH起始地址 */
#define STM32FLASH_BASE         FLASH_BASE
/* FLASH结束地址 */
#define STM32FLASH_END          (STM32FLASH_BASE | STM32FLASH_SIZE)
/* FLASH页大小：2K */
#define STM32FLASH_PAGE_SIZE    FLASH_PAGE_SIZE
/* FLASH总页数 */
#define STM32FLASH_PAGE_NUM     (STM32FLASH_SIZE / STM32FLASH_PAGE_SIZE)

#define WRITE_START_ADDR        ((uint32_t)0x08060000)
#define WRITE_END_ADDR          ((uint32_t)0x08080000)


uint32_t Internal_WriteFlash(uint32_t addrStart, const uint16_t *pData, uint32_t dataLen);
uint32_t Internal_ReadFlash(uint32_t addrStart, void *pData, uint32_t dataLen);

void InternalFlash_WriteRecord(item_info_t** head_ref, uint32_t dataLen);
void InternalFlash_ReadRecord(uint32_t dataLen);
void InternalFlash_ResetRecord(uint32_t addrStart, uint32_t addrEnd);

#endif /* __INTERNAL_FLASH_H */

