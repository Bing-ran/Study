1、 制作静态库
	.c --> .o
	使用ar工具制作.a文件
	ar -rc libxxx.a xxx.o

2、只用静态库
	gcc ...... -lxxx -L (pwd)