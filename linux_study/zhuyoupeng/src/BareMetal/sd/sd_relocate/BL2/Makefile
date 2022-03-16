BL2.bin: start.o led.o
	arm-none-linux-gnueabi-ld -Tlink.lds -o BL2.elf $^
	arm-none-linux-gnueabi-objcopy -O binary BL2.elf BL2.bin
	arm-none-linux-gnueabi-objdump -D BL2.elf > BL2.dis
	
%.o : %.S
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis x210 -f

	
	
