/*
 * cmds.c
 *
 *  Created on: 2021. márc. 9.
 *      Author: major
 */

#include <string.h>
#include "cmsis_os.h"

#include "stdbool.h"
#include "cmds.h"
#include "shell.h"

void cmd_test(int argc, char **argv) {
	// Do sg.
	sh_printf("Test command has been executed.");
}
