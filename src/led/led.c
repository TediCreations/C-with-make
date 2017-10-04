#include "stm32f0xx.h"

void led_init( void )
{
	/* Enable GPIOA clock */
	RCC->AHBENR |= ( 1U << 17 );

	/* Make GPIOA pin5 an output */
	GPIOA->MODER |= ( 0x01U << 5*2 );
}

void led_on( void )
{
	/* Set GPIOA pin5 */
	GPIOA->ODR |= ( 1U << 5 );
}

void led_off( void )
{
	/* Reset GPIOA pin5 */
	GPIOA->ODR &= ~( 1U << 5 );
}

void led_toggle( void )
{
	/* Toggle GPIOA pin5 */
	GPIOA->ODR ^= ( 1U << 5 );
}
