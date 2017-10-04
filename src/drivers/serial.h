/******************************************************************************
	About
******************************************************************************/

/**
 * \file serial.h
 *
 * \brief	UART adapter for the ST HAL driver.
 *
 * Created:			17/05/2017
 *
 * \author	Elias Kanelis	hkanelhs@yahoo.gr
 */

/**
* \defgroup	Drivers	Drivers
*
* \code	#include <serial.h>	\endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef SERIAL_H_ONLY_ONE_INCLUDE_SAFETY
#define SERIAL_H_ONLY_ONE_INCLUDE_SAFETY

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
	Include files
******************************************************************************/

#include "stm32f0xx_hal.h"

#include "gpio.h"

#include <stdint.h>		/* uint32_t */

/******************************************************************************
	Type definitions
******************************************************************************/

/**
 * UART port.
 */
typedef enum
{
	SP_UART1 = 1,		/**< The UART1 port */
	SP_UART2 = 2,		/**< The UART2 port */
	SP_UART3 = 3,		/**< The UART3 port */
	SP_UART4 = 4		/**< The UART4 port */
} serialPort_t;

/**
 * UART handler.
 */
typedef struct
{
	serialPort_t port;			/**< The UART port */

	gpio_t rx_pin;				/**< The UART Rx gpio. */
	gpio_t tx_pin;				/**< The UART Tx gpio. */
	
	uint8_t rx_pin_altfun;		/**< The UART Rx gpio alternate function. */
	uint8_t tx_pin_altfun;		/**< The UART Tx gpio alternate function. */
	
} serial_t;

/******************************************************************************
	Function declarations
******************************************************************************/

serial_t	serial_build( const serialPort_t port,
						  const gpio_t rx_pin, const uint8_t rx_pin_altfun,
						  const gpio_t tx_pin, const uint8_t tx_pin_altfun );

void		serial_configure( serial_t* me, const uint32_t baudrate );
						  
void		serial_enableInterrupt( serial_t* me );
void		serial_disableInterrupt( serial_t* me );

uint32_t	serial_initialize( serial_t* me );
uint32_t	serial_deinitialize( serial_t* me );

uint32_t	serial_puts( serial_t* me, const char* s );
uint32_t	serial_puts_IT( serial_t* me, const char* s );

uint32_t	serial_getc( serial_t* me, const char* c );
uint32_t	serial_getc_IT( serial_t* me, const char* c );
						  
uint32_t	serial_putc( serial_t* me, const char c );

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_H_ONLY_ONE_INCLUDE_SAFETY */
