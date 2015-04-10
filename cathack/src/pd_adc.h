
#ifndef __PD_ADC_H
#define __PD_ADC_H

#include "stm32f0xx.h"

#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_adc.h"

void ADC1_Config(void);
uint16_t GetTouch(void);

#endif
