/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_flash.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ����������
*******************************************************************************/
#ifndef __CRY_FLASH_H__
#define __CRY_FLASH_H__

#include "hc32_ddl.h"


#define FLASH_SECTOR_ADRR 	(0x0007E000u)		// FLASH SECTOR63 ��ʼ��ַ   (0x0007E000u ~ 0x0007FFFFu) (512k FLASH), ���У�0x0007FFE0u ~ 0x0007FFFFu����32Byte���� (����оƬ�����ֲᣨpage 231�� ע������)
#define	FLASH_SECTOR_LEN	(0x00001FE0u)		// SECTOR ��С8K, ��32Byte����  ��0x00002000u-0x20u��

// д32�ֽڵ�Flashָ����ַ
void Flash_Write(uint32_t addr, uint32_t u32data);   


#endif
