/******************************************************************************
	Code
******************************************************************************/

/******************************************************************************
	Include files
******************************************************************************/

#include "serial.h"

#include "gpio.h"

#include <string.h>		/* strlen */
#include <stdint.h>		/* uint32_t */

#include "stm32f0xx_hal.h"

/******************************************************************************
	Local variables
******************************************************************************/

/* External variables */
/* 
 * These local vaairables are shared with interrupt
 * TODO: Study about why they are not volatile
 */
static UART_HandleTypeDef huart1;	/**< UART1 port handle in STM32 HAL lib */
static UART_HandleTypeDef huart2;	/**< UART2 port handle in STM32 HAL lib */
static UART_HandleTypeDef huart3;	/**< UART3 port handle in STM32 HAL lib */
static UART_HandleTypeDef huart4;	/**< UART4 port handle in STM32 HAL lib */

/*
 * UART3 and UART4 share the same irq so we monitor the interrups usage
 * in order to know when to fully disable interrupts
 */
static uint8_t isUART3_irq = 0;		/**< UART3 interrupt enable flag.
										 0 is not enabled */
static uint8_t isUART4_irq = 0;		/**< UART3 interrupt enable flag.
										 0 is not enabled */

/******************************************************************************
	Local function declarations
******************************************************************************/

static UART_HandleTypeDef* getUART_Handle( const serialPort_t port )
{
	/* Find port */
	if ( port == SP_UART1 )
	{
		return &huart1;
	}
	else if ( port == SP_UART2 )
	{
		return &huart2;
	}
	else if ( port == SP_UART3 )
	{
		return &huart3;
	}
	else if ( port == SP_UART4 )
	{
		return &huart4;
	}
	else
	{
		/* 
		 * This is a silent error
		 * TODO: It should inform the system somehow.
		 */
		return NULL;
	}
}

/**
 * \brief Initializes a pin with its alternate function as UART.
 * 
 * \param[in]	me			The gpio handle.
 * \param[in]	pin_altfun	The pin alternative function.
 * 
 * \retval		0			Success.
 * \retval		1			Fail.
 */
static uint32_t initialize_pin( gpio_t* me, const uint8_t pin_altfun )
{
	/* GPIO Ports Clock Enable */
	if( me->port == GPIOA )
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if( me->port == GPIOB )
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
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
		/* No such port available */
		/* Exit with fail */
		return 1U;
	}
	
	/* Settings for UART */
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = me->pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = pin_altfun;
	
	/* Initialize gpio pin */
	HAL_GPIO_Init( me->port, &GPIO_InitStruct );
	
	/* Exit with success */
	return 0U;
}

/**
 * \brief Enable the USART3_4_IRQn if both UART3 and UART4 are not using it.
 * 
 * \param[in]	isUART3_irq		Interrupt use of UART3.
 * \param[in]	isUART4_irq		Interrupt use of UART4.
 */
static void enable_USART3_4_irq( const uint8_t isUART3_irq, const uint8_t isUART4_irq )
{
	if ( (!isUART3_irq) || ( !isUART4_irq ) )
	{
		/* 
		 * Since UART3 and UART4 are not using interrupts
		 * we should enable USART3_4_IRQn
		 */
		
		/* USART3_4_IRQn interrupt configuration */
		HAL_NVIC_SetPriority( USART3_4_IRQn, 0, 0 );
		HAL_NVIC_EnableIRQ( USART3_4_IRQn );
	}
}

/**
 * \brief Disable the USART3_4_IRQn if both UART3 and UART4 are not using it.
 * 
 * \param[in]	isUART3_irq		Interrupt use of UART3.
 * \param[in]	isUART4_irq		Interrupt use of UART4.
 */
static void disable_USART3_4_irq( const uint8_t isUART3_irq, const uint8_t isUART4_irq )
{
	if ( (!isUART3_irq) || ( !isUART4_irq ) )
	{
		/* 
		 * Since UART3 and UART4 are not using interrupts
		 * we should disable USART3_4_IRQn
		 */
		
		/* USART3_4_IRQn interrupt configuration */
		HAL_NVIC_SetPriority( USART3_4_IRQn, 0, 0 );
		HAL_NVIC_DisableIRQ( USART3_4_IRQn );
	}
}

/**
 * \brief Initializes UART.
 * 
 * \param[in]	me		The gpio handle.
 * \param[in]	port	The UART port to initialize.
 * 
 * \retval		0		Success.
 * \retval		1		Fail.
 */
static uint32_t serial_init( serial_t* me, const serialPort_t port )
{
	/* Get UART handler */
	UART_HandleTypeDef* huart = getUART_Handle( me->port );
	
	/* Init port */
	if( port == SP_UART1 )
	{
		huart->Instance = USART1;
	}
	else if( port == SP_UART2 )
	{
		huart->Instance = USART2;
	}
	else if( port == SP_UART3 )
	{
		huart->Instance = USART3;
	}
	else if( port == SP_UART4 )
	{
		huart->Instance = USART4;
	}
	else
	{
		/* Not available port */
		
		/* Exit with error */
		return 1U;
	}
	
	/* Default initialization */
	huart->Init.WordLength = UART_WORDLENGTH_8B;
	huart->Init.StopBits = UART_STOPBITS_1;
	huart->Init.Parity = UART_PARITY_NONE;
	huart->Init.Mode = UART_MODE_TX_RX;
	huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
	huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	/* Exit with success */
	return 0U;
}

