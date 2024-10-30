#include "stm32f10x.h"                  // Device header
#include "lcd_i2c.h"
#include "delay.h"
#include "stdio.h"

int t = 1;

int main()
{
	lcd_init();
	while(1)
	{
		lcd_printf(0, 0, "chao %d", t);
		t++;
		DelayMs(1000);
	}
}

