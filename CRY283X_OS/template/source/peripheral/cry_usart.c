/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_usart.c
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ÔëÉù´«¸ÐÆ÷
*******************************************************************************/
#include "cry_usart.h"





void uart_init(void)
{
	stc_usart_uart_init_t stcInitCfg;
	stc_irq_regi_conf_t stcIrqRegiCfg;
	uint32_t u32Fcg1Periph = PWC_FCG1_PERIPH_USART1 | PWC_FCG1_PERIPH_USART2 | \
                             PWC_FCG1_PERIPH_USART3 | PWC_FCG1_PERIPH_USART4;
	
	
	MEM_ZERO_STRUCT(stcInitCfg);
    MEM_ZERO_STRUCT(stcIrqRegiCfg);
	

	PWC_Fcg1PeriphClockCmd(u32Fcg1Periph, Enable);
	
	PORT_SetFunc(DEBUG_USART_RX_PORT, DEBUG_USART_RX_PIN, DEBUG_USART_RX_FUNC, Disable);
    PORT_SetFunc(DEBUG_USART_TX_PORT, DEBUG_USART_TX_PIN, DEBUG_USART_TX_FUNC, Disable);
	
	
	USART_DeInit(DEBUG_USART_CH);
	stcInitCfg.enClkMode = UsartIntClkCkOutput;
	stcInitCfg.enClkDiv = UsartClkDiv_1;
	stcInitCfg.enDataLength = UsartDataBits8;
	stcInitCfg.enDirection = UsartDataLsbFirst;
	stcInitCfg.enStopBit = UsartOneStopBit;
	stcInitCfg.enParity = UsartParityNone;
	stcInitCfg.enSampleMode = UsartSampleBit8;
	stcInitCfg.enDetectMode = UsartStartBitFallEdge;
	stcInitCfg.enHwFlow = UsartRtsEnable;
	USART_UART_Init(DEBUG_USART_CH, &stcInitCfg);
	
	USART_SetBaudrate(DEBUG_USART_CH, DEBUG_USART_BAUDRATE);
	
//    stcIrqRegiCfg.enIRQn = Int004_IRQn;
//    stcIrqRegiCfg.pfnCallback = &UsartRxIrqCallback;
//    stcIrqRegiCfg.enIntSrc = INT_USART2_RI;
//    enIrqRegistration(&stcIrqRegiCfg);
//    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
//    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
//    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);


    USART_FuncCmd(DEBUG_USART_CH, UsartRx, Enable);
	USART_FuncCmd(DEBUG_USART_CH, UsartTx, Enable);
//    USART_FuncCmd(DEBUG_USART_CH, UsartRxInt, Enable);
}



