#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	int ret = 0;

	srand(time(NULL));
	for (int i = 0; i < 10; ++i)
	{
		printf("%d ", (rand() % 10));
	}
	
	printf("%s\n", "\n");

	return 0;
}