/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_qspi_flash.c
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x 噪声传感器
*******************************************************************************/
#include "cry_qspi_flash.h"



void qspi_flash_init(void)
{
	stc_qspi_init_t stcQspiInit;
	
	MEM_ZERO_STRUCT(stcQspiInit);
	
	PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_QSPI, Enable);
	
	// 引脚初始化
	PORT_SetFunc(QSPCK_PORT, QSPCK_PIN, Func_Qspi, Disable);
    PORT_SetFunc(QSNSS_PORT, QSNSS_PIN, Func_Qspi, Disable);
    PORT_SetFunc(QSIO0_PORT, QSIO0_PIN, Func_Qspi, Disable);
    PORT_SetFunc(QSIO1_PORT, QSIO1_PIN, Func_Qspi, Disable);
	
    stcQspiInit.enClkDiv = QspiHclkDiv2;
    stcQspiInit.enSpiMode = QspiSpiMode0;
    stcQspiInit.enBusCommMode = QspiBusModeRomAccess;
    stcQspiInit.enPrefetchMode = QspiPrefetchStopComplete;
    stcQspiInit.enPrefetchFuncEn = Disable;
    stcQspiInit.enQssnValidExtendTime = QspiQssnValidExtendNot;
    stcQspiInit.enQssnIntervalTime = QspiQssnIntervalQsck8;
    stcQspiInit.enQsckDutyCorr = QspiQsckDutyCorrNot;
    stcQspiInit.enVirtualPeriod = QspiVirtualPeriodQsck8;
    stcQspiInit.enWpPinLevel = QspiWpPinOutputLow;
    stcQspiInit.enQssnSetupDelayTime = QspiQssnSetupDelayHalfQsck;
    stcQspiInit.enQssnHoldDelayTime = QspiQssnHoldDelayHalfQsck;
    stcQspiInit.enFourByteAddrReadEn = Disable;
    stcQspiInit.enAddrWidth = QspiAddressByteThree;
    stcQspiInit.stcCommProtocol.enReadMode = QspiReadModeStandard;
    stcQspiInit.stcCommProtocol.enTransInstrProtocol = QspiProtocolExtendSpi;
    stcQspiInit.stcCommProtocol.enTransAddrProtocol = QspiProtocolExtendSpi;
    stcQspiInit.stcCommProtocol.enReceProtocol = QspiProtocolExtendSpi;
    stcQspiInit.u8RomAccessInstr = QSPI_3BINSTR_STANDARD_READ;
    QSPI_Init(&stcQspiInit);
}

void qspi_flash_WriteEnable(void)
{
    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(FLASH_INSTR_WRITE_ENABLE);
    QSPI_ExitDirectCommMode();
}

en_result_t qspi_flash_WaitForWriteEnd(void)
{
    en_result_t enRet = Ok;
    uint8_t u8Status = 0u;
    uint32_t u32Timeout;
    stc_clk_freq_t stcClkFreq;

    CLK_GetClockFreq(&stcClkFreq);
    u32Timeout = stcClkFreq.sysclkFreq / 1000u;
    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(FLASH_INSTR_READ_SR1);
    do
    {
        u8Status = QSPI_ReadDirectCommValue();
        u32Timeout--;
    } while ((u32Timeout != 0u) && ((u8Status & FLASH_BUSY_BIT_MASK) == FLASH_BUSY_BIT_MASK));

    if (FLASH_BUSY_BIT_MASK == u8Status)
    {
        enRet = ErrorTimeout;
    }
    QSPI_ExitDirectCommMode();

    return enRet;
}

en_result_t qspi_flash_WritePage(uint32_t u32Addr, const uint8_t *pData, uint16_t len)
{
    en_result_t enRet = Ok;
    uint16_t u16Index = 0u;

    if ((u32Addr > FALSH_MAX_ADDR) || (NULL == pData) || (len > FLASH_PAGE_SIZE))
    {
        enRet = Error;
    }
    else
    {
        qspi_flash_WriteEnable();

        QSPI_EnterDirectCommMode();
        QSPI_WriteDirectCommValue(FLASH_INSTR_PAGE_PROGRAM);
        QSPI_WriteDirectCommValue((uint8_t)((u32Addr & 0xFF0000ul) >> 16));
        QSPI_WriteDirectCommValue((uint8_t)((u32Addr & 0xFF00u) >> 8));
        QSPI_WriteDirectCommValue((uint8_t)(u32Addr & 0xFFu));
        while (len--)
        {
			printf("[%d] QSPI FLASH WRITE, VALUE = [0x%x]\r\n", u16Index, pData[u16Index]);
			
           QSPI_WriteDirectCommValue(pData[u16Index]);
           u16Index++;
        }
        QSPI_ExitDirectCommMode();

        enRet = qspi_flash_WaitForWriteEnd();
    }

    return enRet;
}

en_result_t qspi_flash_Erase4KbSector(uint32_t u32Addr)
{
    en_result_t enRet = Ok;

    if (u32Addr >= FALSH_MAX_ADDR)
    {
        enRet = Error;
    }
    else
    {
        qspi_flash_WriteEnable();

        QSPI_EnterDirectCommMode();
        QSPI_WriteDirectCommValue(FLASH_INSTR_ERASE_4KB_SECTOR);
        QSPI_WriteDirectCommValue((uint8_t)((u32Addr & 0xFF0000ul) >> 16));
        QSPI_WriteDirectCommValue((uint8_t)((u32Addr & 0xFF00u) >> 8));
        QSPI_WriteDirectCommValue((uint8_t)(u32Addr & 0xFFu));
        QSPI_ExitDirectCommMode();

        enRet = qspi_flash_WaitForWriteEnd();
    }

    return enRet;
}

void qspi_flash_EraseChip(void)
{
    qspi_flash_WriteEnable();
    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(FLASH_INSTR_ERASE_CHIP);
    QSPI_ExitDirectCommMode();
    qspi_flash_WaitForWriteEnd();
}

uint8_t qspi_flash_ReadStatusRegister(uint8_t u8Reg)
{
    uint8_t regSta = 0u;

    QSPI_EnterDirectCommMode();
    QSPI_WriteDirectCommValue(u8Reg);
    regSta = QSPI_ReadDirectCommValue();
    QSPI_ExitDirectCommMode();

    return regSta;
}

void qspi_flash_test(void)
{
	uint32_t flashAddr = 0u;
	
	uint32_t pFlashReadVal = 0x12345678;
	
	en_result_t result1, result2;
	
	qspi_flash_init();
	
	Ddl_Delay1ms(5ul);
	
	uint32_t pFlashReadAddr = QSPI_BUS_ADDRESS + flashAddr;
	
	printf("[1] pFlashReadAddr = %d\r\n", *(uint32_t *)pFlashReadAddr);
	
	result1 = qspi_flash_Erase4KbSector(flashAddr);
	
	result2 = qspi_flash_WritePage(flashAddr, (uint8_t*)&pFlashReadVal, sizeof(uint32_t));
	
	Ddl_Delay1ms(5ul);
	
	
	pFlashReadVal = *(uint32_t *)pFlashReadAddr;
	
	printf("[2] pFlashReadAddr = %d %d %d\r\n", pFlashReadVal, result1, result2);
	
}



