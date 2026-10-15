/*
 * stm32f407xx_spi_driver.c
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
