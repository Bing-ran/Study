lcd.bin: sdram_init.o led.o start.o lcd.o main.o
	arm-none-linux-gnueabi-ld -Tsdram_init.lds -o lcd.elf $^
	arm-none-linux-gnueabi-objcopy -O binary lcd.elf lcd.bin
	arm-none-linux-gnueabi-objdump -D lcd.elf > lcd.dis
	gcc mkv210_image.c -o x210
	./x210 lcd.bin 210.bin
	
%.o : %.S
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis x210 -f

	
	
