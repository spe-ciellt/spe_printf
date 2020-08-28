/*
 * Copyright (c) 2013-2020 Stefan Petersen, Ciellt AB
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

/*! \mainpage
 * \section intro A simple printf library
 *
 * This library implements the printf function usually found in different
 * libc libraries. The goal of the implementation is to be as complete as
 * possible, but at the same time as small as possible and not use any
 * static nor dynamic buffers.
 *
 * It is implemented with the notion of file descriptors. Since no state is
 * kept between calls, the functions are considered reentrant. That is valid
 * as long as the callback to output character by character is reentrant
 * (or protected by mutexes).
 *
 * There is a need for startup initialization per file descriptor to
 * register a callback function to output one single character at a time.
 * Each file descriptor need its own initalization. That makes it possible to
 * use file descriptor as a way to print out text at separate outputs, may
 * it be a serial port, LCD or similar.
 *
 * \section conversion_tags Conversion tags
 * Conversion tags are the character(s) after %.
 *
 * \subsection conversion_tags_supported Supported
 *
 * \li %: prints out a plain percent character.
 * \li c: prints out a char variable.
 * \li s: prints out a zero-terminated string pointed to by a variable.
 * \li d: prints out a signed integer variable in decimal format.
 * \li u: prints out an unsigned integer variable in decimal format.
 * \li x: prints out an unsigned integer variable in hexadecimal format.
 * \li f: prints out floating point number, if compiled in. Compile with
 * ``CFLAGS += -DUSE_DOUBLE`` as argument to compiler.
 *
 * \subsection conversion_tags_optional Optional
 *
 * The `l` modifier can be used with signed, unsigned and hexadecimal
 * conversion tags to print out `long` variables (`%%lu`, `%%ld` and `%%lx`).
 *
 * \section printf_variants Variants of the printf routines
 *
 * There are two groups of printf functions, each with one function that
 * takes a file descriptor and one that uses stdout.
 * \li The general group contains spe_printf() and spe_fprintf()
 * \li The variadic group contains spe_vprintf() and spe_vfprintf()
 * (see https://en.wikipedia.org/wiki/Variadic_function)
 *
 * \section supported What is supported and what is not supported
 *
 * To understand what *minimal width* and *precision* are, see: \n
 * \b "The C Programming Language, Second Edition", Brian W. Kernighan &
 * Dennis M. Ritchie, pp.153-155. \n
 *
 * \subsection supported_supported Supported
 * \li All conversion tags above.
 * \li Minimal width and optional precision for all numerical types.
 * \li Reentrance (of course if callback is reentrant).
 *
 * \subsection supported_unsupported Unsupported
 * \li minimal width and optional precision for strings.
 * \li negative minimal width (left adjustment).
 * \li minimal width as a parameter to the conversion string.
 */

/**
 * \file
 */
#include <stdarg.h>

#include "spe_printf.h"

#ifdef USE_DOUBLE
#include <math.h>
#endif /* USE_DOUBLE */

static const char tohex[] = "0123456789abcdef";

static void
print_char(SPE_FILE *fd, const char c)
{
    if (fd->putc) {
        fd->putc(c);
    }
    if (fd->str) {
        if (fd->curr < (fd->max - 1)) {
            fd->str[fd->curr++] = c;
        }
    }
} /* print_char */

/**
 * \b print_uil
 *
 * This is an internal function not for use by application code.
 *
 * Print unsigned integer long to fd.
 *
 * @param fd Pointer to filedescriptor to output result to.
 * @param number The actual number to print out.
 * @param base Base to print out number in. Most comman are 2, 10 and 16.
 * @param min_width Minimum field width.
 * @param precision Minimum number of digits to represent the integer.
 * @param neg Non-zero if a minus sign should be added. Determined by the
              conversion parser.
 * @retval 0 on success.
 * @retval -1 on failure.
 */
static int
print_uil(SPE_FILE *fd, unsigned long number, const int base,
          int min_width, const int precision, int neg)
{
    unsigned long divider = 1UL;
    int nuf_digits = 1;

    /* Find the biggest number dividable by base to use as starting
       point for dividing down character by character*/
    while ((number / divider) >= (unsigned long)base) {
        divider *= (unsigned long)base;
        nuf_digits++;
        if (divider == 0L) {
            return -1;
        }
    }

    /* We must make room for minus sign */
    if (neg && (min_width > precision)) {
        min_width -= 1;
    }

    /* Do the eventual fill out coming from minimal width and precision.
       Before printing out any noticiable character, print out eventual minus.*/
    while (nuf_digits < min_width) {
        if (min_width <= precision) {
            if (neg) {
                neg = 0;
                print_char(fd, '-');
            }
            print_char(fd, '0');
        } else {
            print_char(fd, ' ');
        }
        min_width--;
    }

    /* If we didn't print out any minus, do it now. */
    if (neg) {
        print_char(fd, '-');
    }

    /* Print out character by character by using the divider we just found. */
    /* This is the secret sauce to this no-buffering print routine. */
    while (1) {
        unsigned long digit = number / divider;
        print_char(fd, tohex[digit]);
        number = number - (digit * divider);
        divider /= (unsigned long)base;
        if (divider == 0L) {
            break;
        }
    }

    return 0;
} /* print_uil */

