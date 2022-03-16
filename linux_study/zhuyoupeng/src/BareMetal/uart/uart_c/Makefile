uart.bin: start.o led.o uart.o main.o
	arm-none-linux-gnueabi-ld -Ttext 0x0 -o uart.elf $^
	arm-none-linux-gnueabi-objcopy -O binary uart.elf uart.bin
	arm-none-linux-gnueabi-objdump -D uart.elf > uart.dis
	gcc mkv210_image.c -o x210
	./x210 uart.bin 210.bin
	
%.o : %.S
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis x210 -f

	
	
