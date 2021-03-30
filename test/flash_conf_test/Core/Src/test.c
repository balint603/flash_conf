/*
 * test.c
 *
 *  Created on: 2021. márc. 26.
 *      Author: major
 */



#include "main.h"
#include "fc.h"
#include "nvs.h"
#include "shell.h"

static const char *TAG = "Test fc - ";

const char *setget_int_name = "setgetint";
const char *setget_ui_name = "uiuiuiui";
const char *setget_fl_name = "setgetfl";


const int descr_len = 3;
fc_config_t descriptor[3] = {
		{ .name = "setgetint", .type = FC_INT, .min_value = { .as_int = -1}, .max_value = { .as_int = 10000}, .default_value = { .as_int = 500 }, .KEY = 156 },
		{ .name = "uiuiuiui", .type = FC_UINT, .min_value = { .as_uint = 0}, .max_value = { .as_uint = 10000}, .default_value = { .as_uint = 300 }, .KEY = 999 },
		{ .name = "setgetfl", .type = FC_FLOAT, .min_value = { .as_float = -10}, .max_value = { .as_float = 10000}, .default_value = { .as_float = 1.1 }, .KEY = 157 }
};

struct nvs_fs g_nvs = {
		.offset = 0x0801F000,
		.sector_size = FLASH_PAGE_SIZE,
		.sector_count = 2
};

int test_setget_int() {
	pr_info("%s Test SetGet INT____________", TAG);
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

int test_setget_uint() {
	pr_info("%s Test SetGet UINT____________", TAG);
	int result = 0;
	uint32_t outval = 0;
	int res = fc_get_uint(setget_ui_name, &outval);
	if ( FC_OK == res ) {
		pr_info("%s Got from fc.", TAG);
	} else {
		pr_info("%s Error from fc_get_int: %i", TAG, res);
		result = 1;
	}
	pr_info("%s Read fc value. Name: %s, Value: %X", TAG, setget_ui_name, outval);

	res = fc_set_uint(setget_ui_name, ++outval);
	if ( FC_OK != res ) {
		pr_info("%s Cannot save fc value. Name %s, Value: %X Errcode=%i", TAG, setget_ui_name, outval, res);
	}
	return result;
}

int test_setget_float() {
	pr_info("%s Test SetGet FLOAT__________", TAG);
	const float incr_val = 0.1;
	int result = 0;
	float outval = 0;
	int res = fc_get_float(setget_fl_name, &outval);
	if ( FC_OK == res ) {
		pr_info("%s Got from fc.", TAG);
	} else {
		pr_info("%s Erro from fc_get_float: %i", TAG, res);
		result = 1;
	}
	pr_info("%s Read fc value. Name: %s, Value: %f", TAG, setget_fl_name, outval);

	outval += incr_val;
	res = fc_set_float(setget_fl_name, outval);
	if ( FC_OK != res ) {
		pr_info("%s Cannot save fc value. Name %s, Value: %f Errcode=%i", TAG, setget_fl_name, outval, res);
	}
	return result;
}

int test_descriptor_check() {
	pr_info("%s Start descriptor check test", TAG);
	fc_deinit();
	fc_config_t desc_limit_d[] = {
		{ .name = "floatLimitD", .type = FC_FLOAT, .min_value = { .as_float = -1}, 	.max_value = { .as_float = 600}, 	.default_value = { .as_float = -2 }, .KEY = 11 }
	};
	fc_config_t desc_limit_u[] = {
		{ .name = "floatLimitU", .type = FC_FLOAT, .min_value = { .as_float = -1}, 	.max_value = { .as_float = 1000}, 	.default_value = { .as_float = 1001 }, .KEY = 12 }
	};
	fc_config_t desc_wrong_limits[] = {
		{ .name = "floatWrongL", .type = FC_FLOAT, .min_value = { .as_float = 100}, .max_value = { .as_float = 50}, 	.default_value = { .as_float = 75 }, .KEY = 13 }
	};
	fc_config_t desc_same_name[] = {
		{ .name = "SameName", .type = FC_INT, 	.min_value = { .as_int = -1}, .max_value = { .as_int = 1000}, .default_value = { .as_int = 500 }, .KEY = 14 },
		{ .name = "SameName", .type = FC_FLOAT, .min_value = { .as_int = -1}, .max_value = { .as_int = 1000}, .default_value = { .as_int = 500 }, .KEY = 15 }
	};
	int res = 0;

	if ( FC_ERR_INVALID_PARAM != fc_init(desc_limit_d, 1, &g_nvs) ) {
		pr_info("%s Default is lower then lower limit test failed", TAG);
		res += 1;
	}
	fc_deinit();
	if ( FC_ERR_INVALID_PARAM != fc_init(desc_limit_u, 1, &g_nvs) ) {
		pr_info("%s Default is greater then upper limit test failed", TAG);
		res += 10;
	}
	fc_deinit();
	if ( FC_ERR_INVALID_PARAM != fc_init(desc_wrong_limits, 1, &g_nvs) ) {
		pr_info("%s Reverse limit test failed", TAG);
		res += 100;
	}
	fc_deinit();
	if ( FC_ERR_INVALID_PARAM != fc_init(desc_same_name, 2, &g_nvs) ) {
		pr_info("%s Same name test failed", TAG);
		res += 1000;
	}
	return res;
}

void test_start() {
	pr_info("%s start", TAG);
	if ( fc_init(descriptor, descr_len, &g_nvs) )	pr_err("%s fc_init failed", TAG);
	if ( test_setget_int() )				pr_err("%s setget_int test failed", TAG);
	if ( test_setget_uint() )				pr_err("%s setget_uint test failed", TAG);
	if ( test_setget_float() )				pr_err("%s setget_float test failed", TAG);
	if ( test_descriptor_check() )			pr_err("%s descriptor check test failed", TAG);
}
































