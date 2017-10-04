/******************************************************************************
	About
******************************************************************************/

/**
 * \file gpio.h
 *
 * \brief	GPIO HAL adapter for the STM32F072RB microcontroller
 *
 * Created:			21/05/2017
 *
 * \author	Elias Kanelis	hkanelhs@yahoo.gr
 */

/**
* \defgroup	Drivers	Drivers
*
* \code	#include <gpio.h>	\endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef GPIO_H_ONLY_ONE_INCLUDE_SAFETY
#define GPIO_H_ONLY_ONE_INCLUDE_SAFETY

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
	Include files
******************************************************************************/

#include "stm32f0xx_hal.h"	/* GPIO_TypeDef */

#include <stdint.h>			/* uint16_t */

/******************************************************************************
	Type definitions
******************************************************************************/

/**
 * GPIO mode
 */
typedef enum
{
	/* Input */
	//GM_INPUT_FLOATING,			/**<  Default mode: Input floating. */
	GM_INPUT_IT_RISING,				/**<  Input with rising interrupt. */
	GM_INPUT_IT_FALLING,			/**<  Input with falling interrupt. */
	GM_INPUT_IT_RISING_FALLING,		/**<  Input with rising and falling interrupt. */
	
	/* Output */
	GM_OUTPUT_PP,					/**< Push pull. */
	GM_OUTPUT_OD,					/**< Open drain. */
	
	/* Alternative function */
	GM_MODE_AF_PP					/**< Alternative function mode. */
} gpioMode_t;

/**
 * GPIO pull resistor
 */
typedef enum
{
	GP_NONE,			/**< No Pull-up or Pull-down activation. */
	GP_PULLUP,			/**< Pull-up activation. */
	GP_PULLDOWN			/**< Pull-down activation. */
} gpioPullup_t;

/**
 * GPIO handle
 */
typedef struct
{
	GPIO_TypeDef* port;		/**< The GPIO port. */
	uint16_t pin;			/**< The GPIO pin. */
	gpioMode_t mode;		/**< The GPIO mode. */
	gpioPullup_t pullRes;	/**< The GPIO pull resistor. */
} gpio_t;

/******************************************************************************
	Function declarations
******************************************************************************/

gpio_t		gpio_build( GPIO_TypeDef* port, const uint16_t pin );

void		gpio_configure( gpio_t* me, const gpioMode_t mode, const gpioPullup_t pullRes );

uint32_t	gpio_initialize( gpio_t* me );
void		gpio_deinitialize( gpio_t* me );

void		gpio_write_high( gpio_t* me );
void		gpio_write_low( gpio_t* me );
void		gpio_toggle( gpio_t* me );

uint32_t	gpio_read( gpio_t* me );
uint32_t	gpio_read_IT( gpio_t* me );//This clear flags

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H_ONLY_ONE_INCLUDE_SAFETY */
