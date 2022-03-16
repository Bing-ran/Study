#include <stdio.h>
#include <stdarg.h>

void simple_va_fun(int i, ...)
{
	va_list arg_ptr;
	int j=0;

	va_start(arg_ptr, i);
	j=va_arg(arg_ptr, int);
	va_end(arg_ptr);
	printf("%d %d\n", i, j);
	return;
}


void main(void)
{

	simple_va_fun(10);

	simple_va_fun(11 , 12);



}

