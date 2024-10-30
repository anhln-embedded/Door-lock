#include "stm32f10x.h"                  // Device header
#include "delay.h"  

volatile int usTicks = 0;

void SysTick_Handler(void) // Function SysTick_Handler will be called every 1 us
{
	if(usTicks != 0)
	{
		usTicks --;
	}
}

void DelayInit(void)
{
	// Update SystemCoreClock value
	SystemCoreClockUpdate();
	// configure a tick to be 1 us
	SysTick_Config(SystemCoreClock / 1000000);
}
void DelayUs(uint32_t vrTime)
{
	// Reload us value
	usTicks = vrTime;
	// Wait until usTick reach zero
	while(usTicks);
}
void DelayMs(uint32_t vrTime)
{
	while(vrTime--) // Wait vrTime reach zero
	{
		// delay 1ms
		DelayUs(1000);
	}
}