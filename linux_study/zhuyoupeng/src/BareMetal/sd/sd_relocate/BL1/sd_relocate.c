/*
 * @Author: your name
 * @Date: 2021-07-04 22:35:30
 * @LastEditTime: 2021-07-04 23:12:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \sd_relocate\BL1\sd_relocate.c
 */
#include "sd_relocate.h"

/**
 * @description: 重定位：从SD卡第45扇区开始，赋值32个扇区内容到DDR的0x23E00000
 * @param {*}
 * @return {*}
 */
void copy_bl2_to_ddr(void)
{
    // 第一步，读取SD卡扇区到DDR中
    // 注意此处的p1定义：0xD0037F98 这个地址存的是函数的入口地址，所以先要解引用，然后在强制转换为目标函数类型
    pCopySDMMCto2Mem p1 = (pCopySDMMCto2Mem)(*(unsigned int *)0xD0037F98);

    p1(CHANNEL_NUM, SD_START_BLOCK, SD_BLOCK_CNT, (unsigned int *)DDR_START_ADDR, 0);   // 实际读取SD卡到DDR中

    // 第二步，跳转到DDR中的BL2去执行
    pBL2Type p2 = (pBL2Type)DDR_START_ADDR;
    p2();
}