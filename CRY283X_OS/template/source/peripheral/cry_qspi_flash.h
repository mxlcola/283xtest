/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_qspi_flash.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x 噪声传感器
*******************************************************************************/
#ifndef __CRY_QSPI_FLASH_H__
#define __CRY_QSPI_FLASH_H__

#include "hc32_ddl.h"


#define QSNSS_PORT                      (PortB)
#define QSNSS_PIN                       (Pin12)

#define QSPCK_PORT                      (PortB)
#define QSPCK_PIN                       (Pin13)

#define QSIO0_PORT                      (PortB)
#define QSIO0_PIN                       (Pin14)

#define QSIO1_PORT                      (PortB)
#define QSIO1_PIN                       (Pin15)




#define FLASH_DEVICE					"W25Q64JV"


#define QSPI_BUS_ADDRESS                (0x98000000ul)		// QSPI FLASH 总线地址映射 （芯片手册 801）
#define FLASH_PAGE_SIZE                 (0x100u)			// 256 	页数量
#define FLASH_SRCTOR_SIZE               (0x1000u)			// 4K	每页大小
#define FALSH_MAX_ADDR                  (0x800000ul)		// 8M	总共大小

#define FLASH_DUMMY_BYTE_VALUE          (0xffu)
#define FLASH_BUSY_BIT_MASK             (0x01u)

#define FLASH_INSTR_WRITE_ENABLE        (0x06u)
#define FLASH_INSTR_PAGE_PROGRAM        (0x02u)
#define FLASH_INSTR_ERASE_4KB_SECTOR    (0x20u)
#define FLASH_INSTR_ERASE_CHIP          (0xC7u)
#define FLASH_INSTR_READ_SR1            (0x05u)
#define FLASH_INSTR_READ_SR2            (0x35u)
#define FLASH_INSTR_READ_SR3            (0x15u)
#define FLASH_INSTR_WRITE_SR1        	(0x01u)
#define FLASH_INSTR_WRITE_SR2        	(0x31u)
#define FLASH_INSTR_WRITE_SR3        	(0x11u)

void qspi_flash_init(void);
void qspi_flash_WriteEnable(void);
en_result_t qspi_flash_WaitForWriteEnd(void);
en_result_t qspi_flash_WritePage(uint32_t u32Addr, const uint8_t *pData, uint16_t len);
en_result_t qspi_flash_Erase4KbSector(uint32_t u32Addr);
void qspi_flash_EraseChip(void);
uint8_t qspi_flash_ReadStatusRegister(uint8_t u8Reg);

void qspi_flash_test(void);


#endif

