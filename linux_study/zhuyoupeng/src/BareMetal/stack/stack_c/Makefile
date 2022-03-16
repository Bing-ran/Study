stack.bin: start.o stack.o
	arm-none-linux-gnueabi-ld -Ttext 0x0 -o stack.elf $^
	arm-none-linux-gnueabi-objcopy -O binary stack.elf stack.bin
	arm-none-linux-gnueabi-objdump -D stack.elf > stack.dis
	gcc mkv210_image.c -o x210
	./x210 stack.bin 210.bin
	
%.o : %.S
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis x210 -f

	
	
