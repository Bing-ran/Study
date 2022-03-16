/*
 * @Author: your name
 * @Date: 2021-07-18 21:23:48
 * @LastEditTime: 2021-07-18 21:52:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \my_shell\main.h
 */
#ifndef __MAIN_H_
#define __MAIN_H_

#include "uart.h"

#define MAX_LINE_CMD	256		// 命令长度

typedef struct {
	char cmd[MAX_LINE_CMD];		// 命令
	int (*pfunc)(void);	// 操作函数
}cmd_list;

#endif // !1

