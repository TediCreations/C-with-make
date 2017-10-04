/******************************************************************************
	Code
******************************************************************************/

/******************************************************************************
	Include files
******************************************************************************/

#include "gpio.h"

#include "stm32f0xx_hal.h"

#include <stdint.h>		/* uint16_t */

/******************************************************************************
	Local variables
******************************************************************************/

volatile uint16_t gs_exti_flag = 0;		/**< EXTI gpio interrupt flag */

/******************************************************************************
	Local function declarations
******************************************************************************/

/**
 * \brief Sets the nth bit.
 * 
 * \param[in]	bit		The bit to set.
 */
static inline void set_exti_flag( const uint16_t bit )
{
	/* Set bit */
	gs_exti_flag |= 1 << bit;
}

/**
 * \brief Clears the nth bit.
 * 
 * \param[in]	bit		The bit to set.
 */
static inline void clear_exti_flag( const uint16_t bit )
{
	/* Clear bit */
	gs_exti_flag &= ~(1 << bit);
}

/**
 * \brief Gets the flag (nth bit).
 * 
 * \param[in]	bit		The bit to set.
 * 
 * \return		The flag value [0, 1].
 */
static inline uint16_t get_exti_flag( const uint16_t pin_no )
{
	/* Read bit */
	const uint16_t flag = (gs_exti_flag >> pin_no) & 1;
	
	/* Exit with flag */
	return flag;
}

/**
 * \brief Enables the NVIC for GPIO EXTI interrupts.
 * 
 * \param[in]	bit		The bit to set.
 * 
 * \return		The flag value [0, 1].
 */
static void enable_nvic( const uint16_t GPIO_Pin )
{
	if ( ( GPIO_Pin == GPIO_PIN_0 ) || ( GPIO_Pin == GPIO_PIN_1 ) )
	{
		/* EXTI0_1_IRQn interrupt configuration */
		HAL_NVIC_SetPriority( EXTI0_1_IRQn, 3, 0 );
		HAL_NVIC_EnableIRQ( EXTI0_1_IRQn );
	}
	else if ( ( GPIO_Pin == GPIO_PIN_2 ) || ( GPIO_Pin == GPIO_PIN_3 ) )
	{
		/* EXTI2_3_IRQn interrupt configuration */
		HAL_NVIC_SetPriority( EXTI2_3_IRQn, 3, 0 );
		HAL_NVIC_EnableIRQ( EXTI2_3_IRQn );
	}
	else if ( ( GPIO_Pin == GPIO_PIN_4 ) || ( GPIO_Pin == GPIO_PIN_5 ) ||
			  ( GPIO_Pin == GPIO_PIN_6 ) || ( GPIO_Pin == GPIO_PIN_7 ) ||
			  ( GPIO_Pin == GPIO_PIN_8 ) || ( GPIO_Pin == GPIO_PIN_9 ) ||
			  ( GPIO_Pin == GPIO_PIN_10 ) || ( GPIO_Pin == GPIO_PIN_11 ) ||
			  ( GPIO_Pin == GPIO_PIN_12 ) || ( GPIO_Pin == GPIO_PIN_13 ) ||
			  ( GPIO_Pin == GPIO_PIN_14 ) || ( GPIO_Pin == GPIO_PIN_15 ) )
	{
		/* EXTI4_15_IRQn interrupt configuration */
		HAL_NVIC_SetPriority( EXTI4_15_IRQn, 3, 0 );
		HAL_NVIC_EnableIRQ( EXTI4_15_IRQn );
	}
}

/**
 * \brief EXTI line detection callbacks.
 * 
 * \param[in]	GPIO_Pin	The pin connected to EXTI line.
 */
