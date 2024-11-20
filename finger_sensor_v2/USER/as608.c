
#include <string.h>
#include "delay.h"
#include "usart2.h"
#include "as608.h"

u32 AS608Addr = 0XFFFFFFFF;


void PS_StaGPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);		  
}


static void MYUSART_SendData(u8 data)
{
	while ((USART2->SR & 0X40) == 0)
		;
	USART2->DR = data;
}

static void SendHead(void)
{
	MYUSART_SendData(0xEF);
	MYUSART_SendData(0x01);
}

static void SendAddr(void)
{
	MYUSART_SendData(AS608Addr >> 24);
	MYUSART_SendData(AS608Addr >> 16);
	MYUSART_SendData(AS608Addr >> 8);
	MYUSART_SendData(AS608Addr);
}

static void SendFlag(u8 flag)
{
	MYUSART_SendData(flag);
}

static void SendLength(int length)
{
	MYUSART_SendData(length >> 8);
	MYUSART_SendData(length);
}

static void Sendcmd(u8 cmd)
{
	MYUSART_SendData(cmd);
}

static void SendCheck(u16 check)
{
	MYUSART_SendData(check >> 8);
	MYUSART_SendData(check);
}


static u8 *JudgeStr(u16 waittime)
{
	char *data;
	u8 str[8];
	str[0] = 0xef;
	str[1] = 0x01;
	str[2] = AS608Addr >> 24;
	str[3] = AS608Addr >> 16;
	str[4] = AS608Addr >> 8;
	str[5] = AS608Addr;
	str[6] = 0x07;
	str[7] = '\0';
	USART2_RX_STA = 0;
	while (--waittime)
	{
		delay_ms(1);
		if (USART2_RX_STA & 0X8000) // 接收到一次数据
		{
			USART2_RX_STA = 0;
			data = strstr((const char *)USART2_RX_BUF, (const char *)str);
			if (data)
				return (u8 *)data;
		}
	}
	return 0;
}


u8 PS_GetImage(void)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x01);
	temp = 0x01 + 0x03 + 0x01;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_GenChar(u8 BufferID)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); 
	SendLength(0x04);
	Sendcmd(0x02);
	MYUSART_SendData(BufferID);
	temp = 0x01 + 0x04 + 0x02 + BufferID;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_Match(void)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x03);
	temp = 0x01 + 0x03 + 0x03;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_Search(u8 BufferID, u16 StartPage, u16 PageNum, SearchResult *p)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x08);
	Sendcmd(0x04);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage >> 8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum >> 8);
	MYUSART_SendData(PageNum);
	temp = 0x01 + 0x08 + 0x04 + BufferID + (StartPage >> 8) + (u8)StartPage + (PageNum >> 8) + (u8)PageNum;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
	{
		ensure = data[9];
		p->pageID = (data[10] << 8) + data[11];
		p->mathscore = (data[12] << 8) + data[13];
	}
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_RegModel(void)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x05);
	temp = 0x01 + 0x03 + 0x05;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_StoreChar(u8 BufferID, u16 PageID)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x06);
	Sendcmd(0x06);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(PageID >> 8);
	MYUSART_SendData(PageID);
	temp = 0x01 + 0x06 + 0x06 + BufferID + (PageID >> 8) + (u8)PageID;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_DeletChar(u16 PageID, u16 N)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x07);
	Sendcmd(0x0C);
	MYUSART_SendData(PageID >> 8);
	MYUSART_SendData(PageID);
	MYUSART_SendData(N >> 8);
	MYUSART_SendData(N);
	temp = 0x01 + 0x07 + 0x0C + (PageID >> 8) + (u8)PageID + (N >> 8) + (u8)N;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_Empty(void)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01 + 0x03 + 0x0D;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_WriteReg(u8 RegNum, u8 DATA)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x05);
	Sendcmd(0x0E);
	MYUSART_SendData(RegNum);
	MYUSART_SendData(DATA);
	temp = RegNum + DATA + 0x01 + 0x05 + 0x0E;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_ReadSysPara(SysPara *p)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x0F);
	temp = 0x01 + 0x03 + 0x0F;
	SendCheck(temp);
	data = JudgeStr(1000);
	if (data)
	{
		ensure = data[9];
		p->PS_max = (data[14] << 8) + data[15];
		p->PS_level = data[17];
		p->PS_addr = (data[18] << 24) + (data[19] << 16) + (data[20] << 8) + data[21];
		p->PS_size = data[23];
		p->PS_N = data[25];
	}
	else
	{
		ensure = 0xff;
	}
		
	return ensure;
}


