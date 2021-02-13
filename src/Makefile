#
# Copyright (c) 2013-2019 Stefan Petersen, Ciellt AB
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

CC=gcc
CFLAGS=-Wall -Wextra -DUSE_DOUBLE -std=c99

CPPCHECK_TESTS = "--enable=warning,style,performance,portability"

all: spe_printf-example

spe_printf-example: spe_printf-example.o spe_printf.o

# Generate Doxygen documentation
doxygen:
	@doxygen ./Doxyfile

# Run cppcheck
cppcheck:
	@cppcheck --quiet $(CPPCHECK_TESTS) --std=c99 --platform=unix32 .
	@cppcheck --quiet -DUSE_DOUBLE $(CPPCHECK_TESTS) --std=c99 --platform=unix32 .

clean:
	rm -rf *~ *.o docs spe_printf-example
