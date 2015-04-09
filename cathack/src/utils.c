
#include "utils.h"
					 											   
#include "stm32f0xx.h"
	

/* ==============================================================================================
	// at CPU Timer
============================================================================================== */

void Utils_Task(unsigned int elapsed_us)
{  
	Delay_Task(elapsed_us);			
	Counter_Task(elapsed_us);
	Event_Task(elapsed_us);
	Interval_Task(elapsed_us);
}


/* ==============================================================================================
	// Delay
============================================================================================== */
	   
volatile unsigned int DelayCount = 0;										  

void Delay_Task(unsigned int elapsed_us)
{	  
	if (DelayCount > elapsed_us)
		DelayCount -=  elapsed_us;
	else
		DelayCount = 0;
}

void Delay_us(unsigned int us)
{					   
	DelayCount = us;
	while (DelayCount);
}

void Delay_ms(unsigned int ms)
{
	DelayCount = ms * 1000;
	while (DelayCount);
}

	
/* ==============================================================================================
	// Counter
============================================================================================== */

volatile unsigned int Counter[10];
	   
void Counter_Task(unsigned int elapsed_us)
{
	Counter[0] += elapsed_us;
	Counter[1] += elapsed_us;
	Counter[2] += elapsed_us;
}

void Counter_Reset(uint8_t index)
{
	Counter[index] = 0;
}

unsigned int Counter_Get(uint8_t index)
{
	return Counter[index];
}

	
/* ==============================================================================================
	// Event
============================================================================================== */
			
volatile unsigned int EventCounter = 0;
void (*EventFuncPointer)(void);

void Event_Set(void (*func)(void), unsigned int ms)
{
	EventFuncPointer = func;
	EventCounter = ms * 1000;
}

uint8_t Event_IsClear(void)
{
//	if (EventCounter == 0)
//		return 1;
//	else
//		return 0;
	return !EventCounter;
}

void Event_Task(unsigned int elapsed_us)
{
	if (EventCounter > 0)
	{
		if (EventCounter <= elapsed_us)
		{
			EventFuncPointer();
			EventCounter = 0;
		}
		else
			EventCounter -= elapsed_us;
	}
}


/* ==============================================================================================
	// Interval
============================================================================================== */
			
unsigned int Interval_Counter[3];
unsigned int Interval_Max[3];
uint8_t Interval_Flag[3];

void Interval_Set(uint8_t index, unsigned int interval)
{
	Interval_Flag[index] = 0;
	Interval_Counter[index] = 0;
	Interval_Max[index] = interval * 1000;
}

uint8_t Interval_Check(uint8_t index)
{
	uint8_t flag = Interval_Flag[index];
	Interval_Flag[index] = 0;
	return flag;
}

void Interval_Task(unsigned int elapsed_us)
{ 	   
	Interval_Counter[0] += elapsed_us;
	Interval_Counter[1] += elapsed_us;
	Interval_Counter[2] += elapsed_us;	

	if (Interval_Counter[0] >= Interval_Max[0])	
	{
		Interval_Counter[0] = 0;
		Interval_Flag[0] = 1;
	}  
	if (Interval_Counter[1] >= Interval_Max[1])	
	{
		Interval_Counter[1] = 0;
		Interval_Flag[1] = 1;
	}
	if (Interval_Counter[2] >= Interval_Max[2])	
	{
		Interval_Counter[2] = 0;
		Interval_Flag[2] = 1;
	}
}

				  
/* ==============================================================================================
	// Mathmetical Utility
============================================================================================== */

float MinF(float inA, float inB)
{
	if (inA < inB)
		return inA;
	else
		return inB;
}
 
float MaxF(float inA, float inB)
{
	if (inA > inB)
		return inA;
	else
		return inB;
}

float ConstrainF(float input, float min, float max)
{
	if (input < min)
		return min;
	else if (input > max)
		return max;
	else
		return input;
}

float Deg2RadF(float deg)
{
	return deg * 3.141592f / 180.0f;
}

float Rad2DegF(float rad)
{
	return rad * 180.0f / 3.141592f;
}