/**
 * \b print_sil
 *
 * This is an internal function not for use by application code.
 *
 * Print signed integer long to fd.
 *
 * @param fd Pointer to filedescriptor to output result to.
 * @param number The actual number to print out.
 * @param base Base t print out number in. Most comman are 2, 10 and 16.
 * @param min_width Minimum field width.
 * @param precision Minimum number of digits to represent the integer.
 * @param neg Non-zero if a minus sign should be added. Determined by the
              conversion parser.
 * @retval 0 on success.
 * @retval -1 on failure.
 */
static int
print_sil(SPE_FILE *fd, signed long number, const int base,
          const int min_width, const int precision)
{
    int neg = 0;

    if (number < 0L) {
        neg = 1;
        number = -number;
    }

    return print_uil(fd, (unsigned long)number, base, min_width, precision, neg);
} /* print_sil */


/**
 * \b print_ui
 *
 * This is an internal function not for use by application code.
 *
 * Print unsigned integer to fd.
 *
 * @param fd Pointer to filedescriptor to output result to.
 * @param number The actual number to print out.
 * @param base Base t print out number in. Most comman are 2, 10 and 16.
 * @param min_width Minimum field width.
 * @param precision Minimum number of digits to represent the integer.
 * @param neg Non-zero if a minus sign should be added. Determined by the
              conversion parser.
 * @retval 0 on success.
 * @retval -1 on failure.
 */
static int
print_ui(SPE_FILE *fd, unsigned int number, int base,
         int min_width, int precision, int neg)
{
    unsigned long divider = 1UL;
    int nuf_digits = 1;

    /* Find the biggest number dividable by base to use as starting
       point for dividing down character by character*/
    while ((number / divider) >= (unsigned int)base){
        divider *= (unsigned long)base;
        nuf_digits++;
        if (divider == 0L) {
            return -1;
        }
    }

    /* We must make room for minus sign */
    if (neg && (min_width > precision)) {
        min_width -= 1;
    }

    /* Do the eventual fill out coming from minimal width and precision.
       Before printing out any noticiable character, print out eventual minus.*/
    while (nuf_digits < min_width) {
        if (min_width <= precision) {
            if (neg) {
                neg = 0;
                print_char(fd, '-');
            }
            print_char(fd, '0');
        } else {
            print_char(fd, ' ');
        }
        min_width--;
    }

    /* If we didn't print out any minus, do it now. */
    if (neg) {
        print_char(fd, '-');
    }

    /* Print out character by character by using the divider we just found. */
    /* This is the secret sauce to this no-buffering print routine. */
    while (1) {
        unsigned long digit = number / divider;
        print_char(fd, tohex[digit]);
        number = number - (unsigned int)(digit * divider);
        divider /= (unsigned long)base;
        if (divider == 0L) {
            break;
        }
    }

    return 0;
} /* print_ui */

/**
 * \b print_si
 *
 * This is an internal function not for use by application code.
 *
 * Print signed integer to fd.
 *
 * @param fd Pointer to filedescriptor to output result to.
 * @param number The actual number to print out.
 * @param base Base t print out number in. Most comman are 2, 10 and 16.
 * @param min_width Minimum field width.
 * @param precision Minimum number of digits to represent the integer.
 * @param neg Non-zero if a minus sign should be added. Determined by the
              conversion parser.
 *
 * @retval 0 on success.
 * @retval -1 on failure.
 */
static int
print_si(SPE_FILE *fd, signed int number, int base, int min_width, int precision)
{
    int neg = 0;

    if (number < 0) {
        neg = 1;
        number = -number;
    }

    return print_ui(fd, (unsigned int)number, base, min_width, precision, neg);
} /* print_si */


/**
 * \b print_d
 *
 * This is an internal function not for use by application code.
 * Only included if USE_DOUBLE is defined.
 *
 * Print doubles to fd.
 *
 * @param fd Pointer to filedescriptor to output result to.
 * @param fp The actual number to print out.
 * @param min_width Minimum field width.
 * @param precision Minimum number of digits to represent the integer.
 *
 * @retval 0 on success.
 * @retval -1 on failure.
 */
