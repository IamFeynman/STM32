#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64Ins.h"


void W25Q64_Init(void) 
{
	MySPI_Init();	
	
}


void W25Q64_ReadID(uint8_t *MID,uint16_t *DID)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_JEDEC_ID);
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	MySPI_Stop();	
}


void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();	
}

void W25Q64_WaitBusy(void)
{
	uint32_t Timerout;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	Timerout =100000;
	while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) ==0x01)
		{
			Timerout--;
			if(Timerout == 0)
				break;
		}
	MySPI_Stop();
}

void W25Q64_PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	MySPI_SwapByte(Address>>16);
	MySPI_SwapByte(Address>>8);
	MySPI_SwapByte(Address);
	for(i=0;i<Count;i++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_SwapByte(Address>>16);
	MySPI_SwapByte(Address>>8);
	MySPI_SwapByte(Address);
	MySPI_Stop();
	
	W25Q64_WaitBusy();
}

void W25Q64_ReadData(uint32_t Address,uint8_t *DataArray,uint32_t Count)
{
	uint32_t i;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address>>16);
	MySPI_SwapByte(Address>>8);
	MySPI_SwapByte(Address);
	for(i=0;i<Count;i++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
}

void W25Q64_WriteStatus(void)//解除写保护
{
    W25Q64_WriteEnable(); //写使能
    
    MySPI_Start();
    MySPI_SwapByte(W25Q64_WRITE_STATUS_REGISTER); //发送写状态寄存器的指令
    MySPI_SwapByte(0x00); //写入状态寄存器1
    MySPI_SwapByte(0x00); //写入状态寄存器2
    MySPI_Stop();
    
    W25Q64_WaitBusy();  //等待忙
}
