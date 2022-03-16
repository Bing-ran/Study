SECTIONS
{
	. = 0x0;

	.text : {
		start.o
		sdram_init.o
		* (.text)
	}

	.data : {
		* (.data)
	}

	bss_start = .;
	.bss : {
		* (.bss)
	}

	bss_end = .;
}