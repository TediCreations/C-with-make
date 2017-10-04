/******************************************************************************
	Code
******************************************************************************/

/******************************************************************************
	Include files
******************************************************************************/

#include "cpu.h"

#include "stm32f0xx_hal.h"

/******************************************************************************
	Local function declarations
******************************************************************************/

#ifdef USE_FULL_ASSERT

void assert_failed( uint8_t* file, uint32_t line )
{
	
}

#endif

/******************************************************************************
	Function definitions
******************************************************************************/

/**
 * \brief Initialize processor clocks.
 * 
 * \retval	0	Success.
 * \retval	1	Fail.
 */
uint32_t cpu_config_clock( void )
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */	
	HAL_Init();
	
	/* Enable the High Speed APB (APB2) peripheral clock */
	__HAL_RCC_SYSCFG_CLK_ENABLE();

	/* System interrupt init*/
	/* SVC_IRQn interrupt configuration */
	HAL_NVIC_SetPriority( SVC_IRQn, 3, 0 );
	/* PendSV_IRQn interrupt configuration */
	HAL_NVIC_SetPriority( PendSV_IRQn, 3, 0 );
	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority( SysTick_IRQn, 3, 0 );
	
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	
    /* Configure LSE Drive Capability */
	__HAL_RCC_LSEDRIVE_CONFIG( RCC_LSEDRIVE_LOW );
	
	/* Initializes the CPU, AHB and APB busses clocks */
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_OscInitStruct.OscillatorType = 	RCC_OSCILLATORTYPE_HSI|
										RCC_OSCILLATORTYPE_HSI48|
										RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
	if ( HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK )
	{
		/* Exit with fail */
		return 1;
	}

    /* Initializes the CPU, AHB and APB busses clocks */
	RCC_ClkInitStruct.ClockType =	RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|
									RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		/* Exit with fail */
		return 1;
	}
	
	/* Configure the Systick interrupt time */
	HAL_SYSTICK_Config( HAL_RCC_GetHCLKFreq()/1000 );
	
	/* Configure the Systick */
	HAL_SYSTICK_CLKSourceConfig( SYSTICK_CLKSOURCE_HCLK );
	
	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority( SysTick_IRQn, 3, 0 );
	
	/* Exit with success */
	return 0;
}

/**
 * \brief Software resets the processor.
 */
void cpu_reset( void )
{
	NVIC_SystemReset();
}

/**
 * \brief Delay routine in msec.
 * 
 * \param[in]	ms	Time to delay in milliseconds.
 * 
 * \retval		0	Success.
 * \retval		1	Fail.
 */
void cpu_delay_ms( uint32_t ms )
{
	HAL_Delay( ms );
}

/******************************************************************************
	Interrupt Handler definitions
******************************************************************************/

/**
* \brief System tick timer interruption and Exception Handler.
*/
void SysTick_Handler(void)
{
	HAL_IncTick();
}
