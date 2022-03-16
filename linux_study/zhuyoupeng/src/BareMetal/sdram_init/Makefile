sdram.bin: sdram_init.o led.o sdram.o
	arm-none-linux-gnueabi-ld -Tsdram_init.lds -o sdram.elf $^
	arm-none-linux-gnueabi-objcopy -O binary sdram.elf sdram.bin
	arm-none-linux-gnueabi-objdump -D sdram.elf > sdram.dis
	gcc mkv210_image.c -o x210
	./x210 sdram.bin 210.bin
	
%.o : %.S
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis x210 -f

	
	
