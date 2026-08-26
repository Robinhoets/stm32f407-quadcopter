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
 * 	PB15 -> MOSI
 * 	PB13 -> SCLK
 * 	PB12 -> NSS
 *	ALT function mode : 5
 */

/*
 * 	In Master, if SSM = 0
 * 		and SPE pulled to 1 -> NSS = 0
 * 		or SPE pulled to 0 -> NSS = 1
 * 	In Slave, SSOE must be enabled to get above results
 * 	(Slave Select Output Enabled)
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
	// NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);
}

void SPI2_Inits()
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;	// 2MHz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;					// Hardware slave

	SPI_Init(&SPI2handle);
}


int main()
{

	char user_data[] = "Hello World";

	SPI2_GPIOInits();

	SPI2_Inits();

//	SPI_SSIConfig(SPI2, ENABLE);

	// do inits before enabling the SPI peripheral
	SPI_PeripheralControl(SPI2, ENABLE);

	SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

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

