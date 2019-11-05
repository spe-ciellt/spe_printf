/*
 * Copyright (c) 2015-2019 Stefan Petersen, Ciellt AB
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

#ifndef OUTPUT_MOCK_H
#define OUTPUT_MOCK_H

#define OUTPUT_MOCK_MAX_STRINGLENGTH 100

/**
 * Setup and destroy functions.
 */
void output_mock_setup(void);
void output_mock_destroy(void);

/**
 *  Takes a character as input and stores it.
 */
void output_mock_char_input(char c);

/**
 * Get the string that has been inputted one-by-one by output_mock_char_input()
 * Returns a pointer to the string with the data.
 */
char *output_mock_get_string(void);

/**
 * Get the length of string that has been inputted one-by-one by
 * output_mock_char_input()
 * Returns the number of characters.
 */
int output_mock_get_stringlength(void);

#endif /* OUTPUT_MOCK_H */
