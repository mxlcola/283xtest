/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_flash.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x 噪声传感器
*******************************************************************************/
#ifndef __CRY_FLASH_H__
#define __CRY_FLASH_H__

#include "hc32_ddl.h"


#define FLASH_SECTOR_ADRR 	(0x0007E000u)		// FLASH SECTOR63 起始地址   (0x0007E000u ~ 0x0007FFFFu) (512k FLASH), 其中（0x0007FFE0u ~ 0x0007FFFFu）后32Byte保留 (根据芯片技术手册（page 231） 注意事项)
#define	FLASH_SECTOR_LEN	(0x00001FE0u)		// SECTOR 大小8K, 后32Byte保留  （0x00002000u-0x20u）

// 写32字节到Flash指定地址
void Flash_Write(uint32_t addr, uint32_t u32data);   


#endif
