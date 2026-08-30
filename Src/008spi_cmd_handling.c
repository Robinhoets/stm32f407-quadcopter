/*
 * 008spi_cmd_handling.c
 *
 *  Created on: Aug 27, 2026
 *      Author: Robert
 */
#include <string.h>
#include "stm32f407xx.h"

/**************************************************************
 * 	Arduino specific area
 */
#define COMMAND_LED_CTRL				0x50
#define COMMAND_SENSOR_READ				0x51
#define COMMAND_LED_READ				0x52
#define COMMAND_PRINT					0x53
#define COMMAND_ID_READ					0x54

#define LED_ON							1
#define LED_OFF							0

#define ANALOG_PIN0						0
#define ANALOG_PIN1						1
#define ANALOG_PIN2						2
#define ANALOG_PIN3						3
#define ANALOG_PIN4						4

#define LED_PIN							9

/*
 *	End Arduino specific area
 *****************************************************************/

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

	// MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

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

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(ackbyte == 0xF5)
	{
		return 1;
	}
	return 0;
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
	uint8_t dummy_write = 0xff;
	uint8_t dummy_read = 0xff;

	GPIO_ButtonInit();

	SPI2_GPIOInits();

	SPI2_Inits();

	SPI_SSOEConfig(SPI2, ENABLE);

	while(1)
	{
		// button press goes to 1 and control goes past loop
		while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) );

		delay();

		// Enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		/*
		 * 	Send Commands
		 * 		1 - CMD_LED_CTRL <pin no (1)>		<value(1)>
		 * 			If slave supports this command, it will send ACK.
		 * 			Clear off the RXNE with receive data.
		 * 			Data is in slave shift register. To move out, send dummy bytes.
		 *		2 - CMD_SENSOR_READ	<analog pin number(1) >
		 *
		 *
		 */
		// --- (1) ---
		uint8_t commandcode = COMMAND_LED_CTRL;
		uint8_t ackbyte;
		uint8_t args[2];

		SPI_SendData(SPI2, &commandcode, 1);
		SPI_ReceiveData(SPI2, &dummy_read, 1);
		SPI_SendData(SPI2, &dummy_write, 1);
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		if( SPI_VerifyResponse(ackbyte) )
		{
			// send arguments
			args[0] = LED_PIN;
			args[1] = LED_ON;
			SPI_SendData(SPI2, args, 2); // may need to change to 2

			// dummy read
			SPI_ReceiveData(SPI2,args,2);
		}

		// --- (2) ---
		while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) );
		delay();
		commandcode = COMMAND_SENSOR_READ;
		SPI_SendData(SPI2, &commandcode, 1);
		SPI_ReceiveData(SPI2, &dummy_read, 1);
		SPI_SendData(SPI2, &dummy_write, 1);
		SPI_ReceiveData(SPI2, &ackbyte, 1);
		if( SPI_VerifyResponse(ackbyte) )
		{
			// send arguments
			args[0] = ANALOG_PIN0;
			SPI_SendData(SPI2, args, 1); // may need to change to 2

			SPI_ReceiveData(SPI2, &dummy_read, 1);
			delay();
			SPI_SendData(SPI2, &dummy_write, 1);

			uint8_t analog_read;
			SPI_ReceiveData(SPI2, &analog_read, 1);
		}



		/*
		 * 	confirm spy is not busy - if returns 1, busy
		 * 	busy is maintainted by stm32 hardware in the SPI_SR BSY register
		 */
		while( SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) );

		// Disable
		SPI_PeripheralControl(SPI2,DISABLE);
	}

	return 0;
}

