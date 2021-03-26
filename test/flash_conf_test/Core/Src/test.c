/*
 * test.c
 *
 *  Created on: 2021. márc. 26.
 *      Author: major
 */



#include "main.h"
#include "fc.h"
#include "shell.h"

static const char *TAG = "Test fc - ";

const char *setget_int_name = "setgetint";

const int descr_len = 1;
fc_config_t descriptor[1] = {
		{ .name = "setgetint", .type = FC_INT, .min_value = { .as_int = -1}, .max_value = { .as_int = 1000}, .default_value = { .as_int = 500 }, .KEY = 56 }
};

int test_setget_int() {
	int result = 0;
	int outval = 0;
	int res = fc_get_int(setget_int_name, &outval);
	if ( FC_OK == res ) {
		pr_info("%s Got from fc.", TAG);
	} else {
		pr_info("%s Error from fc_get_int: %i", TAG, res);
		result = 1;
	}
	pr_info("%s Read fc value. Name: %s, Value: %i", TAG, setget_int_name, outval);

	res = fc_set_int(setget_int_name, ++outval);
	if ( FC_OK != res ) {
		pr_info("%s Cannot save fc value. Name %s, Value: %i Errcode=%i", TAG, setget_int_name, outval, res);
	}
	return result;
}

void test_start() {
	pr_info("%s start", TAG);
	if ( fc_init(descriptor, descr_len) )	pr_err("%s fc_init failed", TAG);
	if ( test_setget_int() )	pr_err("%s setget_int test failed", TAG);
}
