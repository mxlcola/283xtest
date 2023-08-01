/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_spi_flash.c
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x 噪声传感器
*******************************************************************************/
#include "cry_spi_flash.h"


void spi_flash_init(void)
{
	stc_port_init_t stcPortInit;
	stc_spi_init_t stcSpiInit;
	
	
	MEM_ZERO_STRUCT(stcPortInit);
	MEM_ZERO_STRUCT(stcSpiInit);
	
	stcPortInit.enPinMode = Pin_Mode_Out;
    PORT_Init(SPI_NSS_PORT, SPI_NSS_PIN, &stcPortInit);
    SPI_NSS_HIGH();
	
	
	PWC_Fcg1PeriphClockCmd(SPI_UNIT_CLOCK, Enable);
	
    PORT_SetFunc(SPI_SCK_PORT, 	SPI_SCK_PIN, 	SPI_SCK_FUNC, 	Disable);
    PORT_SetFunc(SPI_MOSI_PORT, SPI_MOSI_PIN, 	SPI_MOSI_FUNC, 	Disable);
    PORT_SetFunc(SPI_MISO_PORT, SPI_MISO_PIN, 	SPI_MISO_FUNC, 	Disable);
	
	stcSpiInit.enClkDiv 					= SpiClkDiv2;
    stcSpiInit.enFrameNumber 				= SpiFrameNumber1;
    stcSpiInit.enDataLength 				= SpiDataLengthBit8;
    stcSpiInit.enFirstBitPosition 			= SpiFirstBitPositionMSB;
    stcSpiInit.enSckPolarity 				= SpiSckIdleLevelLow;
    stcSpiInit.enSckPhase 					= SpiSckOddSampleEvenChange;
    stcSpiInit.enReadBufferObject 			= SpiReadReceiverBuffer;
    stcSpiInit.enWorkMode 					= SpiWorkMode4Line;
    stcSpiInit.enTransMode 					= SpiTransFullDuplex;
    stcSpiInit.enCommAutoSuspendEn 			= Disable;
    stcSpiInit.enModeFaultErrorDetectEn 	= Disable;
    stcSpiInit.enParitySelfDetectEn 		= Disable;
    stcSpiInit.enParityEn 					= Disable;
    stcSpiInit.enParity 					= SpiParityEven;
	
    stcSpiInit.enMasterSlaveMode 						= SpiModeMaster;
    stcSpiInit.stcDelayConfig.enSsSetupDelayOption 		= SpiSsSetupDelayCustomValue;
    stcSpiInit.stcDelayConfig.enSsSetupDelayTime 		= SpiSsSetupDelaySck1;
    stcSpiInit.stcDelayConfig.enSsHoldDelayOption 		= SpiSsHoldDelayCustomValue;
    stcSpiInit.stcDelayConfig.enSsHoldDelayTime 		= SpiSsHoldDelaySck1;
    stcSpiInit.stcDelayConfig.enSsIntervalTimeOption	= SpiSsIntervalCustomValue;
    stcSpiInit.stcDelayConfig.enSsIntervalTime 			= SpiSsIntervalSck6PlusPck2;
//	stcSpiInit.stcSsConfig.enSsValidBit              	= SpiSsValidChannel0;
//    stcSpiInit.stcSsConfig.enSs0Polarity             	= SpiSsLowValid;

    SPI_Init(SPI_UNIT, &stcSpiInit);
    SPI_Cmd(SPI_UNIT, Enable);
	
	SPI_NSS_LOW();
	spi_flash_WriteReadByte(0XFF);
	SPI_NSS_HIGH();
	
//	SPI_Cmd(SPI_UNIT, Disable);
//	SPI_SetClockDiv(SPI_UNIT, SpiClkDiv2);
//	SPI_Cmd(SPI_UNIT, Enable);
}


uint8_t spi_flash_WriteReadByte(uint8_t u8Data)
{
    uint8_t u8Byte;

    while (Reset == SPI_GetFlag(SPI_UNIT, SpiFlagSendBufferEmpty))
    {
    }

    SPI_SendData8(SPI_UNIT, u8Data);

    while (Reset == SPI_GetFlag(SPI_UNIT, SpiFlagReceiveBufferFull))
    {
    }
    u8Byte = SPI_ReceiveData8(SPI_UNIT);

    return u8Byte;
}

void spi_flash_WriteEnable(void)
{
    SPI_NSS_LOW();
    spi_flash_WriteReadByte(SPI_FLASH_INSTR_WRITE_ENABLE);
    SPI_NSS_HIGH();
}

en_result_t spi_flash_WaitForWriteEnd(void)
{
    en_result_t enRet = Ok;
    uint8_t u8Status = 0u;
    uint32_t u32Timeout;
    stc_clk_freq_t stcClkFreq;

    CLK_GetClockFreq(&stcClkFreq);
    u32Timeout = stcClkFreq.sysclkFreq / 1000u;
    SPI_NSS_LOW();
    spi_flash_WriteReadByte(SPI_FLASH_INSTR_READ_SR1);
    do
    {
        u8Status = spi_flash_WriteReadByte(SPI_FLASH_DUMMY_BYTE_VALUE);
        u32Timeout--;
    } while ((u32Timeout != 0ul) && ((u8Status & SPI_FLASH_BUSY_BIT_MASK) == SPI_FLASH_BUSY_BIT_MASK));

    if (SPI_FLASH_BUSY_BIT_MASK == u8Status)
    {
        enRet = ErrorTimeout;
    }
    SPI_NSS_HIGH();

    return enRet;
}

