/*
 * 002led_button.c
 *
 *  Created on: Aug 14, 2026
 *      Author: Robert
 */
#include <string.h>
#include "stm32f407xx.h"

#define HIGH			1
#define LOW				0
#define BTN_PRESSED		HIGH

void delay(void)
{
	for(uint32_t i=0; i<500000; i++);
}

int main(void)
{
	GPIO_Handle_t GpioLed, GPIObtn;
	memset(&GpioLed,0,sizeof(GpioLed));
	memset(&GPIObtn,0,sizeof(GPIObtn));

	// Led Configuration
	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&GpioLed);

	// Button Configuration
	GPIObtn.pGPIOx = GPIOD;
	GPIObtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIObtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GPIObtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIObtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

	GPIO_PeriClockControl(GPIOD, ENABLE);
	GPIO_Init(&GPIObtn);

	GPIO_WriteToOutputPin(GPIOD, GPIO_PIN_NO_12, GPIO_PIN_RESET);
	// IRQ configurations
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, NVIC_IRQ_PRI15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI9_5, ENABLE);

	while(1);
	return 0;
}

void EXTI9_5_IRQHandler(void)
{
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_5);
	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);
}
