/**	\mainpage Flash Config module for STM32
 * fc.h
 *	\version 1
 *	\author	Major
 *	\date 10 March 2021
 *
 *	\section intro Introduction
 *	This module purpose is to hide lower lever EEPROM write/read operations and able to save configuration variables on flash memory of STM32. (Tested on F091).
 *
 *  A configuration value can be defined with the following data:
 *  -	name				- a short string which is basically a key to the variable
 *  -	type				- int, float or char* array is supported yet
 *  -	min value
 *  -	max value
 *  -	default value
 *
 * \subsection descr Using this module
 * 	-	EEPROM module must be configured and initialized before using this module!
 *	-	??????????????????????????????????????????????????,
 *
 */

#ifndef FC_H_
#define FC_H_

#include "eeprom.h"

#endif /* FC_H_ */
