/******************************************************************************
	About
******************************************************************************/

/**
 * \file cpu.h
 *
 * \brief	STM32F072RB utilities.
 *
 * Created:			24/05/2017
 *
 * \author	Elias Kanelis	hkanelhs@yahoo.gr
 */

/**
* \defgroup	Drivers	Drivers
*
* \code	#include <cpu.h>	\endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef CPU_H_ONLY_ONE_INCLUDE_SAFETY
#define CPU_H_ONLY_ONE_INCLUDE_SAFETY

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
	Include files
******************************************************************************/

#include <stdint.h>	/* uint32_t */

/******************************************************************************
	Custom definitions
******************************************************************************/


/*  ---   DO NOT TOUCH BELOW THIS LINE!   ---  */

/******************************************************************************
	Definitions
******************************************************************************/

/******************************************************************************
	Type definitions
******************************************************************************/

/**
 * CPU sleep modes.
 */
typedef enum cpu_sleep_mode
{
	CSM_ACTIVE	= 0,    /**< Active mode */
	CSM_SLEEP_0	= 1,    /**< Sleep mode 0 */
	CSM_SLEEP_1	= 2,    /**< Sleep mode 1 */
	CSM_SLEEP_2	= 3,    /**< Sleep mode 2 */
	CSM_SLEEP_3	= 4,    /**< Sleep mode 3 */
	CSM_WAIT	= 5,    /**< Wait mode */
	CSM_RET		= 6,    /**< Retention mode */
	CSM_BACKUP	= 7		/**< Backup mode */
} cpu_sleep_mode_t;

/******************************************************************************
	Function declarations
******************************************************************************/

uint32_t	cpu_config_clock( void );

//uint32_t	cpu_is_in_debug( void );
void		cpu_reset( void );
//uint32_t	cpu_sleep( const cpu_sleep_mode_t mode );
void		cpu_delay_ms( uint32_t ms );

#ifdef __cplusplus
}
#endif

#endif /* CPU_H_ONLY_ONE_INCLUDE_SAFETY */
