/*
 * @Author: your name
 * @Date: 2021-07-04 22:35:42
 * @LastEditTime: 2021-07-04 22:50:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \sd_relocate\BL1\sd_relocate.h
 */
#ifndef __SD_RELOCATE_H_
#define __SD_RELOCATE_H_

typedef unsigned int bool;
// 函数指针
/**
 * @description: 
 * @param {int}             通道号
 * @param {unsigned} int    开始扇区号 45
 * @param {unsigned} short  读取扇区个数 32
 * @param {unsigned} int    读取到放入内存地址：0x23E00000
 * @return {*}              0
 */
typedef bool (*pCopySDMMCto2Mem)(int, unsigned int, unsigned short, unsigned int*, bool);

#define CHANNEL_NUM 2
#define SD_START_BLOCK 45
#define SD_BLOCK_CNT   32
#define DDR_START_ADDR 0x23E00000

typedef void (*pBL2Type)(void);
#endif // !__SD_RELOCATE_H_


