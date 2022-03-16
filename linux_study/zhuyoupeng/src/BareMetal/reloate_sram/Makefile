reloate.bin: reloate.o led.o
	arm-none-linux-gnueabi-ld -Tlink.lds -o reloate.elf $^
	arm-none-linux-gnueabi-objcopy -O binary reloate.elf reloate.bin
	arm-none-linux-gnueabi-objdump -D reloate.elf > reloate.dis
	gcc mkv210_image.c -o x210
	./x210 reloate.bin 210.bin
	
%.o : %.S
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis x210 -f

	
	
