#include <stdio.h>

int main(int argc, char const *argv[],char **env)
{
	int i = 0;
	while(NULL != argv[i])
	{
		printf("argv[%d] = %s\n", i,argv[i]);
		i++;
	}

	i = 0;
	while(NULL != env[i])
	{
		printf("env[%d] = %s\n", i,env[i]);
		i++;
	}


	return 0;
}