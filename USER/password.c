#include "password.h"
#include "string.h"


void password_init(){

}

void password_save(char pass[]){
	Flash_Unlock();
	Flash_Erase(PASSWORD_ADDRESS);
	Flash_Write_String(PASSWORD_ADDRESS, pass);
	Flash_Lock();	
}
uint8_t password_check(char pass[]){
	char buff[20];
	Flash_Read_String_1byte(PASSWORD_ADDRESS, buff);
	return strcmp(buff, pass) == 0;
}


uint8_t add_id_card(uint8_t CardID[]){
//	Flash_Unlock();
//	Flash_Erase(ID_CARD_BASE);
	uint8_t * ptr = (uint8_t *)ID_CARD_BASE;
	uint16_t i = 0;
	while(1){
		if(*ptr == 0xff  )
		{	
			if(*((uint32_t *)ptr) == 0xffffffff)
			{
				Flash_Unlock();
//				Flash_Erase(ID_CARD_BASE);
				Flash_Write_Byte_Array((uint32_t)(ptr), CardID, 5);
				Flash_Lock();	
				return 1;
			}				
		}
		ptr += 6;
		i++;
		if(i >= 100)
		{
			return 0;
		}
	}
}
uint8_t check_id_card(uint8_t CardID[]){
	uint8_t * ptr = (uint8_t *)ID_CARD_BASE;		
	uint16_t i = 0;
	while(1){
		if(*ptr == 0xff  )
		{	
			if(*((uint32_t *)ptr) == 0xffffffff)
			{
				return 0;
			}				
		}
		else if(ptr[0] == CardID[0] && ptr[1] == CardID[1] && ptr[2] == CardID[2] && ptr[3] == CardID[3] && ptr[4] == CardID[4]) 
		{
			return 1;
		}
		ptr += 6;
		i++;
		if(i >= 100)
		{
			return 0;
		}		
	}
}