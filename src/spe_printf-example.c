/*
 * Copyright (c) 2013-2015 Stefan Petersen, Ciellt AB
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "spe_printf.h"

/**
 * \file
 *
 * Sample implementation using spe_printf() routines.
 */

/**
 * Local version of output one character.
 *
 * @param c Character to print out.
 */
static void
myputc(char c)
{
    putchar(c);
} /* myputc */

/* Register myputc() in a file descriptor as a callback for spe_printf() */
SPE_FILE output = SPE_PRINTF_SETUP(myputc);

/* Define the declared filedescriptor as stdout. */
SPE_FILE *spe_stdout = &output;

/**
 * Prints out some text using spe_printf()
 */
int
main(int argc, char *argv[])
{
    char msg[] = "Hello World";
    long lv = 1231232312L;
    uint8_t other = 123;
    int r;

    /* Fix stupid compilation warning */
    argc = argc;
    argv = argv;

    r = spe_printf("Percent:%% string:%s \n", msg);
    assert(r == 0);

    r = spe_printf("unsigned int(6.4):[%6.4u] and long:%ld\n", other, lv);
    assert(r == 0);

    return 0;
} /* main */
