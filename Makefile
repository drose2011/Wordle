CC  = gcc

INCLUDES = #-I../../lab3/solutions/part1
CFLAGS   = -g -Wall $(INCLUDES)

LDFLAGS = -g -L.#./../lab3/solutions/part1
LDLIBS = -lmylist

wordle: wordle.o

wordle.o: mylist.h wordle.c

gen5dict: gen5dict.o

gen5dict.o: gen5dict.c

.PHONY: clean
clean:
	rm -f *.o *~ a.out core wordle gen5dict

.PHONY: all
all: clean wordle

