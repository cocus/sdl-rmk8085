/*
 * console_impl.c
 *
 *  Created on: Feb 28, 2018
 *      Author: santiago
 */

#include <string.h>
//#include <unistd.h>
//#include <sys/select.h>
//#include <termios.h>

//struct termios orig_termios;

void console_end()
{
    //tcsetattr(0, TCSANOW, &orig_termios);
}

void console_start()
{

}

int kbhit()
{
    /*struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);*/
    return 0;
}
int patch_to_cr(int c)
{
	if (c == 10)
	{
		c = 13;
	}

	return c;
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return patch_to_cr(r);
    } else {
        return patch_to_cr(c);
    }
}
