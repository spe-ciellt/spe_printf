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

#include <stdarg.h>

#include "spe_printf.h"

#ifdef USE_DOUBLE
#include <math.h>
#endif /* USE_DOUBLE */

const static char tohex[] = "0123456789abcdef";

static void
print_char(SPE_FILE *fd, char c)
{
    fd->putc(c);
} /* print_char */


static int
print_uil(SPE_FILE *fd, unsigned long number, int base,
	  int min_width, int precision, int neg)
{
    unsigned long divider = 1;
    int digit = 0;
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
	neg = 0;
	print_char(fd, '-');
    }

    /* Print out character by character by using the divider we just found. */
    /* This is the secret sauce to this no-buffering print routine. */
    while (1) {
	digit = number / divider;
	print_char(fd, tohex[digit]);
	number = number - (digit * divider);
	divider /= base;
	if (divider == 0L) {
	    break;
	}
    }

    return 0;
} /* print_uil */


static int
print_sil(SPE_FILE *fd, signed long number, int base,
	  int min_width, int precision)
{
    int neg = 0;

    if (number < 0L) {
	neg = 1;
	number = -number;
    }

    return print_uil(fd, (unsigned long)number, base, min_width, precision, neg);
} /* print_sil */


static int
print_ui(SPE_FILE *fd, unsigned int number, int base,
	 int min_width, int precision, int neg)
{
    unsigned int divider = 1;
    int digit = 0;
    int nuf_digits = 1;

    /* Find the biggest number dividable by base to use as starting
       point for dividing down character by character*/
    while ((number / divider) >= (unsigned int)base){
	divider *= base;
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
	neg = 0;
	print_char(fd, '-');
    }

    /* Print out character by character by using the divider we just found. */
    /* This is the secret sauce to this no-buffering print routine. */
    while (1) {
	digit = number / divider;
	print_char(fd, tohex[digit]);
	number = number - (digit * divider);
	divider /= base;
	if (divider == 0L) {
	    break;
	}
    }

    return 0;
} /* print_ui */


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


#ifdef USE_DOUBLE
#define DOUBLE_DEFAULT_PRECISION 6
static int
print_d(SPE_FILE *fd, double fp, int min_width, int precision)
{
    int ii, id;
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
    ii = (int)fp;
    tmp = fp  * pm - truncf(fp) * pm;
    id = (int)tmp;

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


static int
print_string(SPE_FILE *fd, char *string, int min_width, int precision)
{
    int i;

    for (i = 0; string[i] != 0; i++) {
	print_char(fd, string[i]);
    }

    return 0;
} /* print_string */


static int
conversion(SPE_FILE *fd, const char *fmt, int i, va_list *ap)
{
    int long_modifier = 0;
    int min_width = 0;
    int precision = 0;

    /* Read in eventual minimum width given as first paramter after % */
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
	    print_char(fd, va_arg(*ap, int));
	    return i;
	case 's': /* String */
	    print_string(fd, va_arg(*ap, char*), min_width, precision);
	    return i;
	case 'd': /* Signed integer and long */
	    if (long_modifier) {
		long_modifier = 0;
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
		long_modifier = 0;
		print_uil(fd, va_arg(*ap, unsigned long), 10,
			  min_width, precision, 0);
	    } else {
		print_ui(fd, va_arg(*ap, unsigned int), 10,
			 min_width, precision, 0);
	    }
	    return i;
	case 'x': /* Hex */
	    print_ui(fd, va_arg(*ap, unsigned int), 16, min_width, precision, 0);
	    return i;
	case 'b': /* Binary */
	    print_ui(fd, va_arg(*ap, unsigned int), 2, min_width, precision, 0);
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

static int
internal_fprintf(SPE_FILE *fd, const char *fmt, va_list *ap)
{
    int i;

    for (i = 0; fmt[i]; i++) {
	if (fmt[i] == '%') {
	    if ((i = conversion(fd, fmt, i, ap)) < 0) {
		return -1;
	    }
	} else {
	    print_char(fd, fmt[i]);
	}
    }

    return 0;
} /* internal_fprintf */


int
spe_fprintf(SPE_FILE *fd, const char *fmt, ...)
{
    va_list ap;
    int returned;

    va_start(ap, fmt);
    returned = internal_fprintf(fd, fmt, &ap);
    va_end(ap);

    return returned;
} /* spe_fprintf */


int
spe_printf(const char *fmt, ...)
{
    va_list ap;
    int returned;

    va_start(ap, fmt);
    returned = internal_fprintf(spe_stdout, fmt, &ap);
    va_end(ap);

    return returned;
} /* spe_fprintf */
