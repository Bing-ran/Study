bootloader1.bin: sdram_init.o start.o sd_relocate.o
	arm-none-linux-gnueabi-ld -Tlink.lds -o bootloader1.elf $^
	arm-none-linux-gnueabi-objcopy -O binary bootloader1.elf bootloader1.bin
	arm-none-linux-gnueabi-objdump -D bootloader1.elf > bootloader1.dis
	gcc mkv210_image.c -o x210
	./x210 bootloader1.bin BL1.bin
	
%.o : %.S
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

%.o : %.c
	arm-none-linux-gnueabi-gcc -o $@ $< -c -nostdlib

clean:
	rm *.o *.elf *.bin *.dis x210 -f

	
	
