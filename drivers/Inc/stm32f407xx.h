/*
 * stm32f407xx.h
 *
 *  Created on: Aug 7, 2026
 *      Author: Robert
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>

#define __vo volatile	// shorthand for volatile

/*
 * 	Base addresses of Flash and SRAM memories
 * 	Reference Manual:
 * 		FLASH_BASEADDR: Table 5. Flash module organization (STM32F40x and STM32F41x)
 * 		SRAM1_BASEADDR: Table 3. Memory mapping vs. Boot mode/physical remap
 *						in STM32F405xx/07xx and STM32F415xx/17xx
 *		SRAM2:			(2) Memory and bus Architecture -> (2.2) Memory Organization -> System Memory
 */
#define FLASH_BASEADDR				0x08000000U
#define SRAM1_BASEADDR				0x20000000U
#define SRAM2_BASEADDR				0x2001C000U
#define ROM_BASEADDR				0x1FFF0000U
#define SRAM 						SRAM1_BASEADDR


/*
 * 	AHBx and APBx Bus Peripheral base addresses
 *	Reference Manual:
 *		Table 2.3 - beginning of APB1 is 0x4000 0000 - TIM2
 */
#define PERIPH_BASEADDR				0x40000000U
#define APB1PERIPH_BASEADDR			PERIPH_BASE
#define APB2PERIPH_BASEADDR			0x40010000U
#define AHB1PERIPH_BASEADDR			0x40020000U
#define AHB2PERIPH_BASEADDR			0x50000000U

/*
 * 	Base addresses of peripherals on AHB1 bus
 */
#define GPIOA_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR				(AHB1PERIPH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1000)
#define GPIOF_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1400)
#define GPIOG_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1800)
#define GPIOH_BASEADDR				(AHB1PERIPH_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR				(AHB1PERIPH_BASEADDR + 0x2000)
#define RCC_BASEADDR				(AHB1PERIPH_BASEADDR + 0x3800)

/*
 * 	Base addresses of peripherals on APB1
 */
#define I2C1_BASEADDR				(APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR				(APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR				(APB1PERIPH_BASEADDR + 0x5C00)
#define SPI2_BASEADDR				(APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASEADDR				(APB1PERIPH_BASEADDR + 0x3C00)
#define USART2_BASEADDR				(APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASEADDR				(APB1PERIPH_BASEADDR + 0x4800)
#define UART4_BASEADDR				(APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASEADDR				(APB1PERIPH_BASEADDR + 0x5000)

/*
 * 	Base addresses of peripherals on APB2
 */
#define EXTI_BASEADDR				(APB2PERIPH_BASEADDR + 0x3C00)
#define SPI1_BASEADDR				(APB2PERIPH_BASEADDR + 0x3000)
#define USART1_BASEADDR				(APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASEADDR				(APB2PERIPH_BASEADDR + 0x1400)
#define SYSCFG_BASEADDR				(APB2PERIPH_BASEADDR + 0x3800)


/*************************************peripheral register definition structures*******************************************/

typedef struct
{
	__vo uint32_t MODER;				/*!< GPIO port mode register,										Address offset: 0x00 >*/
	__vo uint32_t OTYPER;				/*!< GPIO port output type register,								Address offset: 0x04 >*/
	__vo uint32_t OSPEEDR;				/*!< GPIO port output speed register,								Address offset: 0x08 >*/
	__vo uint32_t PUPDR;				/*!< GPIO port pull-up/pull-down register,							Address offset: 0x0C >*/
	__vo uint32_t IDR;					/*!< GPIO port input data register,									Address offset: 0x10 >*/
	__vo uint32_t ODR;					/*!< GPIO port output data register,								Address offset: 0x14 >*/
	__vo uint32_t BSRR;					/*!< GPIO port bit set/reset register,								Address offset: 0x18 >*/
	__vo uint32_t LCKR;					/*!< GPIO port configuration lock register,							Address offset: 0x1C >*/
	__vo uint32_t AFR[2];				/*!< GPIO alternate function low and high registers,				Address offset: 0x20 and 0x24 >*/
}GPIO_RegDef_t;

