
#ifndef __PD_PWM_H
#define __PD_PWM_H

#include "stm32f0xx.h"

#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"

#define SERVO_MIN 1000
#define SERVO_MAX 2000

void PWM_Set(uint16_t duty);
void PWM_SetP(uint16_t percent);

void PWM_Configuration(void);

#endif