/******************************************************************************
	Function definitions
******************************************************************************/

/**
 * \brief Builds and provides the UART handle.
 * 
 * \param[in]	port			The UART port.
 * \param[in]	rx_pin			The UART Rx gpio.
 * \param[in]	rx_pin_altfun	The UART Rx gpio alternate function.
 * \param[in]	tx_pin			The UART Tx gpio.
 * \param[in]	tx_pin_altfun	The UART Tx gpio alternate function.
 * 
 * \return		The UART handle.
 */
serial_t serial_build( const serialPort_t port,
					   const gpio_t rx_pin, const uint8_t rx_pin_altfun,
					   const gpio_t tx_pin, const uint8_t tx_pin_altfun )
{
	serial_t aux;
	aux.port = port;
	aux.rx_pin = rx_pin;
	aux.tx_pin = tx_pin;
	
	aux.rx_pin_altfun = rx_pin_altfun;
	aux.tx_pin_altfun = tx_pin_altfun;
	
	serial_init( &aux, port );
	
	return aux;
}

/**
 * \brief Configures the UART handle.
 * 
 * \param[in]	me			The UART handle.
 * \param[in]	baudrate	The communication baudrate.
 */
void serial_configure( serial_t* me, const uint32_t baudrate )
{
	/* Set baudrate */
	UART_HandleTypeDef* huart = getUART_Handle( me->port );
	huart->Init.BaudRate = baudrate;
}

/**
 * \brief Enables the UART port interrupts.
 * 
 * \param[in]	me			The UART handle.
 */
void serial_enableInterrupt( serial_t* me )
{
	/* Find port */
	if( me->port == SP_UART1 )
	{
		/* USART1_IRQn interrupt configuration */
		HAL_NVIC_SetPriority( USART1_IRQn, 0, 0 );
		HAL_NVIC_EnableIRQ( USART1_IRQn);
	}
	else if( me->port == SP_UART2 )
	{
		/* USART2_IRQn interrupt configuration */
		HAL_NVIC_SetPriority( USART2_IRQn, 0, 0 );
		HAL_NVIC_EnableIRQ( USART2_IRQn );
	}
	else if( me->port == SP_UART3 )
	{
		/* UART3 is enabled */
		isUART3_irq = 1U;
		
		/* USART3_4_IRQn interrupt configuration */
		enable_USART3_4_irq( isUART3_irq, isUART4_irq );
	}
	else if( me->port == SP_UART4 )
	{
		/* UART4 is enabled */
		isUART4_irq = 1U;
		
		/* USART3_4_IRQn interrupt configuration */
		enable_USART3_4_irq( isUART3_irq, isUART4_irq );
	}
}

/**
 * \brief Disables the UART port interrupts.
 * 
 * \param[in]	me			The UART handle.
 */
void serial_disableInterrupt( serial_t* me )
{
	/* Find port */
	if( me->port == SP_UART1 )
	{
		/* USART1_IRQn interrupt configuration */
		HAL_NVIC_DisableIRQ( USART1_IRQn);
	}
	else if( me->port == SP_UART2 )
	{
		/* USART2_IRQn interrupt configuration */
		HAL_NVIC_DisableIRQ( USART2_IRQn );
	}
	else if( me->port == SP_UART3 )
	{
		/* UART3 is disabled */
		isUART3_irq = 0U;
		
		/* USART3_4_IRQn interrupt configuration */
		disable_USART3_4_irq( isUART3_irq, isUART4_irq );
	}
	else if( me->port == SP_UART4 )
	{
		/* UART4 is disabled */
		isUART4_irq = 0U;
		
		/* USART3_4_IRQn interrupt configuration */
		disable_USART3_4_irq( isUART3_irq, isUART4_irq );
	}
}
/**
 * \brief Initializes the UART handle.
 * 
 * \param[in]	me			The UART handle.
 * 
 * \retval		0			Success.
 * \retval		1			Fail.
 */
uint32_t serial_initialize( serial_t* me )
{
	/* Peripheral clock enable */
	if( me->port == SP_UART1 )
	{
		__HAL_RCC_USART1_CLK_ENABLE();
	}
	else if( me->port == SP_UART2 )
	{
		__HAL_RCC_USART2_CLK_ENABLE();
	}
	else if( me->port == SP_UART3 )
	{
		__HAL_RCC_USART3_CLK_ENABLE();
	}
	else if( me->port == SP_UART4 )
	{
		__HAL_RCC_USART4_CLK_ENABLE();
	}
	else
	{
		/* There is not such UART port */
		
		/* Exit with error */
		return 1U;
	}
		
	/* Initialize */
	if ( HAL_UART_Init( getUART_Handle( me->port ) ) != HAL_OK )
	{
		/* Exit with error */
		return 1U;
	}
	
	/* Initialize GPIO pins */
	if( initialize_pin( &(me->rx_pin), me->rx_pin_altfun ) )
	{
		/* Exit with error */
		return 1U;
	}
	if( initialize_pin( &(me->tx_pin), me->tx_pin_altfun ) )
	{
		/* Exit with error */
		return 1U;
	}
	
	/* Exit with success */
	return 0U;
}

