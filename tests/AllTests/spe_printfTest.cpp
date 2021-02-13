/*
 * Copyright (c) 2013-2019 Stefan Petersen, Ciellt AB
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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "CppUTest/TestHarness.h"

extern "C" {
#include "spe_printf.h"
#include "output_mock.h"
}

SPE_FILE output = SPE_PRINTF_SETUP(output_mock_char_input);
SPE_FILE *spe_stdout = &output;

char *ref_string;
const static int ref_string_length = 60;

TEST_GROUP(spe_printf)
{
    void setup() {
        output_mock_setup();
        ref_string = (char *)malloc(ref_string_length);
        CHECK(ref_string);
        memset(ref_string, 0, ref_string_length);
    }
    void teardown() {
        free(ref_string);
        ref_string = NULL;
        output_mock_destroy();
    }
};

static void
do_comparison(const char *fmt, ...)
{
    va_list ap, ap1;

    va_start(ap, fmt);
    va_copy(ap1, ap);
    LONGS_EQUAL(0, spe_vprintf(fmt, ap));
    vsnprintf(ref_string, ref_string_length, fmt, ap1);
    va_end(ap);
    va_end(ap1);
    STRCMP_EQUAL(ref_string, output_mock_get_string());
} // do_comparison


/* Do test with spe_printf first, then we can use do_comparsion() */
TEST(spe_printf, InitalTest)
{
    char str[] = "World";
    LONGS_EQUAL(0, spe_printf("Hello %s %s %%", str, str));
    snprintf(ref_string, ref_string_length, "Hello %s %s %%", str, str);
    STRCMP_EQUAL(ref_string, output_mock_get_string());
}

TEST(spe_printf, HexIntegersLowerCase)
{
    LONGS_EQUAL(0, spe_printf("0x%x", 0xabc12def));
    STRCMP_EQUAL("0xabc12def", output_mock_get_string());
}

TEST(spe_printf, HexIntegersUpperCase)
{
    LONGS_EQUAL(0, spe_printf("0x%X", 0xabc12def));
    STRCMP_EQUAL("0xABC12DEF", output_mock_get_string());
}

/* Not supported by libc printf */
TEST(spe_printf, NewTest)
{
    LONGS_EQUAL(0, spe_printf("k%u,%u", 1,2));
    STRCMP_EQUAL("k1,2", output_mock_get_string());
}

TEST(spe_printf, Characters)
{
    do_comparison("Character %c\n", 'c');
}

TEST(spe_printf, NormalInteger)
{
    do_comparison("%d %d", 105, 1005);
}

TEST(spe_printf, IntegerWithFormat)
{
    uint8_t test = 123;
    do_comparison("[%6.4u] [%6.6d]\n", test, test);
}

TEST(spe_printf, LongWithFormat)
{
    unsigned long lv = 1231232312L;
    do_comparison(" [%ld] [%6.5lu]\n", lv, lv);
}

TEST(spe_printf, NegativeIntegersWithFormat)
{
    do_comparison("[%4.4d] and [%5d] and [%5.4d]", -12, -123, -234);
}

TEST(spe_printf, NegativeIntegersWithLeadingZeroes)
{
    do_comparison("[%4.3d] and [%4.2d] and [%4.1d]", -12, -12, -12);
}

TEST(spe_printf, NegativeIntegersWithManyLeadingZeroes)
{
    do_comparison("[%7.5d] and [%7.3d] and [%1d]", -12, -12, -12);
}

TEST(spe_printf, NegativeLongWithFormat)
{
    do_comparison("[%7.5ld] and [%14.12ld] and [%14ld]\n",
                  -1234567890L, -1234567890L, -1234567890L);
}

TEST(spe_printf, PositivAndNegativeDouble)
{
    do_comparison("[%f] and [%f]", 12.34, -43.21);
}

TEST(spe_printf, SmallDecimalDouble)
{
    do_comparison("[%f] and [%7.2f]", 12.0034, -43.21);
}

TEST(spe_printf, BigDecimalDouble)
{
    do_comparison("[%f] and [%7.2f]", 12.9999, -43.98);
}

TEST(spe_printf, SeveralCharacters)
{
    do_comparison("%u,%u", 1, 2);
}

TEST(spe_printf, snprintfFirstTest)
{
    char string[13];
    LONGS_EQUAL(13, spe_snprintf(string, 13, "Hello World!"));
    STRCMP_EQUAL("Hello World!", string);
}

TEST(spe_printf, snprintfTooShort)
{
    char string[10];
    LONGS_EQUAL(10, spe_snprintf(string, 10, "Hello World!"));
    STRCMP_EQUAL("Hello Wor", string);
}

TEST(spe_printf, snprintfTooLong)
{
    char string[15];
    LONGS_EQUAL(13, spe_snprintf(string, 15, "Hello World!"));
    STRCMP_EQUAL("Hello World!", string);
}

TEST(spe_printf, snprintfTooShortWithdata)
{
    char string[15];
    LONGS_EQUAL(15, spe_snprintf(string, 15, "Hello World!%d", 1234));
    STRCMP_EQUAL("Hello World!12", string);
}
