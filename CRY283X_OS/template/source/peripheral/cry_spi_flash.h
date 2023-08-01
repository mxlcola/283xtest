/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_spi_flash.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ÔëÉù´«¸ÐÆ÷
*******************************************************************************/
#ifndef __CRY_SPI_FLASH_H__
#define __CRY_SPI_FLASH_H__

#include "hc32_ddl.h"


#define SPI_SCK_PORT                    (PortB)
#define SPI_SCK_PIN                     (Pin13)
#define SPI_SCK_FUNC                    (Func_Spi3_Sck)


#define SPI_NSS_PORT                    (PortB)
#define SPI_NSS_PIN                     (Pin12)

#define SPI_NSS_HIGH()                  (PORT_SetBits(SPI_NSS_PORT, SPI_NSS_PIN))
#define SPI_NSS_LOW()                   (PORT_ResetBits(SPI_NSS_PORT, SPI_NSS_PIN))

#define SPI_MOSI_PORT                   (PortB)
#define SPI_MOSI_PIN                    (Pin14)
#define SPI_MOSI_FUNC                   (Func_Spi3_Mosi)


#define SPI_MISO_PORT                   (PortB)
#define SPI_MISO_PIN                    (Pin15)
#define SPI_MISO_FUNC                   (Func_Spi3_Miso)


#define SPI_UNIT                        (M4_SPI3)
#define SPI_UNIT_CLOCK                  (PWC_FCG1_PERIPH_SPI3)


#define SPI_FLASH_DEVICE				"W25Q64JV"

#define SPI_FLASH_PAGE_SIZE              (0x100u)
#define SPI_FLASH_SRCTOR_SIZE            (0x1000u)
#define SPI_FALSH_MAX_ADDR               (0x800000ul)
#define SPI_FLASH_DUMMY_BYTE_VALUE       (0xffu)
#define SPI_FLASH_BUSY_BIT_MASK          (0x01u)


#define SPI_FLASH_INSTR_WRITE_ENABLE     (0x06u)
#define SPI_FLASH_INSTR_PAGE_PROGRAM     (0x02u)
#define SPI_FLASH_INSTR_STANDARD_READ    (0x03u)
#define SPI_FLASH_INSTR_ERASE_4KB_SECTOR (0x20u)
#define SPI_FLASH_INSTR_READ_SR1         (0x05u)
#define SPI_FLASH_INSTR_READ_SR2         (0x35u)
#define SPI_FLASH_INSTR_READ_SR3         (0x15u)
#define SPI_FLASH_INSTR_WRITE_SR1        (0x01u)
#define SPI_FLASH_INSTR_WRITE_SR2        (0x31u)
#define SPI_FLASH_INSTR_WRITE_SR3        (0x11u)


void spi_flash_init(void);
uint8_t spi_flash_WriteReadByte(uint8_t u8Data);
void spi_flash_WriteEnable(void);
en_result_t spi_flash_WaitForWriteEnd(void);
en_result_t spi_flash_WritePage(uint32_t u32Addr, const uint8_t *pData, uint16_t len);
en_result_t spi_flash_ReadData(uint32_t u32Addr, uint8_t *pData, uint16_t len);
en_result_t spi_flash_Erase4KbSector(uint32_t u32Addr);
uint16_t spi_flash_ReadID(void);

void spi_flash_test(void);


#endif
