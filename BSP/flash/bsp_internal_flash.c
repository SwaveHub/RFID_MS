  /**
  ******************************************************************************
  * @file    bsp_internal_flash.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   internal flash 底层应用函数bsp 
  ******************************************************************************
  * @attention
  *
  * 
  * 
  * 
  *
  ******************************************************************************
  */
#include "./flash/bsp_internal_flash.h"

void InternalFlash_UpdateRecord(item_info_t* item_info, uint32_t dataLen);

/**
 @brief 内部Flash读取
 @param address -[in] 读取的地址
 @param pData -[out] 指向需要操作的数据
 @param dataLen -[in] 数据长度
 @return 读出成功的字节数
*/
uint32_t Internal_ReadFlash(uint32_t addrStart, void *pData, uint32_t dataLen)
{
    uint32_t nread = dataLen;
    uint8_t *pBuffer = (uint8_t *)pData;
    const uint8_t *pAddr = (const uint8_t *)addrStart;

    if(!pData || addrStart < STM32FLASH_BASE || addrStart > STM32FLASH_END)
    {
        return 0;
    }

    while(nread >= sizeof(uint32_t) && (((uint32_t)pAddr) <= (STM32FLASH_END - 4)))
    {
        *(uint32_t *)pBuffer = *(uint32_t *)pAddr;
        pBuffer += sizeof(uint32_t);
        pAddr += sizeof(uint32_t);
        nread -= sizeof(uint32_t);
    }

    while(nread && (((uint32_t)pAddr) < STM32FLASH_END))
    {
        *pBuffer++ = *pAddr++;
        nread--;
    }

    return dataLen - nread;
}

/**
 @brief 内部Flash无检查写入
 @param address -[in] 写入的地址
 @param pData -[in] 指向需要操作的数据
 @param dataLen -[in] 数据长度
 @return 实际写入的数据量，单位：半字
*/
uint32_t Internal_WriteFlashNoCheck(uint32_t addrStart, const uint16_t *pData, uint32_t dataLen)
{
    uint32_t nwrite = dataLen;
    uint32_t addrmax = STM32FLASH_END - 2;
    /* 解锁FLASH */
    HAL_FLASH_Unlock();
    while(nwrite)
    {
        if(addrStart > addrmax)
        {
            break;
        }

        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addrStart, *pData);
        if((*(__IO uint16_t*) addrStart) != *pData)
        {
            break;
        }

        nwrite--;
        pData++;
        addrStart += 2;
    }
    /* 加锁FLASH */
    HAL_FLASH_Lock();        
    return (dataLen - nwrite);
}

