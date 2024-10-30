#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

void SysTick_Init(void);
uint32_t SysTick_GetTick(void);
void SysTick_DelayMs(uint32_t delayTime);

#endif /* INC_SYSTICK_H_ */
