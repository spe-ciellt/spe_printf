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

#ifndef SPE_PRINTF_H
#define SPE_PRINTF_H

/* For the variadic versions of the functions. */
#include <stdarg.h>

/*
 * Structure to keep track of the current file instantiation.
 * Don't modify directly, use accessor below.
 */
struct spe_fd {
    void (*putc)(char c);
};

/*
 * File descriptor used thru out spe_printf
 */
#define SPE_FILE struct spe_fd

/*
 * Register the print out one character callback function.
 * The function is defines as 'void putc(char c)'.
 */
#define SPE_PRINTF_SETUP(p) \
    { \
	.putc = p, \
    }

/*
 * Forward declaration of stdout and stderr. Needs to be defined
 * so spe_printf works.
 */
extern SPE_FILE *spe_stdout;
extern SPE_FILE *spe_stderr;


/*
 * The following conversion characters are supported:
 * '%': Plain %
 * 'c': Character
 * 's': String
 * 'd': Signed integer and long
 * 'u': Unsigned integer and long
 * 'l': long modifier, used with u and d
 * 'x': Hex, takes unsigned integer
 * 'b': Binary, takes unsigned integer
 * 'f': Double, floating point, if support is compiled in
 */

/*
 * spe_fprintf is the local version is fprintf.
 */
int spe_fprintf(SPE_FILE *fd, const char *fmt, ...);

/*
 * spe_printf is the local version is printf.
 * Note that spe_stderr must have been defined.
 */
int spe_printf(const char *fmt, ...);

/**
 * Variadic versions of the above functions.
 */
int spe_vfprintf(SPE_FILE *fd, const char *fmt, va_list ap);
int spe_vprintf(const char *fmt, va_list ap);

#endif /* SPE_PRINTF_H */
