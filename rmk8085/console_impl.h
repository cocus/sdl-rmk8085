/*
 * console_impl.h
 *
 *  Created on: Feb 28, 2018
 *      Author: santiago
 */

#ifndef CONSOLE_IMPL_H_
#define CONSOLE_IMPL_H_

#include <stdio.h>

void console_start();

int kbhit();
int getch();

void console_end();

#endif /* CONSOLE_IMPL_H_ */
