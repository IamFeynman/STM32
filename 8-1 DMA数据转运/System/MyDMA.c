#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;
void MyDMA_Init(uint32_t AddrA,uint32_t AddrB,uint16_t Size)
{
	MyDMA_Size = Size;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;//外设配置
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;//存储器配置
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_BufferSize = Size;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//方向
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;//memory to memory 选择是否为存储器到存储器的模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//循环？
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//优先级
	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}


void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,MyDMA_Size);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
	
}

