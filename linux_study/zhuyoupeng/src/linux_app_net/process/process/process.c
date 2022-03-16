#include <stdio.h>
#include <stdlib.h>

void exit_r(void)
{
	printf("%s\n", "func");
}

int main(int argc, char const *argv[])
{
	
	printf("Hello World!\n");


	atexit(exit_r);

	printf("%s\n", "My name is lilei and hanmeimei!");

	return 0;
}