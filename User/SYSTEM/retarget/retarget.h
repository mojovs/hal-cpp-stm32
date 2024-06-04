//
// Created by meng on 2024/6/4.
//

#ifndef TEMPLATE_CPP_RETARGET_H
#define TEMPLATE_CPP_RETARGET_H
#ifndef _RETARGET_H__
#define _RETARGET_H__
#include "sys.h"
#include <sys/stat.h>
#include <stdio.h>

void RetargetInit(UART_HandleTypeDef *huart);

int _isatty(int fd);

int _write(int fd, char *ptr, int len);

int _close(int fd);

int _lseek(int fd, int ptr, int dir);

int _read(int fd, char *ptr, int len);

int _fstat(int fd, struct stat *st);

#endif //#ifndef _RETARGET_H__


#endif //TEMPLATE_CPP_RETARGET_H