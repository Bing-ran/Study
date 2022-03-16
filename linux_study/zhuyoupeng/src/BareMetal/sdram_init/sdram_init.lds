SECTIONS
{
	. = 0x20000000;

	.text : {
		sdram.o
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