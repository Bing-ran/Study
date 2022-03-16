/*
 * @Author: your name
 * @Date: 2021-07-18 21:47:53
 * @LastEditTime: 2021-07-18 22:01:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \my_shell\main.c
 */
#include "main.h"

void main(void)
{
	char cmd_buf[MAX_LINE_CMD] = {0};
	
	uart_init();
	puts("Master_r shell\n");
	
	while(1){
		puts("Master_r #");
		memset(cmd_buf, 0x00, sizeof(cmd_buf));
		// 获取输入命令
		gets(cmd_buf);

		// 解析命令

		// 执行命令
		puts(cmd_buf);
		puts("\n");
	}
}