u8 PS_SetAddr(u32 PS_addr)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x07);
	Sendcmd(0x15);
	MYUSART_SendData(PS_addr >> 24);
	MYUSART_SendData(PS_addr >> 16);
	MYUSART_SendData(PS_addr >> 8);
	MYUSART_SendData(PS_addr);
	temp = 0x01 + 0x07 + 0x15 + (u8)(PS_addr >> 24) + (u8)(PS_addr >> 16) + (u8)(PS_addr >> 8) + (u8)PS_addr;
	SendCheck(temp);
	AS608Addr = PS_addr; // 发送完指令，更换地址
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	AS608Addr = PS_addr;
	return ensure;
}


u8 PS_WriteNotepad(u8 NotePageNum, u8 *Byte32)
{
	u16 temp;
	u8 ensure, i;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(36);
	Sendcmd(0x18);
	MYUSART_SendData(NotePageNum);
	for (i = 0; i < 32; i++)
	{
		MYUSART_SendData(Byte32[i]);
		temp += Byte32[i];
	}
	temp = 0x01 + 36 + 0x18 + NotePageNum + temp;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
		ensure = data[9];
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_ReadNotepad(u8 NotePageNum, u8 *Byte32)
{
	u16 temp;
	u8 ensure, i;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x04);
	Sendcmd(0x19);
	MYUSART_SendData(NotePageNum);
	temp = 0x01 + 0x04 + 0x19 + NotePageNum;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
	{
		ensure = data[9];
		for (i = 0; i < 32; i++)
		{
			Byte32[i] = data[10 + i];
		}
	}
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_HighSpeedSearch(u8 BufferID, u16 StartPage, u16 PageNum, SearchResult *p)
{
	u16 temp;
	u8 ensure;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x08);
	Sendcmd(0x1b);
	MYUSART_SendData(BufferID);
	MYUSART_SendData(StartPage >> 8);
	MYUSART_SendData(StartPage);
	MYUSART_SendData(PageNum >> 8);
	MYUSART_SendData(PageNum);
	temp = 0x01 + 0x08 + 0x1b + BufferID + (StartPage >> 8) + (u8)StartPage + (PageNum >> 8) + (u8)PageNum;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
	{
		ensure = data[9];
		p->pageID = (data[10] << 8) + data[11];
		p->mathscore = (data[12] << 8) + data[13];
	}
	else
		ensure = 0xff;
	return ensure;
}


u8 PS_ValidTempleteNum(u16 *ValidN)
{
	u16 temp;
	u8 ensure = 0xff;
	u8 *data;
	SendHead();
	SendAddr();
	SendFlag(0x01); // 命令包标识
	SendLength(0x03);
	Sendcmd(0x1d);
	temp = 0x01 + 0x03 + 0x1d;
	SendCheck(temp);
	data = JudgeStr(2000);
	if (data)
	{
		ensure = data[9];
		*ValidN = (data[10] << 8) + data[11];
	}
	else
		ensure = 0xff;

	if (ensure == 0x00)
	{
		// printf("\r\n有效指纹个数=%d\r\n",(data[10]<<8)+data[11]);
		return ensure;
	}
	else
		// printf("\r\n%s",EnsureMessage(ensure));
		return ensure;
}


u8 PS_HandShake(u32 *PS_Addr)
{
	SendHead();
	SendAddr();
	MYUSART_SendData(0X01);
	MYUSART_SendData(0X00);
	MYUSART_SendData(0X00);
	delay_ms(200);
	if (USART2_RX_STA & 0X8000) // 接收到数据
	{
		if ( 
		    USART2_RX_BUF[0] == 0XEF && USART2_RX_BUF[1] == 0X01 && USART2_RX_BUF[6] == 0X07)
		{
			*PS_Addr = (USART2_RX_BUF[2] << 24) + (USART2_RX_BUF[3] << 16) + (USART2_RX_BUF[4] << 8) + (USART2_RX_BUF[5]);
			USART2_RX_STA = 0;
			return 0;
		}
		USART2_RX_STA = 0;
	}
	return 1;
}



