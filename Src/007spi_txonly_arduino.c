/*
 * 006spi_tx_tesing.c
 *
 *  Created on: Aug 25, 2026
 *      Author: Rober
 */

#include <string.h>
#include "stm32f407xx.h"

void delay(void)
{
	for(uint32_t i=0; i<500000/2; i++);
}


/*
 * SPI2:
 * 	PB14 -> MISO
 * 	PB15 -> MOSI
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
//	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
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
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
}

void SPI2_Inits()
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
//	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;	// 2MHz
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;	// 2MHz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;					// Hardware slave

	SPI_Init(&SPI2handle);
}

void GPIO_ButtonInit(void)
{

	GPIO_Handle_t GPIObtn;
	// Button Configuration
	GPIObtn.pGPIOx = GPIOA;
	GPIObtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIObtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIObtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIObtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
//	GPIObtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_Init(&GPIObtn);
}

/*
 * 	In Master, if SSM = 0
 * 		and SPE pulled to 1 -> NSS = 0
 * 		or SPE pulled to 0 -> NSS = 1
 * 	SSOE must be enabled to get above results
 * 	(Slave Select Output Enabled)
 */
int main()
{

	char user_data[] = "Hello World";

	GPIO_ButtonInit();

	SPI2_GPIOInits();

	SPI2_Inits();

	SPI_SSOEConfig(SPI2, ENABLE);

	while(1)
	{
		// button press goes to 1 and control goes past loop
		while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) );

		delay();
	//	SPI_SSIConfig(SPI2, ENABLE);

		// do inits before enabling the SPI peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		/*
		 * 	Send data section.
		 * 	Arduino expects 1 byte of information and then data
		 */
		uint8_t dataLen = strlen(user_data);
		SPI_SendData(SPI2, &dataLen, 1);
		SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

		/*
		 * 	confirm spy is not busy - if returns 1, busy
		 * 	busy is maintainted by stm32 hardware in the SPI_SR BSY register
		 */
		while( SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) );

		// Disable
		SPI_PeripheralControl(SPI2,DISABLE);
	}
//
//	while(1)
//	{
//	    while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
//
//	    delay();
//
//	    // Enable SPI. With SSOE enabled, NSS should go LOW.
//	    SPI_PeripheralControl(SPI2, ENABLE);
//
//	    // TEMPORARY DEBUG TEST:
//	    // Give Arduino time to notice SS going LOW.
////	    for(volatile uint32_t i = 0; i < 50000; i++);
//
//	    uint8_t dataLen = strlen(user_data);
//
//	    SPI_SendData(SPI2, &dataLen, 1);
//
//	    SPI_SendData(SPI2, (uint8_t*)user_data, dataLen);
//
//	    while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));
//
//	    SPI_PeripheralControl(SPI2, DISABLE);
//	}

	return 0;
}

