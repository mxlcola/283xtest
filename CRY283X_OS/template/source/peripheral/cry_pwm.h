/******************** (C) COPYRIGHT 2023 CRY Sound Co., Ltd ********************
* File Name          : cry_pwm.h
* Author             : Ming
* Mail				 : ming.huang@crysound.com
* Version            : V1.0.0
* Date               : 2023.07.31
* Description        : CRY283x ÔëÉù´«¸ÐÆ÷
*******************************************************************************/
#ifndef __CRY_PWM_H__
#define __CRY_PWM_H__


#include "hc32_ddl.h"


#define PWM_PERIOD	8000
#define PWM_PORT	(PortE)
#define PWM_PIN		(Pin05)

void pwm_init(void);
void pwm_SetCmpValue(uint16_t compValue);


#endif
