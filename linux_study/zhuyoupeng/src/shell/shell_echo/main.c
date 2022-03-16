#include <stdio.h>
#include <string.h>

#define MAX_LINE_CMD	256		// 命令长度

typedef struct {
	char cmd[MAX_LINE_CMD];		// 命令
	int (*pfunc)(void);	// 操作函数
}cmd_list;

int main(int argc, char const *argv[])
{
	char cmd_buf[MAX_LINE_CMD] = {0};
	while(1) {
		// 打印命令行提示符
		printf("Master#:");
		// 命令输入
		memset(cmd_buf, 0x00, sizeof(cmd_buf));
		scanf("%s", cmd_buf);
		// 命令解析
		if(strstr(cmd_buf, "exit")) {
			break;
		}
		// 命令处理
		printf("%s\n", cmd_buf);

	}

	return 0;
}