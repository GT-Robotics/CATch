
#ifndef __UTILS_H
#define __UTILS_H
											   
#include "stm32f0xx.h"

   
void Utils_Task(unsigned int elapsed_us);

void Delay_Task(unsigned int elapsed_us);
void Delay_us(unsigned int us);
void Delay_ms(unsigned int ms);
	
void Counter_Task(unsigned int elapsed_us);
void Counter_Reset(uint8_t index);
unsigned int Counter_Get(uint8_t index);
			 
void Event_Set(void (*func)(void), unsigned int ms);	
uint8_t Event_IsClear(void);
void Event_Task(unsigned int elapsed_us);

void Interval_Set(uint8_t index, unsigned int interval);
uint8_t Interval_Check(uint8_t index);
void Interval_Task(unsigned int elapsed_us);

float MinF(float inA, float inB);
float MaxF(float inA, float inB);
float ConstrainF(float input, float min, float max);
					 
float Deg2RadF(float deg);
float Rad2DegF(float rad);

#endif
