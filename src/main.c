#include "led/led.h"
#include "stm32f0xx.h"


//TODO: This is from minimal.

int main( void )
{
	SystemCoreClockUpdate();

	led_init();
	led_off();


	//TODO: Remove this
	__NOP();

	while( 1 )
	{
		for( unsigned long int i=0; i<=1000000; i++ )
		{
			__NOP();
		}
		led_toggle();

	}

	return 0;
}
