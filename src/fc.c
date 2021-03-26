/**
 * fc.c
 *
 *  Created on: 2021. márc. 10.
 *      Author: major
 */

#include <string.h>
#include <limits.h>
#include <float.h>

#include "stm32f0xx.h"
#include "fc.h"
#include "shell.h"

fc_config_t *g_descriptor_ptr;
size_t g_descr_len;

const char *TAG = "Flash Config";


static int get_key_from_descriptor(const char *name) {
	if ( !g_descriptor_ptr || !name )
		return -1;
	for ( int i = 0; i < g_descr_len; i++ ) {
		if ( !strcmp(g_descriptor_ptr[i].name, name) ) {
			return g_descriptor_ptr[i].KEY;
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

/**	\brief Modul initialization.
 * 	Must be called before any of its functions.
 * 	\return FC_ERR_INVALID_PARAM	- NULL input
 * 			FC_ERR_DESCRIPTOR_ERR	- A problem with descriptor but no fatal.
 * */
fc_err_t fc_init(fc_config_t *descriptor, int descr_length) {
	fc_err_t res = FC_OK;
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
end:
	g_descr_len = descr_length;
	g_descriptor_ptr = descriptor;
	return res;
}

fc_err_t fc_set_int(char *name, int val) {

}
fc_err_t fc_set_uint(char *name, uint32_t val);
fc_err_t fc_set_float(char *name, float val);
fc_err_t fc_set_str(char *name, char *val);

fc_err_t fc_get_int(char *name, int *out);
fc_err_t fc_get_uint(char *name, uint32_t *out);
fc_err_t fc_get_float(char *name, float *out);
fc_err_t fc_get_str(char *name, char *out);




















