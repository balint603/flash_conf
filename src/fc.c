/**
 * fc.c
 *
 *  Created on: 2021. márc. 10.
 *      Author: major
 */

#include <string.h>
#include <limits.h>
#include <float.h>
#include <errno.h>

#include "stm32f0xx.h"
#include "fc.h"
#include "shell.h"
#include "nvs.h"


const char *TAG = "Flash Config - ";

int g_initialized;
fc_config_t *g_descriptor_ptr;
size_t g_descr_len;

struct nvs_fs g_nvs = {
		.offset = FC_NVS_OFFSET,
		.sector_size = FLASH_PAGE_SIZE,
		.sector_count = 2
};


static int get_index_from_descriptor(const char *name) {
	if ( !g_descriptor_ptr || !name )
		return -1;
	for ( int i = 0; i < g_descr_len; i++ ) {
		if ( !strcmp(g_descriptor_ptr[i].name, name) ) {
			return i;
		}
	}
	return -1;
}

static int count_name_in_descriptor(const char *name, fc_config_t *descr_arr, int len) {
	int cnt = 0;
	for (int i = 0; i < len; i++) {
		if ( !strcmp(name, descr_arr[i].name) )
			cnt++;
	}
	return cnt;
}

static int check_data_type_in_descriptor(int index, config_types_en type) {
	if ( !g_descriptor_ptr )
		return -1;
	if ( g_descriptor_ptr[index].type != type )
		return 1;
	return 0;
}

/**	\brief Modul initialization.
 * 	Must be called before any of its functions.
 * 	\return FC_ERR_INVALID_PARAM	- NULL input
 * 			FC_ERR_DESCRIPTOR_ERR	- A problem with descriptor but no fatal.
 * */
fc_err_t fc_init(fc_config_t *descriptor, int descr_length) {
	fc_err_t res = FC_OK;
	if ( g_initialized ) {
		return FC_ERR_NOT_FOUND;
	}
	if ( !descriptor )
		return FC_ERR_INVALID_PARAM;
	for ( int i = 0; i < descr_length; i++ ) {

		if ( !descriptor[i].name )
			pr_warning("%s: Descriptor name cannot be NULL! At %i", TAG, i);

		if ( 1 < count_name_in_descriptor(descriptor[i].name, descriptor, descr_length) ) {		// Check the occurrence of names.
			pr_warning("%s: Descriptor name [%s] should not be the same! ", TAG, descriptor[i].name);
			pr_warning("%s: Only the first descriptor [named=%s] will be handled.", TAG, descriptor[i].name);
		}

		switch ( descriptor[i].type ) {
			case FC_INT:
				if ( descriptor[i].min_value.as_int > descriptor[i].max_value.as_int ) {
					pr_warning("%s: Invalid descriptor limits: name=%s, min=%i, max=%i, set to standard limits",
							descriptor[i].min_value.as_int, descriptor[i].max_value.as_int);
					descriptor[i].min_value.as_int = INT_MIN;
					descriptor[i].max_value.as_int = INT_MAX;
				}
				if ( descriptor[i].default_value.as_int < descriptor[i].min_value.as_int
			      && descriptor[i].default_value.as_int > descriptor[i].max_value.as_int ) {
					res = FC_ERR_DESCRIPTOR_ERR;
					pr_warning("%s: Invalid descriptor values: name=%s min=%i max=%i default=%i, set to min.",
							 TAG, descriptor[i].name, descriptor[i].min_value.as_int, descriptor[i].max_value.as_int,descriptor[i].default_value.as_int);
					descriptor[i].default_value.as_int = descriptor[i].min_value.as_int;
				}
				break;
			case FC_UINT:
				if ( descriptor[i].min_value.as_uint > descriptor[i].max_value.as_uint ) {
					pr_warning("%s: Invalid descriptor limits: name=%s, min=%i, max=%i, set to standard limits",
							descriptor[i].min_value.as_uint, descriptor[i].max_value.as_uint);
					descriptor[i].min_value.as_uint = 0;
					descriptor[i].max_value.as_uint = UINT_MAX;
				}
				if ( descriptor[i].default_value.as_uint < descriptor[i].min_value.as_uint
				  && descriptor[i].default_value.as_uint > descriptor[i].max_value.as_uint ) {
					res = FC_ERR_DESCRIPTOR_ERR;
					pr_warning("%s: Invalid descriptor values: name=%s min=%X max=%X default=%X, set to min.",
							 TAG, descriptor[i].name, descriptor[i].min_value.as_uint, descriptor[i].max_value.as_uint,descriptor[i].default_value.as_uint);
					descriptor[i].default_value.as_uint = descriptor[i].min_value.as_uint;
				}
				break;
			case FC_FLOAT:
				if ( descriptor[i].min_value.as_float > descriptor[i].max_value.as_float ) {
					pr_warning("%s: Invalid descriptor limits: name=%s, min=%i, max=%i, set to standard limits",
							descriptor[i].min_value.as_float, descriptor[i].max_value.as_float);
					descriptor[i].min_value.as_float = FLT_MIN;
					descriptor[i].max_value.as_float = FLT_MAX;
				}
				if ( descriptor[i].default_value.as_float < descriptor[i].min_value.as_float
				  && descriptor[i].default_value.as_float > descriptor[i].max_value.as_float ) {
					res = FC_ERR_DESCRIPTOR_ERR;
					pr_warning("%s: Invalid descriptor values: name=%s min=%f max=%f default=%f, set to min.",
							 TAG, descriptor[i].name, descriptor[i].min_value.as_float, descriptor[i].max_value.as_float,descriptor[i].default_value.as_float);
					descriptor[i].default_value.as_float = descriptor[i].min_value.as_float;
				}
				break;
			case FC_STR:

				break;
			default:
				break;
		}
	}
	if ( FC_OK == res )
		pr_info("%s Descriptor check passed.", TAG);

	int nvs_res = nvs_init(&g_nvs, TAG);
	if ( nvs_res ) {
		pr_err("%s nvs initializing error. Code: %i", TAG, res);
		res = FC_ERR_NO_FLASH;
	} else {
		g_descr_len = descr_length;
		g_descriptor_ptr = descriptor;
		g_initialized = 1;
	}

	return res;
}



