# CC = gcc
# CC = icx

CFLAGS=-std=c11 -g -fno-common -Wall -Wno-switch



SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

default: 9cc

9cc: $(OBJS)
	$(CC) $(CFLAGS) -I./include -o $@ $^ $(LDFLAGS)

asm:
	$(CC) -S 9cc.s $^

test: 9cc
	bash ./test/9cc.sh

clean:
	rm *.o

.PHONY: run asm 9cc clean test
