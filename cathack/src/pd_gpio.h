
#ifndef __PD_GPIO_H
#define __PD_GPIO_H

#include "stm32f0xx.h"

#include "stm32f0xx_misc.h"	  
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"


#define LED_B_ON			GPIOB->BSRR		= GPIO_Pin_15
#define LED_B_OFF			GPIOB->BRR 		= GPIO_Pin_15
#define LED_B_TOGGLE		GPIOB->ODR 		^= GPIO_Pin_15
				   										
#define LED_G_ON			GPIOB->BSRR		= GPIO_Pin_10
#define LED_G_OFF			GPIOB->BRR 		= GPIO_Pin_10
#define LED_G_TOGGLE		GPIOB->ODR 		^= GPIO_Pin_10


#define KEY_PRESSED		(GPIOB->IDR & GPIO_Pin_0)				// ((GPIOD->IDR & GPIO_Pin_9) == GPIO_Pin_9)


	/*
	test
	
	PC0		IRQ			TrackBall_R
	PC1		IRQ			TrackBall_L
	PC2		IRQ			TrackBall_D
	PC3		IRQ			TrackBall_U
	PC13		IRQ			TrackBall_Click
	
	PB8		16_CH1		TrackBall_LED_G
	PB9		17_CH1		TrackBall_LED_R	
	PA4		14_CH1		TrackBall_LED_B
	PA5		DAC2		TrackBall_LED_W
	*/
#define LED_TB_R_TOGGLE		GPIOB->ODR 		^= GPIO_Pin_9
#define LED_TB_G_TOGGLE		GPIOB->ODR 		^= GPIO_Pin_8
#define LED_TB_B_TOGGLE		GPIOA->ODR 		^= GPIO_Pin_4
#define LED_TB_W_TOGGLE		GPIOA->ODR 		^= GPIO_Pin_5
	
#define TB_R					(GPIOC->IDR & GPIO_Pin_0)
#define TB_L					(GPIOC->IDR & GPIO_Pin_1)
#define TB_D					(GPIOC->IDR & GPIO_Pin_2)
#define TB_U					(GPIOC->IDR & GPIO_Pin_3)
#define TB_C					(GPIOC->IDR & GPIO_Pin_13)
	
	
void GPIO_Configuration(void);

#endif

