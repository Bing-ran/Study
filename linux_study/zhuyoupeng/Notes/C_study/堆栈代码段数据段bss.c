栈：小、申请用完后自动释放
	局部变量
堆：大、malloc申请 free释放

代码段：存放代码 + char *p = "linux"；(常量)

数据段：显示初始换非零的全局变量 + const 定义的局部变量 + 静态局部变量

bss段：显示初始化为0 或者 没有初始化的全局 的变量