typedef struct
{
	__vo uint32_t CR;					/*!< RCC clock control register,									Address offset: 0x00 >*/
	__vo uint32_t PLLCFGR;				/*!< RCC PLL configuration register,								Address offset: 0x04 >*/
	__vo uint32_t CFGR;					/*!< RCC clock configuration register,								Address offset: 0x08 >*/
	__vo uint32_t CIR;					/*!< RCC clock interrupt register,									Address offset: 0x0C >*/
	__vo uint32_t AHB1RSTR;				/*!< RCC AHB1 peripheral reset register,							Address offset: 0x10 >*/
	__vo uint32_t AHB2RSTR;				/*!< RCC AHB2 peripheral reset register,							Address offset: 0x14 >*/
	__vo uint32_t AHB3RSTR;				/*!< RCC AHB3 peripheral reset register,							Address offset: 0x18 >*/
	uint32_t RESERVED0;					/*!< Reserved,														Address offset: 0x1C >*/
	__vo uint32_t APB1RSTR;				/*!< RCC APB1 peripheral reset register,							Address offset: 0x20 >*/
	__vo uint32_t APB2RSTR;				/*!< RCC APB2 peripheral reset register,							Address offset: 0x24 >*/
	uint32_t RESERVED1[2];				/*!< Reserved,														Address offset: 0x28 and 0x2C >*/
	__vo uint32_t AHB1ENR;				/*!< RCC AHB1 peripheral clock enable register,						Address offset: 0x30 >*/
	__vo uint32_t AHB2ENR;				/*!< RCC AHB2 peripheral clock enable register,						Address offset: 0x34 >*/
	__vo uint32_t AHB3ENR;				/*!< RCC AHB3 peripheral clock enable register,						Address offset: 0x38 >*/
	uint32_t RESERVED2;					/*!< Reserved,														Address offset: 0x3C >*/
	__vo uint32_t APB1ENR;				/*!< RCC APB1 peripheral clock enable register,						Address offset: 0x40 >*/
	__vo uint32_t APB2ENR;				/*!< RCC APB2 peripheral clock enable register,						Address offset: 0x44 >*/
	uint32_t RESERVED3[2];				/*!< Reserved,														Address offset: 0x48 and 0x5C >*/
	__vo uint32_t AHB1LPENR;			/*!< RCC AHB1 peripheral clock enable in low power mode register,	Address offset: 0x50 >*/
	__vo uint32_t AHB2LPENR;			/*!< RCC AHB2 peripheral clock enable in low power mode register,	Address offset: 0x54 >*/
	__vo uint32_t AHB3LPENR;			/*!< RCC AHB3 peripheral clock enable in low power mode register,	Address offset: 0x58 >*/
	uint32_t RESERVED4;					/*!< Reserved,														Address offset: 0x5C >*/
	__vo uint32_t APB1LPENR;			/*!< RCC APB1 peripheral clock enable in low power mode register,	Address offset: 0x60 >*/
	__vo uint32_t APB2LPENR;			/*!< RCC APB2 peripheral clock enabled in low power mode register,	Address offset: 0x64 >*/
	uint32_t RESERVED5[2];				/*!< Reserved,														Address offset: 0x68 and 0x6C >*/
	__vo uint32_t BDCR;					/*!< RCC Backup domain control register,							Address offset: 0x70 >*/
	__vo uint32_t CSR;					/*!< RCC clock control & status register,							Address offset: 0x74 >*/
	uint32_t RESERVED6[2];				/*!< Reserved,														Address offset: 0x78 and 0x8C >*/
	__vo uint32_t SSCGR;				/*!< RCC spread spectrum clock generation register,					Address offset: 0x80 >*/
	__vo uint32_t PLLI2SCFGR;			/*!< RCC PLLI2S configuration register,								Address offset: 0x84 >*/
}RCC_RegDef_t;

typedef struct
{
	__vo uint32_t IMR;					/*!< Interrupt mask register,										Address offset: 0x00 >*/
	__vo uint32_t EMR;					/*!< Event mask register,											Address offset: 0x04 >*/
	__vo uint32_t RTSR;					/*!< Rising trigger selection register,								Address offset: 0x08 >*/
	__vo uint32_t FTSR;					/*!< Falling trigger selection register,							Address offset: 0x0C >*/
	__vo uint32_t SWIER;				/*!< Software interrupt event register,								Address offset: 0x10 >*/
	__vo uint32_t PR;					/*!< Pending register,												Address offset: 0x14 >*/
}EXTI_RegDef_t;

typedef struct
{
	__vo uint32_t MEMRMP;				/*!< SYSCFG memory remap register,									Address offset: 0x00 >*/
	__vo uint32_t PMC;					/*!< SYSCFG peripheral mode configuration register,					Address offset: 0x004 >*/
	__vo uint32_t EXTICR[4];			/*!< SYSCFG external interrupt configuration register 1,2,3,4,		Address offset: 0x08 - 0x14 >*/
	uint32_t RESERVED1[2];				/*!< 																Reserved, 0x18-0x1C >*/
	__vo uint32_t CMPCR;				/*!< Compensation cell control register,							Address offset: 0x20 >*/
}SYSCFG_RegDef_t;

