#ifndef __SD_H_
#define __SD_H_

typedef unsigned int bool;
// 宏定义
#define CopySDMMCtoMem(z,a,b,c,e)(((bool(*)(int, unsigned int, unsigned short, unsigned int*, bool))(*((unsigned
int *)0xD0037F98;)))(z,a,b,c,e))

// 函数指针
typedef bool (*pCopySDMMCto2Mem)(int, unsigned int, unsigned short, unsigned int*, bool);


pCopySDMMCto2Mem p1 = (pCopySDMMCto2Mem)0xD0037F98;

p1(x, x, x, x, x);
(*p1)(x, x, x, x, x);
#endif