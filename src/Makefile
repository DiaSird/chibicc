# CC = gcc
# CC = icx
CFLAGS=-std=c11 -g -fno-common -Wall -Wno-switch

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

SRC = ./9cc
NUM1 = 110
NUM2 = 20

default: compile

run: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

compile: 
	$(CC) -o $(SRC) $(SRC).c

test: compile
	bash ./test/9cc.sh

.PHONY: run compile test

# command -v cc