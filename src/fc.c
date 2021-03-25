/**
 * fc.c
 *
 *  Created on: 2021. márc. 10.
 *      Author: major
 */

#include "stm32f0xx.h"
#include "fc.h"




fc_err_t fc_init(fc_config_t *descriptor, int descr_length) {
	fc_err_t res = FC_OK;
	for ( int i = 0; i < descr_length; i++ ) {
		switch ( descriptor[i].type ) {
			case FC_INT:
				if ( descriptor[i].default_value.as_int < descriptor[i].min_value.as_int
			      && descriptor[i].default_value.as_int > descriptor[i].max_value.as_int ) {
					res = FC_ERR_INVALID_PARAM;
					goto end;
				}
				break;
			case FC_UINT:
				if ( descriptor[i].default_value.as_uint < descriptor[i].min_value.as_uint
				  && descriptor[i].default_value.as_uint > descriptor[i].max_value.as_uint ) {
					res = FC_ERR_INVALID_PARAM;
					goto end;
				}
				break;
			case FC_STR:

				break;
			case FC_FLOAT:
				if ( descriptor[i].default_value.as_float < descriptor[i].min_value.as_float
				  && descriptor[i].default_value.as_float > descriptor[i].max_value.as_float ) {
					res = FC_ERR_INVALID_PARAM;
					goto end;
				}
				break;
			default:
				break;
		}
	}
end:
	return res;
}

fc_err_t fc_set_int(char *name, int val);
fc_err_t fc_set_uint(char *name, uint32_t val);
fc_err_t fc_set_float(char *name, float val);
fc_err_t fc_set_str(char *name, char *val);

fc_err_t fc_get_int(char *name, int *out);
fc_err_t fc_get_uint(char *name, uint32_t *out);
fc_err_t fc_get_float(char *name, float *out);
fc_err_t fc_get_str(char *name, char *out);




















