/**	\mainpage Flash Config module for STM32
 * fc.h
 *	\version 1
 *	\author	Major
 *	\date 10 March 2021
 *
 *	\section intro Introduction
 *	This module purpose is to hide lower lever NVS write/read operations and able to save configuration variables on flash memory of STM32. (Tested on F091).
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

/** 	@defgroup flash_config
 * 		\brief Flash configuration values. @{  */

#define FC_NVS_OFFSET			(0x0801F000)
#define FC_NVS_SECTOR_SIZE		(FLASH_PAGE_SIZE)
#define FC_NVS_SECTOR_COUNT		(2)

typedef enum {FC_INT, FC_UINT, FC_FLOAT, FC_STR} config_types_en;
typedef enum {FC_OK, FC_ERR_INVALID_PARAM, FC_ERR_NOT_FOUND, FC_ERR_NO_FLASH, FC_ERR_DESCRIPTOR_ERR, FC_ERR_CORRUPTED_DATA} fc_err_t;

typedef struct {
	int as_int;
	uint32_t as_uint;
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
	uint16_t KEY;
} fc_config_t;

/**	\brief Initialization.
 *  Call this function before use setter-getters.
 *	Reading configuration descriptor.
 *
 * 	\param	descriptor		- fc_config_t array which contains information about configuration values
 * 							It must be a statically allocated descriptor array.
 * 	\param	descr_length	- length of this array
 * */
fc_err_t fc_init(fc_config_t *descriptor, int descr_length);

/**	\brief Setter functions.
 * 	\param	name		- key or name of the data (must be contained in descriptor array)
 * 	\param	val			- input data
 * 	\param	write		- write (not null) or just store the data (0)
 * 							 - when 0 you should later call fc_save_running() function to write changes to flash
 * 	\return	fc_err_t error codes.
 * */
fc_err_t fc_set_int(const char *name, int val);
fc_err_t fc_set_uint(const char *name, uint32_t val);
fc_err_t fc_set_float(const char *name, float val);
fc_err_t fc_set_str(const char *name, char *val);

/**	\brief Getter functions.
 * 	\param	name		- key or name of the data
 * 	\return	fc_err_t error codes.
 * */
fc_err_t fc_get_int(const char *name, int *out);
fc_err_t fc_get_uint(const char *name, uint32_t *out);
fc_err_t fc_get_float(const char *name, float *out);
fc_err_t fc_get_str(const char *name, char *out);

/**			@} */ //end of Flash_config_include group  */
#endif /* FC_H_ */

