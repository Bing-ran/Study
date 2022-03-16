#include "stdio.h"

void uart_init(void);

int main(void)
{
	int i = 0, flag = 0;
	uart_init();

	while(i >= 0){
		//uart_getchar();
		//uart_putchar('a' + i);
		printf("%s：%c\n", "hello", 'c' + i);
		delay();
	
		if ((flag == 0) && (++i == 'z' - 'a'))
		{
			flag = 1;
		}
		else if ((flag == 1) && (--i == 0))
		{
			flag = 0;
		}
		
	}

	return 0;
}