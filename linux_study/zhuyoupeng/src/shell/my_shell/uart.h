/*
 * @Author: your name
 * @Date: 2021-07-18 21:51:05
 * @LastEditTime: 2021-07-18 21:59:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \my_shell\uart.h
 */
#ifndef __UART_H_
#define __UART_H_

void uart_init(void);
void puts(const char *str);
char *gets(char *p);
void memset(char *buf, char c, int len);

#endif