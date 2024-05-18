CFLAGS=-std=c11 -g -fno-common
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

default: test

chibicc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): chibicc.h

test: chibicc
	./test.sh

ci: test

clean:
	rm -f chibicc *.o *~ tmp*

.PHONY: ci test clean
