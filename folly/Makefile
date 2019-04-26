CC=g++

DEBUG=0
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c++14
LDFLAGS= -lm

CFILES = test_ll_skiplist.cpp

all: test_ll_skiplist

test_ll_skiplist: $(CFILES) 
	$(CC) $(CFLAGS) -o test_ll_skiplist $(CFILES) $(LDFLAGS) /usr/local/lib/libdouble-conversion.a -lfolly -lglog -ljemalloc