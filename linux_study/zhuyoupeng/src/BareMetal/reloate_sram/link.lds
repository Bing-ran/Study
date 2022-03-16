SECTIONS
{
	. = 0xd0024000;

	.text : {
		reloate.o
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