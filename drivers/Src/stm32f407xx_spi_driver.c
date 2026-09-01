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
	 * 	1 - Enable Peripheral Clock
	 * 	2 - Configure device mode
	 * 	3 - Configure the bus config
	 * 			FD -> bidi mode cleared
	 * 			HD -> bidi mode set
	 * 			Simplex -> bidi mode cleared & RX bit set
	 * 	4 - Configure SclkSpeed
	 * 	5 - Configure DFF
	 * 	6 - Configure CPOL
	 * 	7 - Configure CPHA
	 */
	// (1)
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);
	uint32_t tempreg = 0;
	// (2)
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;
	// (3)
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
	// (4)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;
	// (5)
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;
	// (6)
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;
	//(7)
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

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
 * @fn				- SPI_ReceiveData
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
	while(Len > 0)
	{
		/*
		 *  (1)
		 * 	wait until rxne is set
		 * 	checks status register
		 * 	if expression is not 0, rxne is not set - hang
		 */
		while( SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG) == FLAG_RESET );

		/*
		 *  (2)
		 *  if -> 16 bit dff
		 *  else -> 8 bit dff
		 */
		if( (pSPIx->CR1 & (1 << SPI_CR1_DFF ) ) )
		{
			// load data from DR to rx
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			// decrease Len bc sent out two bytes of data
			Len--;
			Len--;
			// point to next data item
			(uint16_t*)pRxBuffer++;
		}
		else
		{
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			Len--;
			pRxBuffer++;
		}


	}
}

/***************************************************************************
 * @fn				- SPI_PeripheralControl
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
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx,uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

/***************************************************************************
 * @fn				- SPI_SSIConfig
 *
 * @brief			- Makes NSS signal internally high. If not set, master
 * 						gets cleared and device becomes slave because of MODF
 * 						error.
 *
 * @param[in]		-
 * @param[in]		-
 *
 * @return			-
 *
 * @Note			-
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx,uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

/***************************************************************************
 * @fn				- SPI_SSOEConfig
 *
 * @brief			- Configure the Slave Select Output Enabled so that we
 * 						can pull NSS high and low.
 *
 * @param[in]		- SPI_RegDef_t
 *
 * @param[in]		- EnOrDi
 *
 * @return			- none
 *
 * @Note			-
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx,uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	}
	else
	{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}

/*
 * 	IRQ Configuration and ISR handling
 */

/***************************************************************************
 * @fn				- SPI_IRQConfig
 *
 * @brief			- Enable or disables the correct interrupt.
 *
 * @param[in]		- The interrupt number the user calls.
 *
 * @param[in]		- Whether to enable or disable.
 *
 * @return			- none
 *
 * @Note			-
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		/*
		 * 	if -> ISER0 register
		 * 	else if -> ISER1 register
		 * 	else if -> ISER2 register
		 */
		if(IRQNumber <= 31)
		{
			*NVIC_ISER0 |= ( 1 << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64)
		{
			*NVIC_ISER1 |= ( 1 << IRQNumber % 32);
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			*NVIC_ISER2 |= ( 1 << IRQNumber % 64);
		}
	}
	else
	{
		if(IRQNumber <= 31)
		{
			*NVIC_ICER0 |= ( 1 << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64)
		{
			*NVIC_ICER1 |= ( 1 << IRQNumber % 32);
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			*NVIC_ICER2 |= ( 1 << IRQNumber % 64);
		}
	}
}

/***************************************************************************
 * @fn				- SPI_IRQPriorityConfig
 *
 * @brief			- Set the priority of an interrupt.
 *
 * @param[in]		- The number of the interrupt being called.
 *
 * @param[in]		- The priority of the interrupt to be set.
 *
 * @return			- none
 *
 * @Note			-
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	/*
	 * 	1 - find IPR register
	 */
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	// shift by 4 to put into top 4 bits of 8 bit address
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
//	*(NVIC_PR_BASE_ADDR + (iprx * 4)) |= ( IRQPriority << shift_amount );
	*(NVIC_PR_BASE_ADDR + iprx) |= ( IRQPriority << shift_amount );
}
/***************************************************************************
 * @fn				- SPI_SendDataIT
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
void SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	/*
	 * 	1 - Save the Tx buffer address and Len information in some global variables
	 * 	2 - Mark the SPI state as busy in transmission so that no other code can
	 * 		take over the same SPI peripheral until transmission is over
	 * 	3 - Enable the TXEIE control bit to get interrupt whenever the TXE flas is
	 * 		set in SR
	 * 	4 - Data transmission will be handle by the ISR code
	 */

	// (1)

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
void SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
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
