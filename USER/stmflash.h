/*
  ******************************************************************************
  * @file		flash.h                                                            *
  * @author	Luu Ngoc Anh                                                       *
  * @date		08/09/2022                                                         *
  ******************************************************************************
*/
	
#ifndef __ABC__
#define __ABC__

#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f10x.h"                  // Device header

#define MEMORY_START_ADDRESS 0x0800FC00

typedef struct{
	unsigned int ersionCode;
	char Date[24];
	char Human[24];
}Information;

void Flash_Unlock(void);
void Flash_Lock(void);
void Flash_Erase(uint32_t Address);

void Flash_Write_2byte(uint32_t Address, uint16_t vr_Value);
void Flash_Write_4byte(uint32_t Address, uint32_t vr_Value);
void Flash_Write_8byte(uint32_t Address, uint64_t vr_Value);
void Flash_Write_Byte_Array(uint32_t Address, uint8_t *buff, uint16_t Length);
void Flash_Write_String(uint32_t Address, char *buff);
void Flash_Write_Struct(uint32_t Address, Information STR);

uint16_t Flash_Read_2byte(uint32_t Address);
uint32_t Flash_Read_4byte(uint32_t Address);
uint64_t Flash_Read_8byte(uint32_t Address);
void Flash_Read_Byte_Array(uint32_t Address, uint8_t *buff, uint16_t Length);
void Flash_Read_String_1byte(uint32_t Address, char *buff);
Information Flash_Read_Struct(uint32_t Address);

void DisplayStruct(Information STR);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
