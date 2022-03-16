#include <stdio.h>
#include <unistd.h>

#define FILENAME "file.txt"
int main(int argc, char const *argv[])
{
	int ret = -1;

	ret = access(FILENAME,F_OK & X_OK);
	if (ret == 0)
	{
		printf("the file <%s> exist\n", FILENAME);
	} else {
		printf("the file <%s> no exist\n", FILENAME);
	}

	printf("%d\t%d\t%d\n",W_OK,R_OK,X_OK);	
	return 0;
}