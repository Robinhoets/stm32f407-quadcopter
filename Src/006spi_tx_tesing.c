/*
 * 006spi_tx_tesing.c
 *
 *  Created on: Aug 25, 2026
 *      Author: Rober
 */

#include <string.h>
#include "stm32f407xx.h"

/*
 * SPI2:
 * 	PB14 -> MISO
 * 	PB15 ->
 * 	PB13 -> SCLK
 * 	PB12 -> NSS
 *	ALT function mode : 5
 */

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	// MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

//	// MISO
//	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
//	GPIO_Init(&SPIPins);
//
//	// NSS
//	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
//	GPIO_Init(&SPIPins);
}

void SPI2_Inits()
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV256;	// 8MHz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI2handle);
}


int main()
{

	char user_data[] = "Hello World";

	SPI2_GPIOInits();

	SPI2_Inits();

	SPI_SSIConfig(SPI2, ENABLE);

	// do inits before enabling the SPI peripheral
	SPI_PeripheralControl(SPI2, ENABLE);

	SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

	/*
	 * 	confirm spy is not busy - if returns 1, busy
	 * 	busy is maintainted by stm32 hardware in the SPI_SR BSY register
	 */
	while( SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) );
	// Disable
	SPI_PeripheralControl(SPI2,Disable);

	while(1);
//	while(1)
//	{
//	    SPI_SendData(SPI2, (uint8_t *)user_data, strlen(user_data));
//
//	    while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));
//
//	    for(volatile uint32_t i = 0; i < 1000000; i++);
//	}
	return 0;
}
//int main(void)
//{
//    GPIO_Handle_t testPin = {0};
//
//    testPin.pGPIOx = GPIOB;
//    testPin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
//    testPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
//    testPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
//    testPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
//    testPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
//
//    GPIO_Init(&testPin);
//
//    while(1)
//    {
//        GPIO_ToggleOutputPin(GPIOB, GPIO_PIN_NO_13);
//
//        for(volatile uint32_t delay = 0; delay < 100000; delay++);
//    }
//}
