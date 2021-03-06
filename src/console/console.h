﻿/******************************************************************************
	About
******************************************************************************/

/**
 * \file console.h
 *
 * \brief	Console low level driver.
 * 
 * It uses the Proxy design pattern.
 *
 * Created:			30/04/2017
 *
 * \author	Elias Kanelis	hkanelhs@yahoo.gr
 */

/**
* \defgroup	Drivers	Drivers
*
* \code	#include <console.h>	\endcode
*/

/******************************************************************************
	Code
******************************************************************************/

#ifndef CONSOLE_H_ONLY_ONE_INCLUDE_SAFETY
#define CONSOLE_H_ONLY_ONE_INCLUDE_SAFETY

/******************************************************************************
	Definitions
******************************************************************************/

#define CONSOLE_STR_MAX_LEN				1000/**< Maximum lenght for the input string. */

/******************************************************************************
	Type definitions
******************************************************************************/

/**
 * \brief The console.
 */
typedef struct console
{
	char* (*gets)( struct console* me );	/**< Pointer to \ref gets function. */
	void (*puts)( const char* s );			/**< Pointer to \ref puts function. */
	void (*putNum)( const int num );		/**< Pointer to \ref putNum function. */
	char str[ CONSOLE_STR_MAX_LEN ];		/**< Char array that hold input data. */
} console_t;

/******************************************************************************
	Function declarations
******************************************************************************/

console_t console_build( void );

#endif /* CONSOLE_H_ONLY_ONE_INCLUDE_SAFETY */
