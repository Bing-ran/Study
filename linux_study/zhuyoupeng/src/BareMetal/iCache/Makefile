iCache.bin: iCache.o iCache.o
	arm-none-linux-gnueabi-ld -Ttext 0x0 -o iCache.elf $^
	arm-none-linux-gnueabi-objcopy -O binary iCache.elf iCache.bin
	arm-none-linux-gnueabi-objdump -D iCache.elf > iCache.dis
	gcc mkv210_image.c -o x210
	./x210 iCache.bin 210.bin
	
%.o : %.S
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis x210 -f

	
	
