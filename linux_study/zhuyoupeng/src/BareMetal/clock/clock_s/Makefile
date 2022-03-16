clock.bin: start.o led.o clock.o
	arm-none-linux-gnueabi-ld -Ttext 0x0 -o clock.elf $^
	arm-none-linux-gnueabi-objcopy -O binary clock.elf clock.bin
	arm-none-linux-gnueabi-objdump -D clock.elf > clock.dis
	gcc mkv210_image.c -o x210
	./x210 clock.bin 210.bin
	
%.o : %.S
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis x210 -f

	
	
