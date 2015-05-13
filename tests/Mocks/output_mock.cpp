/*
 * Copyright (c) 2015 Stefan Petersen, Ciellt AB
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
#include <stdio.h>
#include <string.h>

#include "CppUTest/TestHarness.h"

extern "C" {
#include "output_mock.h"
}

static char stored_string[OUTPUT_MOCK_MAX_STRINGLENGTH];
static int string_index = 0;

void
output_mock_setup(void)
{
    memset(stored_string, 0, OUTPUT_MOCK_MAX_STRINGLENGTH);
    string_index = 0;
} // output_mock_init


void
output_mock_destroy(void)
{
} // output_mock_destroy


void
output_mock_char_input(char c)
{
    stored_string[string_index++] = c;
} // output_mock_char_input


char *
output_mock_get_string(void)
{
    return stored_string;
} // output_mock_get_string


int
output_mock_get_string_length(void)
{
    return string_index;
} // output_mock_get_stringlength
