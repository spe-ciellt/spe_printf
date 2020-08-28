spe printf
===

A printf for small micro controllers, mainly Cortex M3.

Supported conversion tags
==
It supports:
* minimal width and optional precision for all numerical types.
* long modifier for all numerical conversion tags.
* plain percent (%), character (c), string (s), signed integer, 
  unsigned integer(u) and hex (x).
* double if compiled in, see below.
* Includes variadic versions of all printf() functions.
* Includes snprintf()/vsnprintf() versions to print to strings.

It does not support:
* minimal width and optional precision for strings.
* negative minimal width.
* minimal width as a parameter to the conversion string (*)

double()
==
It can print doubles too. Then the variable USE_DOUBLE needs to be set
when compiling, by for instance adding *CFLAGS+=-DUSE_DOUBLE* in the Makefile.
When including usage of double, math.h and math lib is necessary. Using just
casting to change one part (integer or decimal) of the double to an int lost
too much precision. So by using truncf() instead I managed to get better
precision, especially in the decimal part.

Documentation
==
This library is documented using the [Doxygen](http://www.doxygen.org/) format.

In the src directory, execute `make doxygen`. This will generate a
subdirectory in the src directory called docs. By default only html
is generated. Start of by pointing your browser at *docs/html/index.html*.

Unit tests
==
This library has a set of unit tests using the
[CppUTest framework](http://cpputest.github.io/) in the tests directory.

The tests are supposed to run on a platform with a working printf
implementation. The generated string is compared to a string generated
by the test systems ordinary printf.

cpputest is supposed to be installed at $(HOME)/tools/cpputest, update in
tests/Makefile to fit your installation.

Motivation
==
For Cortex M3 using stdio.h available in newlib makes the binary
about 24k bytes bigger. This library adds less than 2k. This library
has no input functions though.

What is so special about this library?
==
This printf library does not use any buffer to convert numerics to
printable strings. Instead it divides down the numeric until there is one
digit left and then starts multiplying back to print the numeric value
character by character.

Another advantage of not using any internal buffers (except it saves precious
RAM) is that it could be considered reentrant. Great news if you intend to
use an RTOS, for instance.

License
==
Released under the very generous Expat License, see LICENSE.txt.

Caveats
==
You can probably optimize some bytes here and there, since I used int
through out the source code. You can probably also use the stdint.h 
types (uint32_t et al) to make it more portable.
The functions print_uil() and print_ui() are almost the same, though one
are for long and the other for int. print_ui() can probably be optimized
away and only print_uil() be used if necessary. print_ui() is left if a need
to remove usage of long completely is necessary.

Usage
==
* Include "spe_printf.h" in your file.
* Write a function that sends out a character on your device.
* Register that function.
* Make sure you define stdout and stderr as pointers to the variable
  that stores the character printing function.

As a simple example, see spe_print-example.c. It is much simpler than it sounds.

Good luck,
Stefan Petersen (spe [at] ciellt (dot) se)
