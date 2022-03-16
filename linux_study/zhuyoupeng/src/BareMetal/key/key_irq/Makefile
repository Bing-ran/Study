CC 		= arm-none-linux-gnueabi-gcc
LD 		= arm-none-linux-gnueabi-ld
OBJCOPY = arm-none-linux-gnueabi-objcopy
OBJDUMP = arm-none-linux-gnueabi-objdump
AR 		= arm-none-linux-gnueabi-ar

INCDIR 	:= $(shell pwd)
# C预处理器的falg，flag就是编译器可选的选项
CPPFLAGS	:= -nostdlib -nostdinc -I$(INCDIR)/include
# C编译器的falg
CFLAGS 		:= -Wall -O2 -fno-builtin

# 导出这些变量到全局，其实就是给子文件夹下面的Makefile使用
export CC LD OBJCOPY OBJDUMP AR	CPPFLAGS CFLAGS

objs := start.o led.o uart.o main.o key.o int.o
objs += lib/libc.a

key.bin: $(objs)
	$(LD) -Tlink.lds -o key.elf $^
	$(OBJCOPY) -O binary key.elf key.bin
	$(OBJDUMP) -D key.elf > key.dis
	gcc mkv210_image.c -o x210
	./x210 key.bin 210.bin
	
lib/libc.a:
	cd lib;	make;	cd ..

%.o : %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< -c

%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< -c

clean:
	rm *.o *.elf *.bin *.dis x210 -f
	cd lib; make clean; cd ..

	
	
