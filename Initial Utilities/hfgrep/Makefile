#
# Makefile for C hfgrep
#
CC = gcc
CFLAGS = -Wall -Werror
RM = /bin/rm
RMFLAGS = -rf
SRCS =  hfgrep.c

OBJS = $(SRCS:.c=.out.o)
PROGS = $(SRCS:.c=.out)

# This is the multi-executable target...
# If you type 'make' at the command line it will build
# all programs
.PHONY: all

all: $(PROGS)

$(PROGS): %: %.o Makefile
	$(CC) $< -o $@

# This line will convert any .c file to a .o file.
%.out.o: %.c Makefile
	$(CC) $(CFLAGS) -o $@ -c $<

# Type 'make clean' up the subdirectory...
# Be Very Careful Not To Haphazzardly Make Changes!!!!!!!
clean:
	$(RM) $(RMFLAGS) $(PROGS) $(OBJS) tests-out

test:
	./test-hfgrep.sh

test-all:
	./test-hfgrep.sh -c
