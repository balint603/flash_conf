/*
 * test.c
 *
 *  Created on: 2021. márc. 11.
 *      Author: major
 */


#include "fc.h"

const char *VAL_INT_OK = "intok";
const char *VAL_INT_ABOVE = "intab";
const char *VAL_INT_BELOW = "intbe";
const char *VAL_FL_OK = "flok";
const char *VAL_FL_ABOVE = "flab";
const char *VAL_FL_BELOW = "flbe";
const char *VAL_STR_OK = "Strval";

const size_t descriptor_n = 9;
const fc_config_t descriptor[] = {
		{.name = VAL_INT_OK, 		.default_value = {.as_int = 50}, .max_value = {.as_int = 100}, .min_value = {.as_int = 0}, .type = FC_INT},
		{.name = VAL_INT_ABOVE, 	.default_value = {.as_int = 50}, .max_value = {.as_int = 100}, .min_value = {.as_int = 0}, .type = FC_INT},
		{.name = VAL_INT_BELOW,		.default_value = {.as_int = 50}, .max_value = {.as_int = 100}, .min_value = {.as_int = 0}, .type = FC_INT},
		{.name = VAL_FL_OK, 		.default_value = {.as_float = 50.5}, .max_value = {.as_float = 100}, .min_value = {.as_float = 0}, .type = FC_FLOAT},
		{.name = VAL_FL_ABOVE, 		.default_value = {.as_float = 50.5}, .max_value = {.as_float = 100}, .min_value = {.as_float = 0}, .type = FC_FLOAT},
		{.name = VAL_FL_BELOW, 		.default_value = {.as_float = 50.5}, .max_value = {.as_float = 100}, .min_value = {.as_float = 0}, .type = FC_FLOAT},
		{.name = VAL_STR_OK, 		.default_value = {.as_string = "Default"}, .type = FC_STR}
};

int test_write_success() {
	const int intvalue = 59;
	int outvalue = -1;
	fc_set_int(VAL_INT_OK, intvalue, 1);
	if (  )
		return -1;
	return 0;
}

int test_invalid_values() {
	return 0;
}

void test_start() {

	fc_init(descriptor, descriptor_n);
	if ( test_write_success() )	sh_log_printf("test_write_success failed.");
	if ( test_invalid_value() )	sh_log_printf("test_invalid_value failed.");
}
