#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "usart1.h"
#include "as608.h"
volatile static int a;

// Hàm d? tìm ki?m d?u vân tay (nhu tru?c)
void Search_FR(void) {
    uint8_t res;
    uint16_t pageStart = 0;
    uint16_t pageNum = 2000;
    SearchResult result;

    res = PS_GetImage();
    if (res != 0) {
        Serial_Printf("Khong the thu thap anh dau van tay\n");
        return;
    }
    Serial_Printf("Anh dau van tay thu thap thanh cong\n");

    res = PS_GenChar(CharBuffer1);
    if (res != 0) {
        Serial_Printf("Khong the tao dac trung tu anh\n");
        return;
    }
    Serial_Printf("Dac trung tu anh tao thanh cong\n");

    res = PS_Search(CharBuffer1, pageStart, pageNum, &result);
    if (res == 0) {
        Serial_Printf("Tim thay dau van tay: ID = %d, Diem so = %d\n", result.pageID, result.mathscore);
    } else if (res == 0xFF) {
        Serial_Printf("Khong tim thay dau van tay\n");
    } else {
        Serial_Printf("Loi khi tim kiem dau van tay: Ma loi = %d\n", res);
    }
}

void Delete_FR_By_ID(){

    uint8_t res = PS_Empty();
		if(res == 0){
			Serial_Printf("Xoa thanh cong\n");
		}
		else
		{
			Serial_Printf("Xoa that bai\n");
		}
}

uint8_t PS_AddFinger(u16 PageID) {
    u8 ensure;
    
    // Bu?c 1: Thu th?p hình ?nh d?u vân tay
    ensure = PS_GetImage();
    if (ensure != 0) {
        return ensure;
    }

    // Bu?c 2: T?o d?c trung t? hình ?nh
    ensure = PS_GenChar(CharBuffer1);
    if (ensure != 0) {
        return ensure;
    }

    // Bu?c 3: Luu d?c trung vào b? nh? v?i PageID
    ensure = PS_StoreChar(CharBuffer1, PageID);
    if (ensure != 0) {
        return ensure;
    }

    Serial_Printf("D?u vân tay dã du?c luu vào PageID %d\n", PageID);

    return 0; // Thành công
}

int main(){
	Serial_Init();
	delay_init();
	usart2_init(57600);
	uint16_t valid;
	
	while(1){
	//		PS_AddFinger(10);
		Search_FR();
//		Delete_FR_By_ID();
		delay_ms(500);
	}
}
