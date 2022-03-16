#include <stdio.h>

int main(void)
{
	char buf[] = {};
	
    fgets(buf, 128, stdin);

    printf("%s\r\n", buf);
    return 0;
}
