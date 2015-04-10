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
const uint8_t STATE_IDLE = 0;
const uint8_t STATE_VIBE = 1;
const uint8_t STATE_SWING = 2;
int main(void) {
	uint8_t state = STATE_IDLE;

	uint16_t proc_cnt = 1;	 
	uint16_t BASE_Threshold = 0;
	uint16_t adc_val = 0;	

	uint16_t touch_cnt = 0;	
	uint16_t idle_cnt = 0;	
   	uint16_t servo_offset = 50;

	uint8_t i,j;
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
		// find Maximum in idle state
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
		{
			touch_cnt++;
			idle_cnt = 0;
		}
		else
			idle_cnt++;
					
		switch(state)
		{
			case STATE_IDLE:
				if(touch_cnt > 5)
					state = STATE_VIBE;
				else if(idle_cnt > 30000)
					state = STATE_SWING;
						    					
				break;
			case STATE_VIBE:	
				PWM_SetP(servo_offset + (TIM2->CNT % 10));
				if(touch_cnt > 25) {
					servo_offset = (TIM2->CNT % 90);
					touch_cnt = 0;
				} else if (idle_cnt > 5000)
				{
					state = STATE_SWING;	
				}			
				break;
			case STATE_SWING:
				for(j=0; j<1+(TIM2->CNT % 5); j++)
				{
					for(i=0; i<100; i++)
					{
						PWM_SetP(i);
						Delay_ms(1+(TIM2->CNT % 40));
					}
					for(i=100; i>0; i--)
					{
						PWM_SetP(i);   
						Delay_ms(1+(TIM2->CNT % 40));
					}
				}
				PWM_SetP((TIM2->CNT % 100));	  			   
				touch_cnt = 0;
				idle_cnt = 0;
				state = STATE_IDLE;
					
				break;
		}			 
		Delay_ms(1);
    }

	return 0;
}

void SysTick_Handler(void)		// 10k Systick
{
	Utils_Task(1000);
}
