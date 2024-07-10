#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"

int main()
{
	OLED_Init();
	Serial_Init();
	LED_Init();
	
	OLED_ShowString(1,1,"TxPacket:");
	OLED_ShowString(3,1,"RxPacket:");

	while(1)
	{	
		if(Serial_RxFlag == 1)
		{
			OLED_ShowString(4,1,"						");
			OLED_ShowString(4,1,Serial_RxPacket);
			if(strcmp(Serial_RxPacket,"LED_ON")==0)
			{
				LED1_ON();
				Serial_SendString("LED_on_OK\r\n");
				OLED_ShowString(2,1,"				");
				OLED_ShowString(2,1,"LED_on_OK");
			}
			else if(strcmp(Serial_RxPacket,"LED_OFF")==0)
			{
				LED1_OFF();
				Serial_SendString("LED_OFF_OK\r\n");
				OLED_ShowString(2,1,"				");
				OLED_ShowString(2,1,"LED_OFF_OK");
			}
			else{
				Serial_SendString("ERROR_COMMAND.\r\n");
				OLED_ShowString(2,1,"				");
				OLED_ShowString(2,1,"ERROR_COMMAND.");
			}
			Serial_RxFlag = 0;
		}
	}
		

}
