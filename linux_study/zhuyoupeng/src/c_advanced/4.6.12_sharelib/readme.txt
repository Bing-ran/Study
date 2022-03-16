1、 制作静态库
	.c --> .o   -fPIC
	使用ar工具制作.a文件
	gcc -o libxxx.so xxx.o -shared

2、只用静态库
	gcc ...... -lxxx -L (pwd)



/*把库添加到   LD_LIBRARY_PATH*/
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ubuntu16/sambashare/c_advanced/4.6.12_sharelib/ku