fc_err_t fc_set_int(const char *name, int val) {
	fc_err_t retval = FC_OK;

	int i = get_index_from_descriptor(name);
	if ( i < 0 ) {
		retval = FC_ERR_INVALID_PARAM;
		goto err;
	}

	if ( g_descriptor_ptr[i].min_value.as_int > val
	 ||  g_descriptor_ptr[i].max_value.as_int < val ) {
		retval = FC_ERR_INVALID_PARAM;
		goto err;
	}

	if ( check_data_type_in_descriptor(g_descriptor_ptr[i].type, FC_INT) ) {
		pr_err("%s Wrong setter function is called for config value: %s", TAG, g_descriptor_ptr[i].name);
		retval = FC_ERR_INVALID_PARAM;
		goto err;
	}

	int nvs_ret = nvs_write(&g_nvs, g_descriptor_ptr[i].KEY, &val, sizeof(int));
	if ( nvs_ret != sizeof(int)) {
		retval = FC_ERR_NO_FLASH;
		goto err;
	}
	pr_info("%s %s=%i saved.", TAG, name, val);
err:
	return retval;
}

fc_err_t fc_set_uint(const char *name, uint32_t val);
fc_err_t fc_set_float(const char *name, float val);
fc_err_t fc_set_str(const char *name, char *val);

fc_err_t fc_get_int(const char *name, int *out) {
	fc_err_t retval = FC_OK;
	if ( !name || !out) {
		retval = FC_ERR_INVALID_PARAM;
		goto err;
	}
	int i = get_index_from_descriptor(name);
	if ( i < 0 ) {
		retval = FC_ERR_INVALID_PARAM;
		goto err;
	}

	*out = g_descriptor_ptr[i].default_value.as_int;

	if ( check_data_type_in_descriptor(g_descriptor_ptr[i].type, FC_INT) ) {
		pr_err("%s Wrong getter function is called for config value: %s", TAG, g_descriptor_ptr[i].name);
		retval = FC_ERR_INVALID_PARAM;
		goto err;
	}

	int outval;
	int nvs_ret = nvs_read(&g_nvs, g_descriptor_ptr[i].KEY, &outval, sizeof(int));
	if ( nvs_ret == sizeof(int) ) {
		if ( g_descriptor_ptr[i].min_value.as_int > outval
		   ||  g_descriptor_ptr[i].max_value.as_int < outval ) {
			retval = FC_ERR_NO_FLASH;
		} else {
			*out = outval;
		}
	} else {
		pr_err("%s Cannot get from nvs. Code: %i", TAG, nvs_ret);
		retval = FC_ERR_NOT_FOUND;
	}
err:
	return retval;
}
fc_err_t fc_get_uint(const char *name, uint32_t *out);
fc_err_t fc_get_float(const char *name, float *out);
fc_err_t fc_get_str(const char *name, char *out);




