/*
 * 	Peripheral definitions.
 * 	Base addresses typecasted to xxx_RegDef_t
 */
#define GPIOA			(GPIO_RegDef_t*) GPIOA_BASEADDR
#define GPIOB			(GPIO_RegDef_t*) GPIOB_BASEADDR
#define GPIOC			(GPIO_RegDef_t*) GPIOC_BASEADDR
#define GPIOD			(GPIO_RegDef_t*) GPIOD_BASEADDR
#define GPIOE			(GPIO_RegDef_t*) GPIOE_BASEADDR
#define GPIOF			(GPIO_RegDef_t*) GPIOF_BASEADDR
#define GPIOG			(GPIO_RegDef_t*) GPIOG_BASEADDR
#define GPIOH			(GPIO_RegDef_t*) GPIOH_BASEADDR
#define GPIOI			(GPIO_RegDef_t*) GPIOI_BASEADDR

#define RCC				((RCC_RegDef_t*) RCC_BASEADDR)

#define EXTI			((EXTI_RegDef_t*) EXTI_BASEADDR)

/*
 * 	Clock Enable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_EN()		( RCC->AHB1ENR |= (1 << 0 ) )	// GPIOA Enable
#define GPIOB_PCLK_EN()		( RCC->AHB1ENR |= (1 << 1 ) )	// GPIOB Enable
#define GPIOC_PCLK_EN()		( RCC->AHB1ENR |= (1 << 2 ) )	// GPIOC Enable
#define GPIOD_PCLK_EN()		( RCC->AHB1ENR |= (1 << 3 ) )	// GPIOD Enable
#define GPIOE_PCLK_EN()		( RCC->AHB1ENR |= (1 << 4 ) )	// GPIOE Enable
#define GPIOF_PCLK_EN()		( RCC->AHB1ENR |= (1 << 5 ) )	// GPIOF Enable
#define GPIOG_PCLK_EN()		( RCC->AHB1ENR |= (1 << 6 ) )	// GPIOG Enable
#define GPIOH_PCLK_EN()		( RCC->AHB1ENR |= (1 << 7 ) )	// GPIOH Enable
#define GPIOI_PCLK_EN()		( RCC->AHB1ENR |= (1 << 8 ) )	// GPIOI Enable

/*
 * 	Clock Enable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_EN()		( RCC->APB1ENR |= (1 << 21 ) )	// I2C1 Enable
#define I2C2_PCLK_EN()		( RCC->APB1ENR |= (1 << 22 ) )	// I2C1 Enable
#define I2C3_PCLK_EN()		( RCC->APB1ENR |= (1 << 23 ) )	// I2C1 Enable

/*
 * 	Clock Enable Macros for SPIx peripherals
 */
#define SPI1_PCLK_EN()		( RCC->APB2ENR |= (1 << 12 ) )	// SPI1 Enable
#define SPI2_PCLK_EN()		( RCC->APB1ENR |= (1 << 14 ) )	// SPI1 Enable
#define SPI3_PCLK_EN()		( RCC->APB1ENR |= (1 << 15 ) )	// SPI1 Enable

/*
 *	Clock Enable Macros for USARTx peripherals
 */
#define USART1_PCLK_EN()		( RCC->APB2ENR |= (1 << 4 ) )	// USART1 Enable
#define UART2_PCLK_EN()			( RCC->APB1ENR |= (1 << 17 ) )	// UART2 Enable
#define UART3_PCLK_EN()			( RCC->APB1ENR |= (1 << 18 ) )	// UART3 Enable
#define UART4_PCLK_EN()			( RCC->APB1ENR |= (1 << 19 ) )	// UART4 Enable
#define UART5_PCLK_EN()			( RCC->APB1ENR |= (1 << 20 ) )	// UART5 Enable
#define USART6_PCLK_EN()		( RCC->APB2ENR |= (1 << 5 ) )	// USART6 Enable

