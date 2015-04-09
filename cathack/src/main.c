#include "stm32f0xx.h"

#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_adc.h"

#include "pd_gpio.h"
#include "pd_pwm.h"
					 
__IO uint16_t  ADC1ConvertedValue = 0, ADC1ConvertedVoltage = 0;

/*
	PB1 ADC1 CH9
	PB10 TIM2 CH3
*/
void ADC1_Config(void)
{
  ADC_InitTypeDef          ADC_InitStructure;
  GPIO_InitTypeDef         GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef        TIM_OCInitStructure; 
  
  /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* TIM3 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  
  /* Configure ADC Channel11 as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* TIM3 Configuration *******************************************************/
  TIM_DeInit(TIM3);
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_OCStructInit(&TIM_OCInitStructure);
    
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* TIM3 TRGO selection */
  TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
  
  /* ADC1 Configuration *******************************************************/
  /* ADCs DeInit */  
  ADC_DeInit(ADC1);
  
  /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits*/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;    
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure); 
  
  /* Convert the ADC1 Channel 11 with 239.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_9 , ADC_SampleTime_28_5Cycles);   

  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);

  /* Enable the auto delay feature */    
  ADC_WaitModeCmd(ADC1, ENABLE); 
  
  /* Enable the Auto power off mode */
  ADC_AutoPowerOffCmd(ADC1, ENABLE); 
  
  /* Enable ADCperipheral[PerIdx] */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADCEN falg */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
  
  /* TIM2 enable counter */
  TIM_Cmd(TIM3, ENABLE);
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
}

void Delay(volatile uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}

int BASE_Threshold = 1500;
int main(void) {
	int proc_cnt = 1;
	int A_MAX = 0;	
	PWM_Configuration();
	GPIO_Configuration(); 
	ADC1_Config();

	PWM_Set(1500);	  	  
	LED_B_OFF;	  
	LED_G_OFF;
			
	while(proc_cnt++ < 500)
	{											   
	    /* Get ADC1 converted data */
	    ADC1ConvertedValue = ADC_GetConversionValue(ADC1);
	    
	    /* Compute the voltage */
	    ADC1ConvertedVoltage = (ADC1ConvertedValue * 3300)/0xFFF;

		//BASE_Threshold = (proc_cnt-1) * BASE_Threshold / proc_cnt + ADC1ConvertedVoltage / proc_cnt;	
		if(A_MAX < ADC1ConvertedVoltage)
			A_MAX = ADC1ConvertedVoltage;
								 
		LED_G_TOGGLE;
		Delay(4800000 / 100);
	}			
		 		  
	LED_B_ON;
	LED_G_OFF;

    while (1)
    {
	    /* Get ADC1 converted data */
	    ADC1ConvertedValue = ADC_GetConversionValue(ADC1);
	    
	    /* Compute the voltage */
	    ADC1ConvertedVoltage = (ADC1ConvertedValue * 3300)/0xFFF;

		if(ADC1ConvertedVoltage > A_MAX * 2)
		{
			LED_B_ON;
			PWM_Set(1000 + (TIM2->CNT % 1000));
			Delay(4800000 / 10);
		}
		else
			LED_B_OFF;
    }

	return 0;
}