#ifdef USE_DOUBLE
#define DOUBLE_DEFAULT_PRECISION 6
static int
print_d(SPE_FILE *fd, double fp, int min_width, int precision)
{
    unsigned int ii, id;
    int n;
    int neg = 0;
    double pm, tmp;

    /* Doubles must be calculated as a positive number to get proper
       integer and decimal parts */
    if (fp < 0.0) {
        neg = 1;
        fp = -fp;
    }

    /* If no precision given, revert to default */
    if (precision == 0) {
        precision = DOUBLE_DEFAULT_PRECISION;
    }

    /* When we print out the decimal, we use the integer print function.
     * To fill out the zeros in the end of the number we must extend
     * the decimal by some 10's exponents. */
    for (pm = 1.0, n = 0; n != precision; n++, pm *= 10.0);

    /* Split double into integer integer and integer decimal */
    ii = (unsigned int)fp;
    tmp = fp  * pm - trunc(fp) * pm;
    id = (unsigned int)tmp;

    /* Punctuation is included in the min_width */
    min_width -= 1;

    /* min_width is including all numbers, also decimal */
    min_width -= precision;
    if (min_width < 0) {
        min_width = 0;
    }

    /* Print it as the integer part, a dot and the decimal part */
    print_ui(fd, ii, 10, min_width, 0, neg);
    print_char(fd, '.');
    print_ui(fd, id, 10, precision, precision, 0);

    return 0;
} /* printf_d */
#endif /* USE_DOUBLE */


/**
 * \b print_string
 *
 * This is an internal function not for use by application code.
 *
 * Print string to fd one character at a time.
 *
 * @param fd Pointer to filedescriptor to output result to.
 * @param string The actual string to print out.
 *
 * @retval 0 on success.
 * @retval -1 on failure.
 */
static int
print_string(SPE_FILE *fd, const char *string)
{
    int i;

    for (i = 0; string[i] != 0; i++) {
        print_char(fd, string[i]);
    }

    return 0;
} /* print_string */


/**
 * \b conversion
 *
 * This is an internal function not for use by application code.
 *
 * Conversion
 * Format string resolver.
 *
 * @param fd Pointer to filedescriptor to output result to.
 * @param fmt The format string to use when formatting output.
 * @param i Index in fmt string we're trying to resolve.
 * @param ap Variable argument list to the print command
 *
 * @retval 0 on success.
 * @retval -1 on failure.
 */
static int
conversion(SPE_FILE *fd, const char *fmt, int i, const va_list *ap)
{
    int long_modifier = 0;
    int min_width = 0;
    int precision = 0;

    /* Read in eventual minimum width given as first parameter after % */
    while ((fmt[i + 1] >= '0') && (fmt[i + 1] <= '9')) {
        min_width *= 10;
        min_width += (fmt[i + 1] - '0');
        i++;
    }

    while (1) {
        i++;
        switch (fmt[i]) {
        case '%': /* Plain % */
            print_char(fd, '%');
            return i;
            break;
        case 'c': /* Character */
            print_char(fd, (char)va_arg(*ap, int));
            return i;
        case 's': /* String */
            print_string(fd, va_arg(*ap, char*));
            return i;
        case 'd': /* Signed integer and long */
            if (long_modifier) {
                print_sil(fd, va_arg(*ap, long), 10, min_width, precision);
            } else {
                print_si(fd, va_arg(*ap, int), 10, min_width, precision);
            }
            return i;
        case 'l': /* long modifier, used with u and d */
            long_modifier = 1;
            break;
        case 'u': /* Unsigned integer and long */
            if (long_modifier) {
                print_uil(fd, va_arg(*ap, unsigned long), 10,
                          min_width, precision, 0);
            } else {
                print_ui(fd, va_arg(*ap, unsigned int), 10,
                         min_width, precision, 0);
            }
            return i;
        case 'x': /* Hex */
            if (long_modifier) {
                print_uil(fd, va_arg(*ap, unsigned long), 16, min_width,
                          precision, 0);
            } else {
                print_ui(fd, va_arg(*ap, unsigned int), 16, min_width,
                         precision, 0);
            }
            return i;
#ifdef USE_DOUBLE
        case 'f':
            print_d(fd, va_arg(*ap, double), min_width, precision);
            return i;
#else
            return -1;
#endif /* USE_DOUBLE */
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            precision *= 10;
            precision += (fmt[i] - '0');
            break;
        case '.':
            break;
        default:
            return -1;
        }
    }

    return 0;
} /* conversion */


