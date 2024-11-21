#include "stmflash.h"
#include "string.h"
#include "stdio.h"

void Flash_Unlock(void)
{
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;	
}
void Flash_Lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}
void Flash_Erase(uint32_t Address)
{
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = Address;
	FLASH->CR |= FLASH_CR_STRT;
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->CR &= ~FLASH_SR_BSY;
	FLASH->CR &= ~FLASH_CR_PER;	
}

void Flash_Write_2byte(uint32_t Address, uint16_t vr_Value)
{
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->CR |= FLASH_CR_PG;
	*(volatile uint16_t*)Address = vr_Value;
	while(FLASH->SR & FLASH_SR_BSY);
	FLASH->CR &= ~FLASH_CR_PG;	
}
void Flash_Write_4byte(uint32_t Address, uint32_t vr_Value)
{
	uint16_t data;
	data =  (uint16_t)vr_Value;
	Flash_Write_2byte(Address,data);
	data = (uint16_t)(vr_Value >> 16);
	Flash_Write_2byte(Address + 2,data);	
}

void Flash_Write_8byte(uint32_t Address, uint64_t vr_Value)
{
	uint32_t data;
	data = (uint32_t) vr_Value;
	Flash_Write_4byte(Address, data);
	data = (uint32_t)(vr_Value >> 32);
	Flash_Write_4byte(Address + 4, data);
}

void Flash_Write_Byte_Array(uint32_t Address, uint8_t *buff, uint16_t Length)
{
	uint16_t i;
	for(i=0; i < (Length/2) + (Length % 2); i++)
	{
		uint16_t data = buff[i * 2] | (uint16_t)buff[i * 2 + 1] << 8;
		Flash_Write_2byte(Address, data);
		Address = Address + 2;
	}	
}
void Flash_Write_String(uint32_t Address, char *buff)
{
	uint16_t len = strlen(buff);
	Flash_Write_Byte_Array(Address,(uint8_t *) buff, len+1);	
}
void Flash_Write_Struct(uint32_t Address, Information STR)
{
	Flash_Unlock();
	Flash_Write_4byte(Address, STR.ersionCode);
	Address = Address + sizeof(STR.ersionCode);
	Flash_Write_String(Address,STR.Date);
	Address = Address + sizeof(STR.Date);
	Flash_Write_String(Address,STR.Human);
	Address = Address + sizeof(STR.Human);	
}

uint16_t Flash_Read_2byte(uint32_t Address)
{
	return *(volatile uint16_t*)Address;
}
uint32_t Flash_Read_4byte(uint32_t Address)
{
	return *(volatile uint32_t*)Address;
}

uint64_t Flash_Read_8byte(uint32_t Address)
{
	return *(volatile uint64_t*)Address;
}

void Flash_Read_Byte_Array(uint32_t Address, uint8_t *buff, uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < (Length / 2); i++)
	{
		uint16_t data = Flash_Read_2byte(Address);
		buff[i * 2]   = (uint8_t)data;
		buff[i * 2 + 1] = (uint8_t) (data >> 8);
		Address = Address + 2;
	}	
}
void Flash_Read_String_1byte(uint32_t Address, char *buff)
{
	uint16_t i = 0;
	while(1)
	{
		*(uint8_t *)(buff + i) = *(uint8_t*)(Address + i);
		if(*(buff + i) == 0)
			break;
		i++;
	}	
}
Information Flash_Read_Struct(uint32_t Address)
{
	Information temp;
	temp.ersionCode = Flash_Read_4byte(Address);
	Address = Address + sizeof(temp.ersionCode);
	Flash_Read_String_1byte(Address,temp.Date);
	Address = Address + sizeof(temp.Date);
	Flash_Read_String_1byte(Address,temp.Human);
	return temp;	
}

void DisplayStruct(Information STR)
{
	printf("ersionCode :%d\n", STR.ersionCode);
	printf("Date : %s\n",STR.Date);
	printf("Human: %s\n",STR.Human);	
}
