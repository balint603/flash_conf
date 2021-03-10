/*
 * cmds.h
 *
 *  Created on: 2021. márc. 9.
 *      Author: major
 */

#ifndef INC_CMDS_H_
#define INC_CMDS_H_

#define CMD_TEST_NAME	"test"
#define CMD_TEST	\
						{ .command_name =CMD_TEST_NAME, .function = cmd_test }

void cmd_test(int argc, char **argv);

#endif /* INC_CMDS_H_ */