void HAL_GPIO_EXTI_Callback( const uint16_t GPIO_Pin )
{
	if ( GPIO_Pin == GPIO_PIN_0 )
	{
		set_exti_flag( 0U );
	}
	if ( GPIO_Pin == GPIO_PIN_1 )
	{
		set_exti_flag( 1U );
	}
	if ( GPIO_Pin == GPIO_PIN_2 )
	{
		set_exti_flag( 2U );
	}
	if ( GPIO_Pin == GPIO_PIN_3 )
	{
		set_exti_flag( 3U );
	}
	if ( GPIO_Pin == GPIO_PIN_4 )
	{
		set_exti_flag( 4U );
	}
	if ( GPIO_Pin == GPIO_PIN_5 )
	{
		set_exti_flag( 5U );
	}
	if ( GPIO_Pin == GPIO_PIN_6 )
	{
		set_exti_flag( 6U );
	}
	if ( GPIO_Pin == GPIO_PIN_7 )
	{
		set_exti_flag( 7U );
	}
	if ( GPIO_Pin == GPIO_PIN_8 )
	{
		set_exti_flag( 8U );
	}
	if ( GPIO_Pin == GPIO_PIN_9 )
	{
		set_exti_flag( 9U );
	}
	if ( GPIO_Pin == GPIO_PIN_10 )
	{
		set_exti_flag( 10U );
	}
	if ( GPIO_Pin == GPIO_PIN_11 )
	{
		set_exti_flag( 11U );
	}
	if ( GPIO_Pin == GPIO_PIN_12 )
	{
		set_exti_flag( 12U );
	}
	if ( GPIO_Pin == GPIO_PIN_13 )
	{
		set_exti_flag( 13U );
	}
	if ( GPIO_Pin == GPIO_PIN_14 )
	{
		set_exti_flag( 14U );
	}
	if ( GPIO_Pin == GPIO_PIN_15 )
	{
		set_exti_flag( 15U );
	}
}

/******************************************************************************
	Function definitions
******************************************************************************/

/**
 * \brief Creates the gpio structure.
 * 
 * \param[in]	port	The gpio port.
 * \param[in]	pin		The gpio pin.
 * 
 * \return		The gpio structure.
 */
gpio_t gpio_build( GPIO_TypeDef* port, const uint16_t pin )
{
	/* Create the gpio structure */
	gpio_t aux;
	aux.port = port;
	aux.pin = pin;
	
	/* Return the gpio structure */
	return aux;
}

/**
 * \brief Configures the gpio.
 * 
 * \param[in]	me		The gpio handle.
 * \param[in]	mode	The gpio mode.
 * \param[in]	pullRes	The pull resistor position.
 */
void gpio_configure( gpio_t* me, const gpioMode_t mode, const gpioPullup_t pullRes )
{
	me->mode = mode;
	me->pullRes = pullRes;
}

/**
 * \brief Initializes the gpio.
 * 
 * \param[in]	me		The gpio handle.
 * 
 * \retval		0		Success.
 * \retval		1		Fail.
 */
uint32_t gpio_initialize( gpio_t* me )
{
	/* GPIO Ports Clock Enable */
	if( me->port == GPIOA )
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if( me->port == GPIOC )
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if( me->port == GPIOF )
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();
	}
	else
	{
		/* It is not supported yet */
		/* Exit with fail */
		return 1U;
	}
	
	/* Initialize pin */
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = me->pin;
	
	/* Select internal pullup resistor */
	if( me->pullRes == GP_PULLUP )
	{
		GPIO_InitStruct.Pull = GPIO_PULLUP;
	}
	else if( me->pullRes == GP_PULLDOWN )
	{
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	}
	else
	{
		GPIO_InitStruct.Pull = GPIO_NOPULL;
	}
	
	/* Select mode */
	if( me->mode == GM_INPUT_IT_RISING )
	{
		/* Enable NVIC */
		enable_nvic( me->pin );
		
		/* Configure GPIO pin */
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	}
	else if( me->mode == GM_INPUT_IT_FALLING )
	{
		/* Enable NVIC */
		enable_nvic( me->pin );
		
		/* Configure GPIO pin */
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	}
	else if( me->mode == GM_INPUT_IT_RISING_FALLING )
	{
		/* Enable NVIC */
		enable_nvic( me->pin );
		
		/* Configure GPIO pin */
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	}
	else if ( me->mode == GM_OUTPUT_PP )
	{
		/* Configure output Level */
		HAL_GPIO_WritePin( me->port, me->pin, GPIO_PIN_RESET );
		
		/* Configure GPIO pin */
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	}
	else if ( me->mode == GM_OUTPUT_OD )
	{
		/*Configure output Level */
		HAL_GPIO_WritePin( me->port, me->pin, GPIO_PIN_RESET );
		
		/* Configure GPIO pin */
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	}
	else
	{
		/* It is not supported yet */
		/* Exit with fail */
		return 1U;
	}
	
	/* Initialize gpio pin */
	HAL_GPIO_Init( me->port, &GPIO_InitStruct );
	
	/* Exit with success */
	return 0U;
}

