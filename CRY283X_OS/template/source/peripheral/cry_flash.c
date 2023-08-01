/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_flash.c
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ÔëÉù´«¸ÐÆ÷
*******************************************************************************/
#include "cry_flash.h"


void Flash_Write(uint32_t addr, uint32_t u32data)
{
	uint32_t i = 0;
	uint32_t u32Addr = 0;
	uint32_t len = (FLASH_SECTOR_LEN >> 2u);
	
	uint32_t u32buffer[len];
	uint32_t *u32bufferSrc = (uint32_t *)FLASH_SECTOR_ADRR;
	
	uint32_t offset = ((addr - FLASH_SECTOR_ADRR) >> 2u);
	
	
	for(i = 0u; i < len; i++)
	{
		u32buffer[i] = u32bufferSrc[i];
	}
	
	u32buffer[offset] = u32data;
	
	EFM_Unlock();
	EFM_FlashCmd(Enable);
	
	while(Set != EFM_GetFlagStatus(EFM_FLAG_RDY))
    {
        ;
    }
	
	EFM_SectorErase(FLASH_SECTOR_ADRR);
	
	u32Addr = FLASH_SECTOR_ADRR;
	
	for(i = 0u; i < len; i++)
    {
        EFM_SingleProgram(u32Addr, u32buffer[i]);
        u32Addr += 4u;
    }
	
//	EFM_SectorErase(FLASH_SECTOR_ADRR);
//	EFM_SequenceProgram(u32Addr, (uint32_t)FLASH_SECTOR_LEN, u32buffer);
//	EFM_MassErase(FLASH_SECTOR_ADRR);
	
	EFM_Lock();
}
