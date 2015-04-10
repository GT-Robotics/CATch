#include "stm32f0xx.h"

#include "stm32f0xx_rcc.h"

#include "pd_gpio.h"
#include "pd_pwm.h"
#include "pd_adc.h"
#include "utils.h"
					 
/*
	PB1 ADC1 CH9
	PB11 TIM2 CH4
*/

int main(void) {
	uint16_t proc_cnt = 1;	 
	uint16_t BASE_Threshold = 0;
	uint16_t adc_val = 0;	

	uint16_t touch_cnt = 0;	
   	uint16_t servo_offset = 0;
	// init peripheral
	PWM_Configuration();
	GPIO_Configuration(); 	
	ADC1_Config();		   
	SysTick_Config(48000000 / 1000); // 48MHz / (48000000  / N ) == N Hz
	  	  
	LED_B_OFF;	  
	LED_G_OFF;
	
	// ADC port calibration		
	while(proc_cnt++ < 1000)
	{											   	    
	    adc_val = GetTouch();
		// recursive average
		//BASE_Threshold = (proc_cnt-1) * BASE_Threshold / proc_cnt + adc_val / proc_cnt;	
		// find Max
		if(BASE_Threshold < adc_val)
			BASE_Threshold = adc_val;
								 
		LED_G_TOGGLE;

		Delay_ms(1);
	}			
	BASE_Threshold = BASE_Threshold / 10;	 		  
	LED_B_ON;
	LED_G_OFF;

    while (1)
    {   	    
	    adc_val = GetTouch();

		if(adc_val < BASE_Threshold)
			touch_cnt++;

		if(touch_cnt > 5)
		{	  		  
			touch_cnt = 0;
			servo_offset = (TIM2->CNT % 90);  
			Delay_ms(300);
		}
		if(touch_cnt) 
		{				
			LED_B_ON;				 
			PWM_SetP(servo_offset + (TIM2->CNT % 10));	
		}			
		else
			LED_B_OFF; 
						
		Delay_ms(10);
    }

	return 0;
}

void SysTick_Handler(void)		// 10k Systick
{
	Utils_Task(1000);
}
