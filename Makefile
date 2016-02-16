# Makefile for suftest and test

# options
CC						= gcc
CFLAGS				= -std=c11 -m64 -O3 -fomit-frame-pointer -g
CPPFLAGS			= -Wall -DNDEBUG
LDFLAGS				=
LDLIBS				=
#TARGET_ARCH		=

# targets
.PHONY: all
all: a2t
a2t: sais.o a2t.o

distclean: clean
clean:
	$(RM) a2t *.o

# dependencies
sais.o a2t.o: sais.h Makefile
