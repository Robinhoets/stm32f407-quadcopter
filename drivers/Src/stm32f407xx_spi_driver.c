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
 * @fn				- SPI_GetFlagStatus
 *
 * @brief			- Check to see if a flag is set in Status Register
 *
 * @param[in]		- *pSPIx is the base address
 *
 * @param[in]		- FlagName
 *
 * @return			- True (1) or false (0)
 *
 * @Note			- If SPI_TXE_FLAG ( 1 << SPI_SR_TXE) is set in the
 * 						pSPIx->StatusRegister, return Flag_SET
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{

	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/***************************************************************************
 * @fn				- SPI_SendData
 *
 * @brief			- Blocking call until SPI_TXE flag reset and then transfer
 * 						all bytes into pSPIx->DR.
 *
 * @param[in]		-*pSPIx is the base address
 *
 * @param[in]		-*pTxBuffer is the pointer to the data
 *
 * @param[in]		- Len is the number of bytes to transmit
 *
 * @return			-
 *
 * @Note			- Blocking api. Check len - if 0 exit. Else:
 * 						 (1) Wait until the tx buffer is empty (otherwise data
 * 						 	already in there will be corrupted). Get this info
 * 						 	in the status register SPI_SR - TXE == 1.
 * 						 (2) Check DFF. 0 == 8 -bit. 1 == 16-bit.
 * 						 Load data register (DR) with 1 byte of data.
 * 						 	A write to the data register writes into the Tx
 * 						 	buffer and a read from the data register returns
 * 						 	the value held in the Rx buffer.
 * 						 Increment the buffer address.
 * 						 Decrement Len--.
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		/*
		 *  (1)
		 * 	wait until txe is set
		 * 	checks status register
		 * 	if expression is not 0, txe is not set - hang
		 */
		while( SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET );

		/*
		 *  (2)
		 *  if -> 16 bit dff
		 *  else -> 8 bit dff
		 */
		if( (pSPIx->CR1 & (1 << SPI_CR1_DFF ) ) )
		{
			// load data into the DR. Typecast for two bytes
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			// decrease Len bc sent out two bytes of data
			Len--;
			Len--;
			// point to next data item
			(uint16_t*)pTxBuffer++;
		}
		else
		{
			pSPIx->DR = *((uint8_t*)pTxBuffer);
			Len--;
			pTxBuffer++;
		}


	}
}



/***************************************************************************
 * @fn				-
 *
 * @brief			-
 *
 * @param[in]		-
 *
 * @param[in]		-
 *
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
