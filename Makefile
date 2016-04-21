CC=gcc
CFLAGS=-ansi -pedantic -Werror -Wall

all: head.c
	$(CC) -o head $(CFLAGS) head.c
