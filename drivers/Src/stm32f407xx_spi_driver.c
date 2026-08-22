/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Aug 20, 2026
 *      Author: Robert
 */
#include "stm32f407xx_spi_driver.h"
/*
 * 	Peripheral Clock setup
 */
/***************************************************************************
 * @fn				- SPI_PeriClockControl
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @Note			-
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx== SPI1)
		{
			SPI1_PCLK_EN();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
	}
	else
	{
		if(pSPIx== SPI1)
		{
			SPI1_PCLK_DI();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
	}
}

/*
 * 	Initialize and De-initialize
 */
/***************************************************************************
 * @fn				- SPI_Init
 *
 * @brief			- Initialize the given SPI by configuring it with the settings passed
 * 						in the parameter
 *
 * @param[in]		- *pGPIOHandle points the the SPI pin we want to initialize
 *
 * @return			- none
 *
 * @Note			-
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	/*
	 * 	1 - Configure device mode
	 * 	2 - Configure the bus config
	 * 			FD -> bidi mode cleared
	 * 			HD -> bidi mode set
	 * 			Simplex -> bidi mode cleared & RX bit set
	 * 	3 - Configure SclkSpeed
	 * 	4 - Configure DFF
	 * 	5 - Configure CPOL
	 * 	6 - Configure CPHA
	 */
	uint32_t tempreg = 0;
	// (1)
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;
	// (2)
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		tempreg &= ~(1 << SPI_CR1_BIDI_MODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		tempreg |= ~(1 << SPI_CR1_BIDI_MODE);

	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		tempreg &= ~(1 << SPI_CR1_BIDI_MODE);
		tempreg |= ~(1 << SPI_CR1_RX_ONLY);
	}
	// (3)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;
	// (4)
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;
	// (5)
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;
	//(6)
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	pSPIHandle->pSPIx->CR1 = tempreg;

}
/***************************************************************************
 * @fn				- SPI_DeInit
 *
 * @brief			- De-initialize a SPI at a given address
 *
 * @param[in]		- base address of the SPI port
 *
 * @return			- none
 *
 * @Note			-
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if(pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}
	else if(pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}
	else if(pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}
}

/*
 * 	Data Send and Receive
 * 	(aside: blocking is non-interrupt based & non-blocking is interrupt based)
 */
/***************************************************************************
 * @fn				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @Note			-
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{

}
/***************************************************************************
 * @fn				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @Note			-
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{

}

/*
 * 	IRQ Configuration and ISR Handling
 */
/***************************************************************************
 * @fn				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @Note			-
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

}
/***************************************************************************
 * @fn				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @Note			-
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{

}
/***************************************************************************
 * @fn				-
 *
 * @brief			-
 *
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @Note			-
 */
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{

}
