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

typedef enum {FC_INT, FC_FLOAT, FC_STR} config_types_en;

typedef struct {
	int as_int;
	float as_float;
	char *as_string;
} fc_val_t;

/** A configuration value object. The descriptor array consist of these. */
typedef struct {
	char *name;
	config_types_en type;
	fc_val_t min_value;
	fc_val_t max_value;
	fc_val_t default_value;
} fc_config_t;

/**	\brief Initialization.
 * 	Call this function before use setter-getters.
 *	Reading configuration descriptor and create virtual addresses to EEPROM.
 *
 * 	\param	descriptor		- fc_config_t array which contains information about configuration values
 * 			descr_length	- length of this array
 * */
void fc_init(fc_config_t *descriptor, int descr_length);

/** \brief Save changed values to flash.
 * */
void fc_write_changed();

/**	\brief Setter functions.
 * 	\param 	name		- key or name of the data (must be contained in descriptor array)
 * 			val			- input data
 * 			write		- write or just store the data
 * 							 when false you should later call fc_write_change() function to write changes to flash
 * 	\return	true		- value is saved
 * 			false		- cannot be saved or wrong value
 * */
bool fc_set_int(char *name, int val, bool write);
bool fc_set_float(char *name, float val, bool write);
bool fc_set_str(char *name, char *val, bool write);

/**	\brief Getter functions.
 * 	\param	name		- key or name of the data
 * 	\return 			- output value from flash or default value (see descriptor array) if cannot be found
 *
 * */
int fc_get_int(char *name);
float fc_get_float(char *name);
char *fc_get_str(char *name);


#endif /* FC_H_ */