/**
 * \brief Deinitializes the gpio.
 * 
 * \param[in]	me		The gpio handle.
 */
void gpio_deinitialize( gpio_t* me )
{
	/* Deinitialize pins */
	HAL_GPIO_DeInit( me->port, me->pin );
}

/**
 * \brief Sets the gpio high.
 * 
 * \param[in]	me		The gpio handle.
 */
void gpio_write_high( gpio_t* me )
{
	HAL_GPIO_WritePin( me->port, me->pin, GPIO_PIN_SET );
}

/**
 * \brief Sets the gpio low.
 * 
 * \param[in]	me		The gpio handle.
 */
void gpio_write_low( gpio_t* me )
{
	HAL_GPIO_WritePin( me->port, me->pin, GPIO_PIN_RESET );
}

/**
 * \brief Toggles the state of the gpio.
 * 
 * \param[in]	me		The gpio handle.
 */
void gpio_toggle( gpio_t* me )
{
	HAL_GPIO_TogglePin( me->port, me->pin );
}

/**
 * \brief Reads the current gpio state.
 * 
 * \param[in]	me		The gpio handle.
 * 
 * \retval		0		Gpio is low.
 * \retval		1		Gpio is high.
 */
uint32_t gpio_read( gpio_t* me )
{
	if ( HAL_GPIO_ReadPin( me->port, me->pin ) == GPIO_PIN_SET )
	{
		/* Exit with pin high */
		return 1U;
	}
	else
	{
		/* Exit with pin low */
		return 0U;
	}
}

/**
 * \brief Reads the gpio interrupt flag.
 * 
 * \param[in]	me		The gpio handle.
 * 
 * \retval		0		Gpio flag is not set or error.
 * \retval		1		Gpio flag is set.
 */
uint32_t gpio_read_IT( gpio_t* me )
{
	/* Get pin */
	uint16_t pin_no = 0;
	if( me->pin == GPIO_PIN_0 )				{ pin_no = 0; }
	else if( me->pin == GPIO_PIN_1 )		{ pin_no = 1; }
	else if( me->pin == GPIO_PIN_2 )		{ pin_no = 2; }
	else if( me->pin == GPIO_PIN_3 )		{ pin_no = 3; }
	else if( me->pin == GPIO_PIN_4 )		{ pin_no = 4; }
	else if( me->pin == GPIO_PIN_5 )		{ pin_no = 5; }
	else if( me->pin == GPIO_PIN_6 )		{ pin_no = 6; }
	else if( me->pin == GPIO_PIN_7 )		{ pin_no = 7; }
	else if( me->pin == GPIO_PIN_8 )		{ pin_no = 8; }
	else if( me->pin == GPIO_PIN_9 )		{ pin_no = 9; }
	else if( me->pin == GPIO_PIN_10 )		{ pin_no = 10; }
	else if( me->pin == GPIO_PIN_11 )		{ pin_no = 11; }
	else if( me->pin == GPIO_PIN_12 )		{ pin_no = 12; }
	else if( me->pin == GPIO_PIN_13 )		{ pin_no = 13; }
	else if( me->pin == GPIO_PIN_14 )		{ pin_no = 14; }
	else if( me->pin == GPIO_PIN_15 )		{ pin_no = 15; }
	else
	{
		/* Exit with error */
		return 0;
	}
	
	/* Get interrupt flag */
	const uint16_t flag = get_exti_flag( pin_no );
	
	if( flag )
	{
		clear_exti_flag( pin_no );
	}
	
	/* Exit with interruot flag */
	return flag;
}

/******************************************************************************
	Interrupt Handler definitions
******************************************************************************/

/**
 * \brief The EXTI0_1 interrupt handler.
 */
void EXTI0_1_IRQHandler( void )
{
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_1 );
}

/**
 * \brief The EXTI2_3 interrupt handler.
 */
void EXTI2_3_IRQHandler( void )
{
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_2 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_3 );
}

/**
 * \brief The EXTI4_15 interrupt handler.
 */
void EXTI4_15_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_4 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_5 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_6 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_7 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_8 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_9 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_11 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_12 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_13 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_14 );
	HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_15 );
}
