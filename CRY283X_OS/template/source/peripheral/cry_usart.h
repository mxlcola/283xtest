/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_usart.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ÔëÉù´«¸ÐÆ÷
*******************************************************************************/
#ifndef __CRY_USART_H__
#define __CRY_USART_H__


#include "hc32_ddl.h"

#define DEBUG_USART_CH                    	(M4_USART2)
	

#define DEBUG_USART_BAUDRATE           		(115200ul)


#define DEBUG_USART_RX_PORT              	(PortA)
#define DEBUG_USART_RX_PIN            		(Pin02)
#define DEBUG_USART_RX_FUNC           		(Func_Usart2_Rx)
	

#define DEBUG_USART_TX_PORT             	(PortA)
#define DEBUG_USART_TX_PIN             		(Pin03)
#define DEBUG_USART_TX_FUNC              	(Func_Usart2_Tx)


void uart_init(void);



#endif

