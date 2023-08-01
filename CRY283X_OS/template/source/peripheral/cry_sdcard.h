/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_sdcard.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ÔëÉù´«¸ÐÆ÷
*******************************************************************************/
#ifndef __CRY_SDCARD_H__
#define __CRY_SDCARD_H__

#include "hc32_ddl.h"
#include "sd_card.h"


#define SDIOC_CD_PORT                   (PortE)
#define SDIOC_CD_PIN                    (Pin14)

#define SDIOC_CK_PORT                   (PortC)
#define SDIOC_CK_PIN                    (Pin12)

#define SDIOC_CMD_PORT                  (PortD)
#define SDIOC_CMD_PIN                   (Pin02)

#define SDIOC_D0_PORT                   (PortC)
#define SDIOC_D0_PIN                    (Pin08)

#define SDIOC_D1_PORT                   (PortC)
#define SDIOC_D1_PIN                    (Pin09)

#define SDIOC_D2_PORT                   (PortC)
#define SDIOC_D2_PIN                    (Pin10)

#define SDIOC_D3_PORT                   (PortC)
#define SDIOC_D3_PIN                    (Pin11)

#define SDIOC_UNIT                      (M4_SDIOC1)

#define SD_SECTOR_START                 (0u)
#define SD_SECTOR_COUNT                 (4u)


#define SD_DMA_UNIT                    	(M4_DMA1)
#define SD_DMA_CH                    	(DmaCh2)

void sdcard_init(void);
en_result_t sdcard_WriteBlocks(uint8_t *pu8Data, uint32_t u32Timeout);
en_result_t sdcard_ReadBlocks(uint8_t *pu8Data, uint32_t u32Timeout);

void sdcard_test(void);

#endif
