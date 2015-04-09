
#include "pd_gpio.h"

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	 
													
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
