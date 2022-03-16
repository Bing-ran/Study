#include <stdio.h>

int a =1;

int main(int argc, char const *argv[])
{
	a = 2;
	printf("%s-%d, a : %d\n", __func__, __LINE__, a);
	return 0;
}