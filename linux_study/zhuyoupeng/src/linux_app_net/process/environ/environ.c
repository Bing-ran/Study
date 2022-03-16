#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int i = 0;
	extern char **environ;
#if 1
	while(NULL != environ[i++]){
		printf("environ[%d] = %s\n", i-1,environ[i-1]);
	}
#else
	char *s = getenv("PATH");
	printf("%s\n", s);
#endif
	return 0;
}