/**@name General versions */
/**@{*/
/**
 * \b spe_fprintf
 *
 * Refer to fprintf() in libc.
 *
 * @param fd A pointer to the file descriptor.
 * @param fmt Format string for formatting the text.
 * @param ... A list of parameters to be displayed.
 *
 * @retval 0 On success.
 * @retval -1 On failure.
 */
int
spe_fprintf(SPE_FILE *fd, const char *fmt, ...)
{
    va_list ap;
    int returned;

    va_start(ap, fmt);
    returned = spe_vfprintf(fd, fmt, ap);
    va_end(ap);

    return returned;
} /* spe_fprintf */


/**
 * \b spe_printf
 *
 * Refer to printf() in libc.
 * Note that spe_stdout must have been defined.
 *
 * @param fmt Format string for formatting the text.
 * @param ... A list of parameters to be displayed.
 *
 * @retval 0 On success.
 * @retval -1 On failure.
 */
int
spe_printf(const char *fmt, ...)
{
    va_list ap;
    int returned;

    va_start(ap, fmt);
    returned = spe_vfprintf(spe_stdout, fmt, ap);
    va_end(ap);

    return returned;
} /* spe_printf */


/**
 * \b spe_snprintf
 *
 * Refer to snprintf() in libc.
 * Prints text to a string.
 *
 * @param str Pointer to string to be written to.
 * @param size Maximum number of characters to be written to the string,
 *          including terminating \0.
 * @param fmt Format string for formatting the text.
 * @param ... A list of parameters to be displayed.
 *
 * @retval >=0 Number of characters written, including terminating \0.
 * @retval -1 On failure.
 */
int
spe_snprintf(char *str, const size_t size, const char *fmt, ...)
{
    va_list ap;
    int returned;

    va_start(ap, fmt);
    returned = spe_vsnprintf(str, size, fmt, ap);
    va_end(ap);

    return returned;
} /* spe_snprintf */

/**@}*/


/**@name Variadic versions */
/**@{*/
/**
 * \b spe_vfprintf
 *
 * Refer to vfprintf() in libc.
 *
 * @param fd A pointer to the file descriptor.
 * @param fmt Format string for formatting the text.
 * @param ap A list of parameters in va_list format.
 *
 * @retval 0 On success.
 * @retval -1 On failure.
 */
int
spe_vfprintf(SPE_FILE *fd, const char *fmt, va_list ap)
{
    int ret = 0;
    /**
     * Problems when compiling on a X86/64 which is described here:
     * Solution is to use a copy, which seems to solve the issue on both
     * Cortex-M and X86/64.
     * https://stackoverflow.com/questions/8047362/is-gcc-mishandling-a-pointer-to-a-va-list-passed-to-a-function
     */
    va_list ap_copy;
    va_copy(ap_copy, ap);

    for (int i = 0; fmt[i]; i++) {
        if (fmt[i] == '%') {
            if ((i = conversion(fd, fmt, i, &ap_copy)) < 0) {
                ret = -1;
                break;
            }
        } else {
            print_char(fd, fmt[i]);
        }
    }
    va_end(ap_copy);
    return ret;
} /* spe_vfprintf */


/**
 * \b spe_vprintf
 *
 * Refer to vprintf() in libc.
 * Note that spe_stdout must have been defined. \n
 *
 * @param fmt Format string for formatting the text.
 * @param ap A list of parameters in va_list format.
 *
 * @retval 0 On success.
 * @retval -1 On failure.
 */
int
spe_vprintf(const char *fmt, va_list ap)
{
    return spe_vfprintf(spe_stdout, fmt, ap);
} /* spe_vprintf */


/**
 * \b spe_vsnprintf
 *
 * Refer to vsnprintf() in libc.
 * Prints text to a string.
 *
 * @param str Pointer to string to be written to.
 * @param size Maximum number of characters to be written to the string,
 *          including terminating \0.
 * @param fmt Format string for formatting the text.
 * @param ap A list of parameters in va_list format.
 *
 * @retval >=0 Number of characters written, including terminating \0.
 * @retval -1 On failure.
 */

int
spe_vsnprintf(char *str, const size_t size, const char *fmt, va_list ap)
{
    SPE_FILE strfd = {
        .putc = NULL,
        .str = str,
        .max = size,
        .curr = 0,
    };

    if (spe_vfprintf(&strfd, fmt, ap) < 0) {
        return -1;
    }
    strfd.str[strfd.curr++] = '\0';

    return (int)strfd.curr;
} /* spe_vsnprintf */

/**@}*/
