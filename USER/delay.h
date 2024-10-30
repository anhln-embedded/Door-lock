/*
  ******************************************************************************
  * @file		abc.h                                                              *
  * @author	Luu Ngoc Anh                                                       *
  * @date		00/00/0000                                                         *
  ******************************************************************************
*/
	
#ifndef __DELAY__H
#define __DELAY__H

#ifdef __cplusplus
 extern "C" {
#endif
void DelayInit(void);
void DelayUs(uint32_t vrTime);
void DelayMs(uint32_t vrTime);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