/**
 @brief 内部Flash写入
 @param address -[in] 写入的地址
 @param pData -[in] 指向需要操作的数据
 @param dataLen -[in] 数据长度
 @return 实际写入的数据量，单位：半字
*/
uint32_t Internal_WriteFlash(uint32_t addrStart, const uint16_t *pData, uint32_t dataLen)
{   
    uint32_t i = 0;
    uint32_t pagepos = 0;         // 页位置
    uint32_t pageoff = 0;         // 页内偏移地址
    uint32_t pagefre = 0;         // 页内空余空间
    uint32_t offset = 0;          // Address在FLASH中的偏移
    uint32_t nwrite = dataLen;    // 记录剩余要写入的数据量
    const uint16_t *pBuffer = (const uint16_t *)pData;
    uint16_t *FlashBuffer = (uint16_t *)malloc(sizeof(uint16_t) * (STM32FLASH_PAGE_SIZE >> 1));
    
    /* 非法地址 */
    if(addrStart < STM32FLASH_BASE || addrStart > (STM32FLASH_END - 2) || dataLen == 0 || pData == NULL)
    {
        return 0;
    }

    /* 解锁FLASH */
    HAL_FLASH_Unlock();

    /* 计算偏移地址 */
    offset = addrStart - STM32FLASH_BASE;
    /* 计算当前页位置 */
    pagepos = offset / STM32FLASH_PAGE_SIZE;
    /* 计算要写数据的起始地址在当前页内的偏移地址 */
    pageoff = ((offset % STM32FLASH_PAGE_SIZE) >> 1);
    /* 计算当前页内空余空间 */
    pagefre = ((STM32FLASH_PAGE_SIZE >> 1) - pageoff);
    /* 要写入的数据量低于当前页空余量 */
    if(nwrite <= pagefre)
    {
        pagefre = nwrite;
    }
    
    while(nwrite != 0)
    {
        /* 检查是否超页 */
        if(pagepos >= STM32FLASH_PAGE_NUM)
        {
            break;
        }

        /* 读取一页 */
        Internal_ReadFlash(STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE, FlashBuffer, STM32FLASH_PAGE_SIZE);

        /* 检查是否需要擦除 */
        for(i = 0; i < pagefre; i++)
        {
            if(*(FlashBuffer + pageoff + i) != 0xFFFF) /* FLASH擦出后默认内容全为0xFF */
            {
                break;
            }
        }

        if(i < pagefre)
        {
            uint32_t count = 0;
            uint32_t index = 0;
            uint32_t PageError = 0;
            FLASH_EraseInitTypeDef pEraseInit;

            /* 擦除一页 */
            pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
            pEraseInit.PageAddress = STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE;
            pEraseInit.Banks = FLASH_BANK_1;
            pEraseInit.NbPages = 1;
            if(HAL_FLASHEx_Erase(&pEraseInit, &PageError) != HAL_OK)
            {
                break;
            }

            /* 复制到缓存 */
            for(index = 0; index < pagefre; index++)
            {
                *(FlashBuffer + pageoff + index) = *(pBuffer + index);
            }

            /* 写回FLASH */
            count = Internal_WriteFlashNoCheck(STM32FLASH_BASE + pagepos * STM32FLASH_PAGE_SIZE, FlashBuffer, STM32FLASH_PAGE_SIZE >> 1);
            if(count != (STM32FLASH_PAGE_SIZE >> 1))
            {
                nwrite -= count;
                break;
            }
        }
        else
        {
            /* 无需擦除，直接写 */
            uint32_t count = Internal_WriteFlashNoCheck(addrStart, pBuffer, pagefre);
            if(count != pagefre)
            {
                nwrite -= count;
                break;
            }
        }

        pBuffer += pagefre;         /* 读取地址递增         */
        addrStart += (pagefre << 1);  /* 写入地址递增         */
        nwrite -= pagefre;          /* 更新剩余未写入数据量 */

        pagepos++;     /* 下一页           */
        pageoff = 0;   /* 页内偏移地址置零  */

        /* 根据剩余量计算下次写入数据量 */
        pagefre = nwrite >= (STM32FLASH_PAGE_SIZE >> 1) ? (STM32FLASH_PAGE_SIZE >> 1) : nwrite;
    }

    /* 加锁FLASH */
    HAL_FLASH_Lock();
    free(FlashBuffer);

    return ((dataLen - nwrite) << 1);
}

/**
 @brief 内部Flash页擦除
 @param pageAddress -[in] 擦除的起始地址
 @param nbPages -[in] 擦除页数
 @return 0 - 成功；-1 - 失败
*/
int Internal_ErasePage(uint32_t pageAddress, uint32_t nbPages)
{
	uint32_t pageError = 0;
	FLASH_EraseInitTypeDef eraseInit;
    /* 解锁FLASH */
    HAL_FLASH_Unlock();
	eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	eraseInit.PageAddress = pageAddress;
	eraseInit.Banks = FLASH_BANK_1;
	eraseInit.NbPages = nbPages;
	if(HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK)
	{
        /* 加锁FLASH */
        HAL_FLASH_Lock();    
		return -1;
	}
    /* 加锁FLASH */
    HAL_FLASH_Lock();    
	return 0;
}

/**
 @brief 内部Flash查找可写入标签记录的起始地址
 @param addrStart -[in] 查找区间起始地址
 @param dataLen -[in] 数据长度
 @return 可写入标签记录的起始地址
*/
uint32_t InternalFlash_FindStartAddr(uint32_t addrStart, uint32_t dataLen)
{
    uint16_t mark = 0;
    uint32_t cnt = 0;
    
    do {
        if (Internal_ReadFlash(addrStart + cnt * dataLen, &mark, 2) == 0) return 0; //内部flash读取失败返回0
         cnt++;
    } while (mark != 0xFFFF);
    
    return addrStart + --cnt * dataLen;
}

