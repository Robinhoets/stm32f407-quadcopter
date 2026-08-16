/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Aug 9, 2026
 *      Author: Robert
 */
#include "stm32f407xx_gpio_driver.h"

/*
 * 	Peripheral Clock Setup
 */

/***************************************************************************
 * @fn				- GPIO_PeriClockControl
 *
 * @brief			- Enables or diables the peripheral clock for a given GPIO port.
 *
 * @param[in]		- Base address of the GPIO peripheral.
 * @param[in]		- ENABLE or DISABLE macros.
 *
 * @return			- none
 *
 * @Note			- none
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}
	}
	else
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOF_PCLK_DI();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOF_PCLK_DI();
		}
		else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_DI();
		}
	}
}

/*
 * 	Initialize and De-initialize
 */

/**************************************************************************
 * @fn				- GPIO_Init
 *
 * @brief			- Initialize the given GPIO by configuring it with the settings passed
 * 						in the parameter
 *
 * @param[in]		- *pGPIOHandle points the the GPIO pin we want to initialize
 *
 * @return			- none
 *
 * @Note			-
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;

	// (1) Configure the mode of the GPIO pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		// each pin is two bits. Left shift the pin mode value by 2 * pin number. Ex) 11 << 2 * 1
		temp = ( pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) );
		// clear the register
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		// store value in actual register
		pGPIOHandle->pGPIOx->MODER |= temp;
	}
	else
	{
		/*
		 * 	Interrupt Mode.
		 * 	if Configure FTSR
		 * 	else if Configure RTSR
		 * 	else if Configure FTSR and RTSR
		 */
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Clear the corresponding RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Clear the corresponding FTSR bit
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		/*
		 * 	(2) Configure the GPIO port selection in SYSCFG_EXTICR.
		 * 	ex) PC 13 button for interrupt -> SYSCFG_EXTICR4 -> EXTI13 -> 0010 for PC pin
		 * 	ex) PC13 -> 13/4 = 3 & 13%4 = 1 -> so SYSCFG_EXTICR<3+1> or..ICR[3] & 1st section 0, 1, 2 etc
		 */
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << ( temp2 * 4 );

		// (3) Enable the exti interrupt delivery using IMR (interrupt mask register)
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}

	temp = 0;

	// (2) Configure the speed
	temp = ( pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) );
	// clear the register
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	temp = 0;

	// (3) configure the pull up and pull down settings
	temp = ( pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) );
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->PUPDR |= temp;
	temp = 0;

	// (4) Configure the output type
	temp = ( pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	temp = 0;

	// (5) Configure the alternate functionality
	/*
	 * 	Alternate function only works when pin mode is set to alternate function.
	 * 	in GPIO_RegDef_t data structure, we represent the low with AFR[0] and high with AFR[1]
	 */
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		uint8_t temp1, temp2;
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~( 0xF << ( 4 * temp2 ) );
		pGPIOHandle->pGPIOx->AFR[temp1] |= ( pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << ( 4 * temp2 ) );
	}
}

/***************************************************************************
 * @fn				- GPIO_DeInit
 *
 * @brief			- De-initialize a GPIO at a given address
 *
 * @param[in]		- base address of the GPIO port
 *
 * @return			- none
 *
 * @Note			-
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{

	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}
	else if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}
	else if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}
	else if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}
	else if(pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}
	else if(pGPIOx == GPIOG)
	{
		GPIOF_REG_RESET();
	}
	else if(pGPIOx == GPIOH)
	{
		GPIOF_REG_RESET();
	}
	else if(pGPIOx == GPIOI)
	{
		GPIOI_REG_RESET();
	}
}

/*
 * 	Data read and write
 */

/***************************************************************************
 * @fn				- GPIO_ReadFromInputPin
 *
 * @brief			- Read value from a specific pin.
 *
 * @param[in]		- Address of the register that holds pin
 *
 * @param[in]		- The pin number that holds the value we want to return.
 *
 * @return			- 0 or 1
 *
 * @Note			-
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	/*
	 * 	Shift bit wanted to lsb.
	 * 	Make every bit position 0 except for lsb.
	 */
	value = (uint8_t) ( (pGPIOx->IDR >> PinNumber) & 0x00000001 );
	return value;
}

/***************************************************************************
 * @fn				- GPIO_ReadFromInputPort
 *
 * @brief			- Read all data from port.
 *
 * @param[in]		- Address of port to read from.
 *
 * @return			- uint16_t size of data in port
 *
 * @Note			-
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t)pGPIOx->IDR;
	return value;
}

/***************************************************************************
 * @fn				- GPIO_WriteToOutputPin
 *
 * @brief			- Write to a specific pin 1 or 0 based on Value.
 *
 * @param[in]		- Address of port.
 *
 * @param[in]		- The pin in the port.
 *
 * @param[in]		- Whether to set or clear the pin.
 *
 * @return			- none
 *
 * @Note			-
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		// write 1
		pGPIOx->ODR |= ( 1 << PinNumber );
	}
	else
	{
		// write 0
		pGPIOx->ODR &= ~( 1 << PinNumber );
	}
}

/***************************************************************************
 * @fn				- GPIO_WriteToOutputPort
 *
 * @brief			- Address of port.
 *
 * @param[in]		- Value to be put in ODR register.
 *
 * @return			- none
 *
 * @Note			-
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}

/***************************************************************************
 * @fn				- GPIO_ToggleOutputPin
 *
 * @brief			- Change the state of the pin to the opposite of it's current state.
 *
 * @param[in]		- Address of port.
 *
 * @param[in]		- Which pin to toggle.
 *
 * @return			- none
 *
 * @Note			-
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= ( 1 << PinNumber );
}

/*
 * 	IRQ Configuration and ISR handling
 */

/***************************************************************************
 * @fn				-
 *
 * @brief			-
 *
 * @param[in]		-
 *
 * @return			-
 *
 * @Note			-
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi)
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
		else if(IRQNumber >= 64 && IRQnumber < 96)
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
		else if(IRQNumber >= 64 && IRQnumber < 96)
		{
			*NVIC_ICER2 |= ( 1 << IRQNumber % 64);
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
 * @return			-
 *
 * @Note			-
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{

}

