#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
	char buf[] = "Hello";

	printf("sizeof(buf):%lu\n", sizeof(buf));
	printf("strlen(buf):%d\n", strlen(buf));

	return 0;

}