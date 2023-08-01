/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_sdcard.c
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ‘Î…˘¥´∏–∆˜
*******************************************************************************/
#include "cry_sdcard.h"



static stc_sd_handle_t stcSdhandle;

void sdcard_init(void)
{
	uint16_t init_count = 0;
	stc_sdcard_init_t stcCardInitCfg;
	stc_sdcard_dma_init_t stcDmaInitCfg;
	
	MEM_ZERO_STRUCT(stcCardInitCfg);
	MEM_ZERO_STRUCT(stcDmaInitCfg);
	
    PORT_SetFunc(SDIOC_D0_PORT, 	SDIOC_D0_PIN, 	Func_Sdio, Disable);
    PORT_SetFunc(SDIOC_D1_PORT, 	SDIOC_D1_PIN, 	Func_Sdio, Disable);
    PORT_SetFunc(SDIOC_D2_PORT, 	SDIOC_D2_PIN, 	Func_Sdio, Disable);
    PORT_SetFunc(SDIOC_D3_PORT, 	SDIOC_D3_PIN, 	Func_Sdio, Disable);
    PORT_SetFunc(SDIOC_CD_PORT, 	SDIOC_CD_PIN, 	Func_Sdio, Disable);
    PORT_SetFunc(SDIOC_CK_PORT, 	SDIOC_CK_PIN, 	Func_Sdio, Disable);
    PORT_SetFunc(SDIOC_CMD_PORT, 	SDIOC_CMD_PIN, 	Func_Sdio, Disable);
	
	
	stcCardInitCfg.enBusWidth = SdiocBusWidth4Bit;
	stcCardInitCfg.enSpeedMode = SdiocNormalSpeedMode; //SdiocHighSpeedMode;//SdiocNormalSpeedMode;
	if(stcCardInitCfg.enSpeedMode == SdiocHighSpeedMode)
	{
		stcCardInitCfg.enClkFreq = SdiocClk50M;
	}
	else
	{
		stcCardInitCfg.enClkFreq = SdiocClk25M;
	}
	stcCardInitCfg.pstcInitCfg = NULL;
	
	
	stcDmaInitCfg.DMAx = SD_DMA_UNIT;
	stcDmaInitCfg.enDmaCh = SD_DMA_CH;
	
	stcSdhandle.SDIOCx = SDIOC_UNIT;
	stcSdhandle.pstcDmaInitCfg = &stcDmaInitCfg;
	

	while(SDCARD_Init(&stcSdhandle, &stcCardInitCfg) != Ok)
	{
		Ddl_Delay1ms(10ul);
		init_count++;
		if(init_count > 100)
		{
			break;
		}
	}

}

en_result_t sdcard_WriteBlocks(uint8_t *pu8Data, uint32_t u32Timeout)
{
	return SDCARD_WriteBlocks(&stcSdhandle, SD_SECTOR_START, SD_SECTOR_COUNT, pu8Data, u32Timeout);
}

en_result_t sdcard_ReadBlocks(uint8_t *pu8Data, uint32_t u32Timeout)
{
	return SDCARD_ReadBlocks(&stcSdhandle, SD_SECTOR_START, SD_SECTOR_COUNT, pu8Data, u32Timeout);
}


// SD ø®∂¡–¥≤‚ ‘
void sdcard_test(void)
{
	uint32_t au32WriteBlocks[512] = {0};
    uint32_t au32ReadBlocks[512] = {0};
	
	for (int i = 0u; i < 512; i++)
	{
		au32WriteBlocks[i] = i;
	}
	
	sdcard_init();
	
	Ddl_Delay1ms(5ul);
	
	en_result_t result1 = sdcard_WriteBlocks((uint8_t *)au32WriteBlocks, 2000u);
	
	Ddl_Delay1ms(5ul);
	
	en_result_t result2 = sdcard_ReadBlocks((uint8_t *)au32ReadBlocks, 20000u);
	
	printf("sdcard test result = %d %d %d\r\n", memcmp(au32WriteBlocks, au32ReadBlocks, sizeof(au32ReadBlocks)), result1, result2);
	
}