/**
 @brief 内部Flash查找指定标签TID记录的起始地址
 @param addrStart -[in] 查找区间起始地址
 @param dataLen -[in] 数据长度
 @return 指定标签TID记录的起始地址
*/
uint32_t InternalFlash_FindTIDAddr(uint8_t *tid, uint32_t addrStart, uint32_t dataLen)
{
    uint32_t cnt = 0;
    item_info_t *item_info_new = (item_info_t *)malloc(sizeof(item_info_t));
    
    do {
        Internal_ReadFlash(WRITE_START_ADDR + cnt * dataLen, (uint16_t *)item_info_new, dataLen);
        if (memcmp(tid, item_info_new->TID, 12) == 0) {
            free(item_info_new);
            return WRITE_START_ADDR + cnt * dataLen;
        }
         cnt++;
    } while (item_info_new->mark != 0xFFFF);
    
    return 0;
}

/**
 @brief 内部Flash写入标签记录
 @param head_ref -[in] 指向需要操作的数据链表头
 @param dataLen -[in] 单个链表结点数据长度
 @return 无
*/
void InternalFlash_WriteRecord(item_info_t** head_ref, uint32_t dataLen)
{
    item_info_t* current = *head_ref;
    uint32_t cnt = 0;
    uint32_t addrStart = 0;
    
    while (current != NULL) {
        //根据不同的mark，用不同的方式写记录至flash
        if (current->mark == MARK_INSTORAGE) {
            addrStart = InternalFlash_FindStartAddr(WRITE_START_ADDR, dataLen); //查找可写入标签记录的起始地址
            if (addrStart >= WRITE_START_ADDR && addrStart <= WRITE_END_ADDR - dataLen) { //地址合法
                Internal_WriteFlash(addrStart , (uint16_t *)current, dataLen / 2);
            }
        } else if (current->mark == MARK_OUTSTORAGE) {
            InternalFlash_UpdateRecord(current, dataLen);
        }
        current = current->next;
        cnt++;
    }
}

/**
 @brief 内部Flash读取标签记录
 @param dataLen -[in] 单个链表结点数据长度
 @return 无
*/
void InternalFlash_ReadRecord(uint32_t dataLen)
{
    item_info_t *item_info_new = (item_info_t *)malloc(sizeof(item_info_t));
    
    uint8_t *p = (uint8_t *)item_info_new;
    uint32_t cnt = 0;
    
    do {
        if (Internal_ReadFlash(WRITE_START_ADDR + cnt * dataLen, (uint16_t *)item_info_new, dataLen) == 0) {
            free(item_info_new);
            return ; //内部flash读取失败返回
        }
        if(item_info_new->mark != 0xFFFF) {
           for (int i = 0; i < dataLen; i++) {
                printf("%02X ", p[i]);
            }
            printf("\r\n");
        } else {
            free(item_info_new);
            return;
        }
         cnt++;
    } while (1);    

}

/**
 @brief 内部Flash重置标签记录区
 @param addrStart -[in] 查找区间起始地址
 @param addrEnd -[in] 查找区间结束地址
 @return 无
*/
void InternalFlash_ResetRecord(uint32_t addrStart, uint32_t addrEnd)
{
    uint32_t nbPages = (addrEnd - addrStart) / STM32FLASH_PAGE_SIZE;
    
    Internal_ErasePage(addrStart, nbPages);
}

/**
 @brief 内部Flash更新标签记录
 @param head_ref -[in] 指向需要操作的数据链表头
 @param dataLen -[in] 数据长度
 @return 无
*/
void InternalFlash_UpdateRecord(item_info_t* item_info, uint32_t dataLen)
{
    item_info_t* item_info_new = malloc(sizeof(item_info_t));
    uint32_t addrStart = 0; 
    
   addrStart = InternalFlash_FindTIDAddr(item_info->TID, WRITE_START_ADDR, dataLen);
    if (addrStart >= WRITE_START_ADDR && addrStart <= WRITE_END_ADDR - dataLen) { //地址合法
        Internal_ReadFlash(addrStart, (uint16_t *)item_info_new, dataLen);
        item_info_new->mark = item_info->mark;
        item_info_new->outstorage_time = item_info->outstorage_time;
        Internal_WriteFlashNoCheck(addrStart, (uint16_t *)item_info_new, dataLen / 2);
    }
    free(item_info_new);
}

/*********************************************END OF FILE**********************/

