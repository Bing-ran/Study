request_mem_region(start,n,name) 向内和申请需要映射的内存地址
	start:申请的起始地址(物理地址A)
	n	 :申请的内存的长度
	name :申请内存的名字
	返回：成功返回0
ioremap(start,n)		映射：传入物理地址返回虚拟地址
	start:映射的起始地址(物理地址A)
	n	 :申请的内存的长度
	返回：虚拟地址(虚拟地址B) 
release_mem_region(start,n)
iounmap(虚拟地址B)


流程：request_mem_region--->ioremap -->

		iounmap-->release_mem_region