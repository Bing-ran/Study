watchdog.bin: watchdog.o 
	arm-none-linux-gnueabi-ld -Ttext 0x0 -o watchdog.elf $^
	arm-none-linux-gnueabi-objcopy -O binary watchdog.elf watchdog.bin
	arm-none-linux-gnueabi-objdump -D watchdog.elf > watchdog.dis
	gcc mkv210_image.c -o x210
	./x210 watchdog.bin 210.bin
	
%.o : %.S
	arm-none-linux-gnueabi-gcc -o $@ $< -c

%.o : %.c
	arm-none-linux-gnueabi-gcc -o $@ $< -c 

clean:
	rm *.o *.elf *.bin *.dis x210 -f

	
	
