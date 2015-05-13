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

#include <stdint.h>
#include <stdio.h>
#include "spe_printf.h"


static void
myputc(char c)
{
    putchar(c);
} /* myputc */

SPE_FILE output = SPE_PRINTF_SETUP(myputc);
SPE_FILE *spe_stdout = &output;

void
testcase_negative(void)
{
    int r;

    printf("Negative integers:\n");

    /* ## */
    r = spe_printf(" (n)Negative int:[%4.4d] and [%5d] and [%5.4d]\n",
		   -12, -123, -234);
    if (r < 0) {
	fprintf(stderr, "Failed sentence\n");
    }
    printf(" (o)Negative int:[%4.4d] and [%5d] and [%5.4d]\n", -12, -123, -234);

    /* ## */
    r = spe_printf(" (n)Negative int:[%4.3d] and [%4.2d] and [%4.1d]\n",
		   -12, -12, -12);
    if (r < 0) {
	fprintf(stderr, "Failed sentence\n");
    }
    printf(" (o)Negative int:[%4.3d] and [%4.2d] and [%4.1d]\n", -12, -12, -12);

    /* ## */
    r = spe_printf(" (n)Negative int:[%7.5d] and [%7.3d] and [%1d]\n",
		   -12, -12, -12);
    if (r < 0) {
	fprintf(stderr, "Failed sentence\n");
    }
    printf(" (o)Negative int:[%7.5d] and [%7.3d] and [%1d]\n", -12, -12, -12);

    /* ## */
    r = spe_printf(" (n)Negative long:[%7.5ld] and [%14.12ld] and [%14ld]\n",
		   -1234567890L, -1234567890L, -1234567890L);
    if (r < 0) {
	fprintf(stderr, "Failed sentence\n");
    }
    printf(" (o)Negative long:[%7.5ld] and [%14.12ld] and [%14ld]\n",
	   -1234567890L, -1234567890L, -1234567890L);

} /* testcase_negative */

static void
testcase_double(void)
{
    int r;

    r = spe_printf("Pos. double %f and negative double %f\n",
                  12.34, -43.21);
    if (r < 0) {
       fprintf(stderr, "Failed sentence\n");
    }
    printf("Pos. double %f and negative double %f\n", 12.34, -43.21);

    r = spe_printf("Small decimal double %f and negative %7.2f\n", 
                  12.0034, -43.21);
    if (r < 0) {
       fprintf(stderr, "Failed sentence\n");
    }
    printf("Small decimal double %f and negative %7.2f\n", 12.0034, -43.21);

} /* testcase_double */


int
main(int argc, char *argv[])
{
    char msg[] = "Hello World";
    long lv = 1231232312L;
    uint8_t other = 123;
    unsigned long slv = 5467L;
    int r;

    r = spe_printf("Percent:%% string:%s \n", msg);
    if (r < 0) {
	fprintf(stderr, "Failed sentence\n");
    }

    r = spe_printf("+unsigned int(6.4):[%6.4u] and long:%ld\n", other, lv);
    if (r < 0) {
	fprintf(stderr, "Failed sentence\n");
    }
    printf("-unsigned int(6.4):[%6.4u] and long:%ld\n", other, lv);

    r = spe_printf("+Int(6.6):%6.6d and unsigned long(6.5):%6.5lu\n", 321, slv);
    if (r < 0) {
	fprintf(stderr, "Failed sentence\n");
    }
    printf("-Int(6.6):%6.6d and unsigned long(6.5):%6.5lu\n", 321, slv);

    r = spe_printf("Int:%d should be 105 and %d should be 1005\n", 105, 1005);
    if (r < 0) {
	fprintf(stderr, "Failed sentence\n");
    }

    testcase_negative();

    testcase_double();

    r = spe_printf("Hex:0x%x should be 0xabc12def\n", 0xabc12def);
    if (r < 0) {
	fprintf(stderr, "Failed sentence\n");
    }

    r = spe_printf("Binary %16.16b\n", 0x0f0f);
    if (r < 0) {
	fprintf(stderr, "Failed sentence\n");
    }

    r = spe_printf("Character %c\n", 'c');
    if (r < 0) {
	fprintf(stderr, "Failed sentence\n");
    }

    return 0;
} /* main */
