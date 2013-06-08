spe printf
===

A printf for small micro controllers, mainly Cortex M3.

Supported conversion tags
==
It supports:
* minimal width and optional precision for all numerical types.
* long modifier.
* plain percent (%), character (c), string (s), signed integer, 
  unsigned integer(u), hex (x) and binary (b).
* double if compiled in, see below

It does not support:
* minimal width and optional precision for strings.
* negative minimal width.
* minimal width as a parameter to the conversion string (*)

double()
==
It can print doubles also. Then the variable USE_DOUBLE needs to be set
when compiling, by for instance adding CFLAGS+=-DUSE_DOUBLE in the Makefile.
When including usage of double, math.h and math lib is necessary. Using just
casting to change one part (integer or decimal) of the double to an int lost
too much precision. So by using truncf() instead I managed to get better
precision, especially in the decimal part.

Motivation
==
For Cortex M3 using stdio.h available in newlib makes the binary
about 24k bytes bigger. This library adds less than 2k. This library
has no input functions though.

License
==
Released under the very generous Expat License, see LICENSE.txt.

Caveats
==
You can probably optimize some bytes here and there, since I used int
through out the source code. You can probably also use the stdint.h 
types (uint32_t et al) to make it more portable.
The functions print_uil() and print_ui() are almost the same, though one
are for long and the other for int. print_ui() can probably be optimimized
away and only print_uil() be used if necessary. print_ui() is left if a need
to remove usage of long completely is necessary.

Usage
==
* Include "spe_printf.h" in your file.
* Write a function that sends out a character on your device.
* Register that function.
* Make sure you define stdout and stderr as pointers to the variable
  that stores the character printing function.

As an example, see spe_print-test.c. It is much simpler than it sounds.

Good luck,
Stefan Petersen (spe [at] ciellt (dot) se)
