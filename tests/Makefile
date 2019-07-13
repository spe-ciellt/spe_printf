#
# Copyright (c) 2013-2015 Stefan Petersen, Ciellt AB
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

#---------
#
# spe_printf CppUTest  Makefile
#
#----------

#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

#--- Inputs ----#
COMPONENT_NAME = spe_printf
CPPUTEST_HOME = /usr
CPPUTEST_SCRIPTS = share/cpputest
TARGET = $(shell uname -m)
TARGET_PLATFORM = $(TARGET)-linux-gnu
TOPDIR = $(shell dirname `pwd`)

CPPUTEST_USE_EXTENSIONS = Y
CPPUTEST_WARNINGFLAGS =  -Wall -Wextra -Werror -Wshadow -Wswitch-default -Wswitch-enum
CPPUTEST_ADDITIONAL_CFLAGS = -DUSE_DOUBLE
CPPUTEST_LDFLAGS = -lm

CPP_PLATFORM = Gcc

# This line is overriding the default new macros.  This is helpful
# when using std library includes like <list> and other containers
# so that memory leak detection does not conflict with stl.
#CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE = -include ApplicationLib/ExamplesNewOverrides.h
MY_SRC_DIRS = $(TOPDIR)/src
SRC_FILES = $(MY_SRC_DIRS)/spe_printf.c

TEST_SRC_DIRS = AllTests

MOCKS_SRC_DIRS = Mocks

INCLUDE_DIRS =\
  $(MY_SRC_DIRS)\
  AllTests\
  $(CPPUTEST_HOME)/include

# File from CppUTest distribution that simplifies building tests.
include MakefileWorker.mk
