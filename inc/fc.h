/**	\mainpage Flash Config module for STM32
 * fc.h
 *	\version 1
 *	\author	Major
 *	\date 10 March 2021
 *
 *	\section intro Introduction
 *	This module purpose is to hide lower lever NVS operations and able to save configuration variables on flash memory of STM32. (Tested on F091).
 *	Extra safety is added with defining descriptor table because the user is able to choose limits and default value of a specific variable.
 * 	Thread-safe is granted on (NVS module) low level.
 *
 *	\subsection supported Currently supported types are:
 *  	- uint32_t
 *  	- int
 *  	- float
 *  	- (for str and blob use nvs module)
 *
 *  A configuration value can be defined with the following data:
 *  -	name				- a short string which is basically a key to the variable
 *  -	type				- choose uint32_t, int, float value
 *  -	MIN value			- lower limit of the variable
 *  -	MAX value			- upper limit of the variable
 *  -	default value		- default value
 *
 *
 *\section howto How to import this module (CUBE IDE, ECLIPSE)
 *	1. 	You can clone or download the repository from https://github.com/balint603/flash_conf.git.
 *	2.	Create a linked folder at <ProjectName>->New->Folder. You should rename it to help to recognize it.
 * 	3.	Go to the Project->Properties->Paths and Symbols. Then add the project inc folder then the linked src folder.
 */

#ifndef FC_H_
#define FC_H_

#include "nvs.h"

/** 	@defgroup flash_config
 * 		\brief Flash configuration variables.
 *		Use fc_init() function before using any of setter/getter functions.
 *
 *		Getter functions always return a value when the name parameter can be found in the configuration (fc_config_t) array.
 *		Default value is returned when there is no such entry saved to flash yet but found in the configuration array.
 *
 * 		@{
 */

//#define FC_STR_GET_SET
/** Variable types. */
typedef enum {FC_INT, FC_UINT, FC_FLOAT, FC_STR} config_types_en;

/** \brief Return codes. */
typedef enum {FC_OK, FC_ERR_INVALID_PARAM, FC_ERR_NOT_FOUND, FC_ERR_NO_FLASH, FC_ERR_CORRUPTED_DATA} fc_err_t;

/** \brief Configuration value data type. */
typedef struct {
	int as_int;
	uint32_t as_uint;
	float as_float;
	char *as_string;
} fc_val_t;

/** \brief Configuration value object. The descriptor array consist of these.
 * 	This structure defines a configuration variable.
 * */
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
 * 	\param	descriptor					- fc_config_t array which contains information about configuration values
 * 							It must be a statically allocated descriptor array.
 * 	\param	descr_length				- length of this array
 * 	\return		FC_OK					- descriptor check is completed without error
 * 	\return		FC_ERR_INVALID_PARAM	- wrong descriptor
 * 	\return		FC_ERR_NO_FLASH			- NVS initialization error occured
 * 	\return		FC_ERR_NOT_FOUND		- already initialized
 * */
fc_err_t fc_init(fc_config_t *descriptor, int descr_length, struct nvs_fs *nvs);

/**	\brief Uninit.
 * */
void fc_deinit();

/**	\brief Setter functions.
 * 	\param	name					- key or name of the data (must be contained in descriptor array)
 * 	\param	val						- input data
 * 	\return	FC_ERR_INVALID_PARAM 	- name has not been found in the descriptor
 * 	\return FC_ERR_NO_FLASH		 	- NVS write error
 * 	\return FC_OK					- saved successfully
 * */
fc_err_t fc_set_int(const char *name, int val);

/**	\brief Setter functions.
 * 	\param	name					- key or name of the data (must be contained in descriptor array)
 * 	\param	val						- input data
 * 	\return	FC_ERR_INVALID_PARAM 	- name has not been found in the descriptor
 * 	\return FC_ERR_NO_FLASH		 	- NVS write error
 * 	\return FC_OK					- saved successfully
 * */
fc_err_t fc_set_uint(const char *name, uint32_t val);

/**	\brief Setter functions.
 * 	\param	name					- key or name of the data (must be contained in descriptor array)
 * 	\param	val						- input data
 * 	\return	FC_ERR_INVALID_PARAM 	- name has not been found in the descriptor
 * 	\return FC_ERR_NO_FLASH		 	- NVS write error
 * 	\return FC_OK					- saved successfully
 * */
fc_err_t fc_set_float(const char *name, float val);
#ifdef FC_STR_GET_SET
/**	\brief Setter functions.
 * 	\param	name		- key or name of the data (must be contained in descriptor array)
 * 	\param	val			- input data
 * 	\return	FC_ERR_INVALID_PARAM 	- name has not been found in the descriptor
 * 	\return FC_ERR_NO_FLASH		 	- NVS write error
 * */
fc_err_t fc_set_str(const char *name, char *val);
#endif

/**	\brief Getter functions.
 * 	\param	name		- key or name of the data
 * 	\param 	out			- ptr of buffer
 *	\return FC_ERR_INVALID_PARAM	- NULL input or cannot find in descriptor
 *	\return	FC_ERR_NO_FLASH			- data is corrupted, default value has been returned
 *  \return FC_ERR_NOT_FOUND		- cannot get from NVS
 * */
fc_err_t fc_get_int(const char *name, int *out);

/**	\brief Getter functions.
 * 	\param	name		- key or name of the data
 * 	\param 	out			- ptr of buffer
 *	\return FC_ERR_INVALID_PARAM	- NULL input or cannot find in descriptor
 *	\return FC_ERR_NO_FLASH			- data is corrupted, default value has been returned
 *	\return FC_ERR_NOT_FOUND		- cannot get from NVS
 * */
fc_err_t fc_get_uint(const char *name, uint32_t *out);

/**	\brief Getter functions.
 * 	\param	name		- key or name of the data
 * 	\param 	out			- ptr of buffer
 *	\return FC_ERR_INVALID_PARAM	- NULL input or cannot find in descriptor
 *	\return FC_ERR_NO_FLASH			- data is corrupted, default value has been returned
 *	\return FC_ERR_NOT_FOUND		- cannot get from NVS
 * */
fc_err_t fc_get_float(const char *name, float *out);
#ifdef FC_STR_GET_SET
/**	\brief Getter functions.
 * 	\param	name		- key or name of the data
 * 	\param 	out			- ptr of buffer
 *	\return FC_ERR_INVALID_PARAM	- NULL input or cannot find in descriptor
 *	\return FC_ERR_NO_FLASH			- data is corrupted, default value has been returned
 *	\return FC_ERR_NOT_FOUND		- cannot get from NVS
 * */
fc_err_t fc_get_str(const char *name, char *out);
#endif

/**			@} */ //end of Flash_config_include group  */
#endif /* FC_H_ */