/**
 * \brief Deinitializes the UART handle.
 * 
 * \param[in]	me			The UART handle.
 * 
 * \retval		0			Success.
 * \retval		1			Fail.
 */
uint32_t serial_deinitialize( serial_t* me )
{
	/* Peripheral clock disable */
	if( me->port == SP_UART1 )
	{
		__HAL_RCC_USART1_CLK_DISABLE();
	}
	else if( me->port == SP_UART2 )
	{
		__HAL_RCC_USART2_CLK_DISABLE();
	}
	else if( me->port == SP_UART3 )
	{
		__HAL_RCC_USART3_CLK_DISABLE();
	}
	else if( me->port == SP_UART4 )
	{
		__HAL_RCC_USART4_CLK_DISABLE();
	}
	else
	{
		/* There is not such UART port */
		
		/* Exit with error */
		return 1U;
	}

	/* Deinitialize pins */
	gpio_deinitialize( &(me->rx_pin) );
	gpio_deinitialize( &(me->tx_pin) );
	
	/* Exit with success */
	return 0U;
}

/**
 * \brief Sends a char array over the UART.
 * 
 * \param[in]	me			The UART handle.
 * \param[in]	s			The char array to sent.
 * 
 * \retval		0			Success.
 * \retval		1			Fail.
 */
uint32_t serial_puts( serial_t* me, const char* s )
{
	/* Transmit */
	const uint16_t len = strlen( s );
	if( HAL_UART_Transmit( getUART_Handle( me->port ), (uint8_t*)s, len, 100 ) != HAL_OK )
	{
		/* Exit with fail */
		return 1U;
	}
	
	/* Exit with success */
	return 0U;
}

/**
 * \brief Sends a char array over the UART using interrupt.
 * 
 * \param[in]	me			The UART handle.
 * \param[in]	s			The char array to sent.
 * 
 * \retval		0			Success.
 * \retval		1			Fail.
 */
uint32_t serial_puts_IT( serial_t* me, const char* s )
{
	/* Transmit */
	const uint16_t len = strlen( s );
	if( HAL_UART_Transmit_IT( getUART_Handle( me->port ), (uint8_t*)s, len ) != HAL_OK )
	{
		/* Exit with fail */
		return 1U;
	}
	
	/* Exit with success */
	return 0U;
}

/**
 * \brief Receives a char over the UART.
 * 
 * \param[in]	me			The UART handle.
 * \param[out]	c			The char we receive.
 * 
 * \retval		0			Success.
 * \retval		1			Fail.
 */
uint32_t serial_getc( serial_t* me, const char* c )
{
	/* Receive */
	if( HAL_UART_Receive( getUART_Handle( me->port ), (uint8_t*)(c), 1, 100 ) != HAL_OK )
	{
		/* Exit with fail */
		return 1U;
	}
	
	/* Exit with success */
	return 0U;
}

/**
 * \brief Receives a char over the UART using interrupt.
 * 
 * \param[in]	me			The UART handle.
 * \param[out]	c			The char we receive.
 * 
 * \retval		0			Success.
 * \retval		1			Fail.
 */
uint32_t serial_getc_IT( serial_t* me, const char* c )
{
	/* Receive */
	if( HAL_UART_Receive_IT( getUART_Handle( me->port ), (uint8_t*)(c), 1 ) != HAL_OK )
	{
		/* Exit with fail */
		return 1U;
	}
	
	/* Exit with success */
	return 0U;
}

/**
 * \brief Sends a char over the UART.
 * 
 * \param[in]	me			The UART handle.
 * \param[in]	c			The char to sent.
 * 
 * \retval		0			Success.
 * \retval		1			Fail.
 */
uint32_t serial_putc( serial_t* me, const char c )
{
	/* Transmit */
	if( HAL_UART_Transmit( getUART_Handle( me->port ), (uint8_t*)&c, 1, 100 ) != HAL_OK )
	{
		/* Exit with fail */
		return 1U;
	}
	
	/* Exit with success */
	return 0U;
}

/******************************************************************************
	Interrupt Handler definitions
******************************************************************************/

/**
 * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
 */
void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler( &huart1 );
}

/**
 * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
 */
void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler( &huart2 );
}

/**
 * @brief This function handles USART3 and USART4 global interrupts.
 */
void USART3_4_IRQHandler(void)
{
	/* UART3 */
	if( isUART3_irq )
	{
		HAL_UART_IRQHandler( &huart3 );
	}
	/* UART4 */
	if( isUART4_irq )
	{
		HAL_UART_IRQHandler( &huart4 );
	}
}