en_result_t spi_flash_WritePage(uint32_t u32Addr, const uint8_t *pData, uint16_t len)
{
    en_result_t enRet = Ok;
    uint16_t u16Index = 0u;

    if ((u32Addr > SPI_FALSH_MAX_ADDR) || (NULL == pData) || (len > SPI_FLASH_PAGE_SIZE))
    {
        enRet = Error;
    }
    else
    {
        spi_flash_WriteEnable();

        SPI_NSS_LOW();
        spi_flash_WriteReadByte(SPI_FLASH_INSTR_PAGE_PROGRAM);
        spi_flash_WriteReadByte((uint8_t)((u32Addr & 0xFF0000ul) >> 16u));
        spi_flash_WriteReadByte((uint8_t)((u32Addr & 0xFF00u) >> 8u));
        spi_flash_WriteReadByte((uint8_t)(u32Addr & 0xFFu));
        while (len--)
        {
            spi_flash_WriteReadByte(pData[u16Index]);
            u16Index++;
        }
        SPI_NSS_HIGH();
        enRet = spi_flash_WaitForWriteEnd();
    }

    return enRet;
}

en_result_t spi_flash_ReadData(uint32_t u32Addr, uint8_t *pData, uint16_t len)
{
    en_result_t enRet = Ok;
    uint16_t u16Index = 0u;

    if ((u32Addr > SPI_FALSH_MAX_ADDR) || (NULL == pData))
    {
        enRet = Error;
    }
    else
    {
        spi_flash_WriteEnable();
        SPI_NSS_LOW();
        spi_flash_WriteReadByte(SPI_FLASH_INSTR_STANDARD_READ);
        spi_flash_WriteReadByte((uint8_t)((u32Addr & 0xFF0000ul) >> 16u));
        spi_flash_WriteReadByte((uint8_t)((u32Addr & 0xFF00u) >> 8u));
        spi_flash_WriteReadByte((uint8_t)(u32Addr & 0xFFu));
        while (len--)
        {
            pData[u16Index] = spi_flash_WriteReadByte(SPI_FLASH_DUMMY_BYTE_VALUE);
			//printf("+++++++++++ 0x%x\r\n", pData[u16Index]);
            u16Index++;
        }
        SPI_NSS_HIGH();
    }

    return enRet;
}

en_result_t spi_flash_Erase4KbSector(uint32_t u32Addr)
{
    en_result_t enRet = Ok;

    if (u32Addr >= SPI_FALSH_MAX_ADDR)
    {
        enRet =  Error;
    }
    else
    {
        spi_flash_WriteEnable();

        SPI_NSS_LOW();
        spi_flash_WriteReadByte(SPI_FLASH_INSTR_ERASE_4KB_SECTOR);
        spi_flash_WriteReadByte((uint8_t)((u32Addr & 0xFF0000ul) >> 16u));
        spi_flash_WriteReadByte((uint8_t)((u32Addr & 0xFF00u) >> 8u));
        spi_flash_WriteReadByte((uint8_t)(u32Addr & 0xFFu));
        SPI_NSS_HIGH();

        enRet = spi_flash_WaitForWriteEnd();
    }

    return enRet;
}

//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
//0XEF18,表示芯片型号为W25Q256
uint16_t spi_flash_ReadID(void)
{
	uint16_t flash_id = 0;	  
	SPI_NSS_LOW();				    
	spi_flash_WriteReadByte(0x90);//发送读取ID命令	    
	spi_flash_WriteReadByte(0x00); 	    
	spi_flash_WriteReadByte(0x00); 	    
	spi_flash_WriteReadByte(0x00); 	 			   
	flash_id|=spi_flash_WriteReadByte(0xFF) << 8;  
	flash_id|=spi_flash_WriteReadByte(0xFF);	 
	SPI_NSS_HIGH();				    
	return flash_id;
}


void spi_flash_test(void)
{
	uint32_t flashAddr = 0u;
	uint32_t pFlashReadVal[32] = {0};// 0x12345678;
	uint32_t pFlashReadRead[32] = {0}; // 0x87654321;
	
	for(int i = 0; i < 32; i++)
	{
		pFlashReadVal[i] = 0x12345678;
	}
	
	spi_flash_init();
	
	Ddl_Delay1ms(5ul);
	
	printf("[SPI FLASH TEST] FLASH ID = 0x%x\r\n", spi_flash_ReadID());
	
	en_result_t result1 = spi_flash_Erase4KbSector(flashAddr);
	
	Ddl_Delay1ms(5ul);
	
	en_result_t result2 = spi_flash_WritePage(flashAddr, (uint8_t*)&pFlashReadVal, sizeof(uint32_t) * 32);
	
	Ddl_Delay1ms(5ul);
	
	en_result_t result3 = spi_flash_ReadData(flashAddr, (uint8_t*)&pFlashReadRead, sizeof(uint32_t) * 32);
	
	for(int i = 0; i < 32; i++)
	{
		printf("[SPI FLASH TEST] pFlashReadRead[%d] = 0x%x\r\n", i, pFlashReadRead[i]);
	}
	
	
	//printf("[SPI FLASH TEST] pFlashReadVal = 0x%x, pFlashReadRead = 0x%x, %d %d %d\r\n", pFlashReadVal[0], pFlashReadRead[0], result1, result2, result3);
	
}