/*
 * 	Clock Enable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_EN()		( RCC->APB2ENR |= (1 << 14) )

/*
 * 	Clock Disable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_DI()			( RCC->AHB1ENR &= ~(1 << 0 ) )
#define GPIOB_PCLK_DI()			( RCC->AHB1ENR &= ~(1 << 1 ) )	// GPIOB Disable
#define GPIOC_PCLK_DI()			( RCC->AHB1ENR &= ~(1 << 2 ) )	// GPIOC Disable
#define GPIOD_PCLK_DI()			( RCC->AHB1ENR &= ~(1 << 3 ) )	// GPIOD Disable
#define GPIOE_PCLK_DI()			( RCC->AHB1ENR &= ~(1 << 4 ) )	// GPIOE Disable
#define GPIOF_PCLK_DI()			( RCC->AHB1ENR &= ~(1 << 5 ) )	// GPIOF Disable
#define GPIOG_PCLK_DI()			( RCC->AHB1ENR &= ~(1 << 6 ) )	// GPIOG Disable
#define GPIOH_PCLK_DI()			( RCC->AHB1ENR &= ~(1 << 7 ) )	// GPIOH Disable
#define GPIOI_PCLK_DI()			( RCC->AHB1ENR &= ~(1 << 8 ) )	// GPIOI Disable

/*
 * 	Clock Disable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_DI()			( RCC->APB1ENR &= ~(1 << 21 ) )	// I2C1 Disable
#define I2C2_PCLK_DI()			( RCC->APB1ENR &= ~(1 << 22 ) )	// I2C1 Disable
#define I2C3_PCLK_DI()			( RCC->APB1ENR &= ~(1 << 23 ) )	// I2C1 Disable

/*
 * 	Clock Disable Macros for SPIx peripherals
 */
#define SPI1_PCLK_DI()			( RCC->APB2ENR &= ~(1 << 12 ) )	// SPI1 Disable
#define SPI2_PCLK_DI()			( RCC->APB1ENR &= ~(1 << 14 ) )	// SPI1 Disable
#define SPI3_PCLK_DI()			( RCC->APB1ENR &= ~(1 << 15 ) )	// SPI1 Disable

/*
 * 	Clock Disable Macros for USARTx peripherals
 */
#define USART1_PCLK_DI()		( RCC->APB2ENR &= ~(1 << 4 ) )	// USART1 Disable
#define UART2_PCLK_DI()			( RCC->APB1ENR &= ~(1 << 17 ) )	// UART2 Disable
#define UART3_PCLK_DI()			( RCC->APB1ENR &= ~(1 << 18 ) )	// UART3 Disable
#define UART4_PCLK_DI()			( RCC->APB1ENR &= ~(1 << 19 ) )	// UART4 Disable
#define UART5_PCLK_DI()			( RCC->APB1ENR &= ~(1 << 20 ) )	// UART5 Disable
#define USART6_PCLK_DI()		( RCC->APB2ENR &= ~(1 << 5 ) )	// USART6 Disable

/*
 * 	Clock Disable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_DI()		( RCC->APB2ENR &= ~(1 << 14) )

/*
 * 	Macros to reset GPIOx peripherals
 */
#define GPIOA_REG_RESET()		do{ ( RCC->AHB1RSTR |= (1 << 0 ) );	( RCC->AHB1RSTR &= ~(1 << 0 ) ); }while(0)
#define GPIOB_REG_RESET()		do{ ( RCC->AHB1RSTR |= (1 << 1 ) );	( RCC->AHB1RSTR &= ~(1 << 1 ) ); }while(0)
#define GPIOC_REG_RESET()		do{ ( RCC->AHB1RSTR |= (1 << 2 ) );	( RCC->AHB1RSTR &= ~(1 << 2 ) ); }while(0)
#define GPIOD_REG_RESET()		do{ ( RCC->AHB1RSTR |= (1 << 3 ) );	( RCC->AHB1RSTR &= ~(1 << 3 ) ); }while(0)
#define GPIOE_REG_RESET()		do{ ( RCC->AHB1RSTR |= (1 << 4 ) );	( RCC->AHB1RSTR &= ~(1 << 4 ) ); }while(0)
#define GPIOF_REG_RESET()		do{ ( RCC->AHB1RSTR |= (1 << 5 ) );	( RCC->AHB1RSTR &= ~(1 << 5 ) ); }while(0)
#define GPIOG_REG_RESET()		do{ ( RCC->AHB1RSTR |= (1 << 6 ) );	( RCC->AHB1RSTR &= ~(1 << 6 ) ); }while(0)
#define GPIOH_REG_RESET()		do{ ( RCC->AHB1RSTR |= (1 << 7 ) );	( RCC->AHB1RSTR &= ~(1 << 7 ) ); }while(0)
#define GPIOI_REG_RESET()		do{ ( RCC->AHB1RSTR |= (1 << 8 ) );	( RCC->AHB1RSTR &= ~(1 << 8 ) ); }while(0)

/***** Some generic macros *****/
#define ENABLE 					1
#define DISABLE 				0
#define SET 					ENABLE
#define RESET 					DISABLE
#define GPIO_PIN_SET			SET
#define GPIO_PIN_RESET			RESET

#include "stm32f407xx_gpio_driver.h"

#endif /* INC_STM32F407XX_H_ */
