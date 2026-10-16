/*
 * stm32f407xx_I2C_driver.c
 *
 *  Created on: Oct 15, 2026
 *      Author: Robert
 */

#include "stm32f407xx_i2c_driver.h"

/*
 * 	Peripheral Clock setup
 */
/***************************************************************************
 * @fn				- I2C_PeriClockControl
 *
 * @brief			- enable the clock on a specific i2c peripheral
 *
 * @param[in]		- *pI2CX - pointer to the i2c struct
 * @param[in]		- EnorDi - enable or disable the peripheral
 *
 * @return			- void
 *
 * @Note			-
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pI2Cx== I2C1)
		{
			I2C1_PCLK_EN();
		}
		else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}
	}
	else
	{
		if(pI2Cx== I2C1)
		{
			I2C1_PCLK_DI();
		}
		else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_DI();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_DI();
		}
	}
}

/***************************************************************************
 * @fn				- I2C_DeInit
 *
 * @brief			- De-initialize a I2C at a given address
 *
 * @param[in]		- base address of the I2C port
 *
 * @return			- none
 *
 * @Note			-
 */
void i2c_DeInit(I2C_RegDef_t *pI2Cx)
{
	if(pI2Cx == I2C1)
	{
		I2C1_PCLK_DI();
	}
	else if(pI2Cx == I2C2)
	{
		I2C2_PCLK_DI();
	}
	else if(pI2Cx == I2C3)
	{
		I2C3_PCLK_DI();
	}
}

/*
 * 	IRQ Configuration and ISR handling
 */

/***************************************************************************
 * @fn				- I2C_IRQConfig
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
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
 * @fn				- I2C_IRQPriorityConfig
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
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	/*
	 * 	1 - find IPR register
	 */
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	// shift by 4 to put into top 4 bits of 8 bit address
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) |= ( IRQPriority << shift_amount );
}

/***************************************************************************
 * @fn				- I2C_PeripheralControl
 *
 * @brief			- Enable or disable the I2C peripheral.
 *
 * @param[in]		- *pI2Cx - pointer to I2C_RegDef_t
 * @param[in]		- EnOrDi - enable or disable
 *
 * @return			- void
 *
 * @Note			-
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx,uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	}
	else
	{
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
	}
}
