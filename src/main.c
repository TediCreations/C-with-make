/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
//#include "stm32f0xx_hal.h"

#include "drivers/gpio.h"
#include "drivers/serial.h"
#include "drivers/cpu.h"

int main(void)
{
	/* Initialize processor clock */
	cpu_config_clock();

	/* Initialize console */
	gpio_t console_rx = gpio_build( GPIOA, GPIO_PIN_3 );
	gpio_t console_tx = gpio_build( GPIOA, GPIO_PIN_2 );
	serial_t console = serial_build( SP_UART2, console_rx, GPIO_AF1_USART2,
											   console_tx, GPIO_AF1_USART2 );
	serial_configure( &console, 115200U );
	serial_enableInterrupt( &console );
	serial_initialize( &console );

	/* Initialize gsm */
	gpio_t gsm_rx = gpio_build( GPIOA, GPIO_PIN_10 );
	gpio_t gsm_tx = gpio_build( GPIOA, GPIO_PIN_9 );
	serial_t gsm = serial_build( SP_UART1, gsm_rx, GPIO_AF1_USART1,
										   gsm_tx, GPIO_AF1_USART1 );
	serial_configure( &gsm, 115200U );
	serial_enableInterrupt( &gsm );
	serial_initialize( &gsm );

	/* Initialize gps */
	gpio_t gps_rx = gpio_build( GPIOB, GPIO_PIN_11 );
	gpio_t gps_tx = gpio_build( GPIOB, GPIO_PIN_10 );
	serial_t gps = serial_build( SP_UART3, gps_rx, GPIO_AF4_USART3,
										   gps_tx, GPIO_AF4_USART3 );
	serial_configure( &gps, 115200U );
	serial_enableInterrupt( &gps );
	serial_initialize( &gps );

	/* Initialize UART4 */
	gpio_t uart4_rx = gpio_build( GPIOC, GPIO_PIN_11 );
	gpio_t uart4_tx = gpio_build( GPIOC, GPIO_PIN_10 );
	serial_t uart4 = serial_build( SP_UART4, uart4_rx, GPIO_AF0_USART4,
											 uart4_tx, GPIO_AF0_USART4 );
	serial_configure( &uart4, 115200U );
	serial_enableInterrupt( &uart4 );
	serial_initialize( &uart4 );

	/* Initialize led */
	gpio_t led = gpio_build( GPIOA, GPIO_PIN_5 );
	gpio_configure( &led, GM_OUTPUT_PP, GP_PULLUP );
	gpio_initialize( &led );
	gpio_write_high( &led );

	/* Initialize button */
	gpio_t button = gpio_build( GPIOC, GPIO_PIN_13 );
	gpio_configure( &button, GM_INPUT_IT_RISING, GP_NONE );
	gpio_initialize( &button );

	/* Greeting */
	serial_puts( &console, "Console...\n" );
	serial_puts( &gsm, "GSM.......\n" );
	serial_puts( &gps, "GPS.......\n" );
	serial_puts( &uart4, "UART4.....\n" );

	serial_puts_IT( &console, "Console...\n" );
	serial_puts_IT( &gsm, "GSM.......\n" );
	serial_puts_IT( &gps, "GPS.......\n" );
	serial_puts_IT( &uart4, "UART4.....\n" );

	//cpu_delay_ms( 1 );
	serial_puts_IT( &console, "Well this is a console...\n" );
	serial_puts_IT( &gsm, "Well this is a GSM.......\n" );
	serial_puts_IT( &gps, "Well this is a GPS.......\n" );
	serial_puts_IT( &uart4, "Well this is a UART4.....\n" );

	//while (1)
	//{
	//	serial_puts( &console, "Button IRQ is pushed........\n" );
	//	//serial_puts( &console, "Resetting...\n" );
	//	cpu_delay_ms( 200 );
	//}

	while (1)
	{
		char c = 0;

		do
		{
			if( serial_getc( &console, &c ) == 0 )
			{
					serial_putc( &console, c );
			}

		}
		while( c == '\n' );

		if( c == '1' )
		{
			serial_puts( &console, "Got one\n" );
		}
		if( c == '2' )
		{
			serial_puts( &console, "Got two\n" );
		}
		if( c == '3' )
		{
			serial_puts( &console, "Got three\n" );
		}


		gpio_toggle( &led );
		//cpu_delay_ms( 250 );

		if( gpio_read_IT( &button ) )
		{
			serial_puts( &console, "Button IRQ is pushed........\n" );
			//serial_puts( &console, "Resetting...\n" );
			cpu_delay_ms( 10 );
			//cpu_reset();

		}
	}
}
