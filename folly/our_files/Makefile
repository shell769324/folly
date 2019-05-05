CC=g++

DEBUG=0
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c++14
LDFLAGS= -lm

CFILES = test_folly.cpp

all: test_folly

test_folly: $(CFILES) 
	$(CC) $(CFLAGS) -o test_folly $(CFILES) $(LDFLAGS) /usr/local/lib/libdouble-conversion.a -lfolly -lglog -ljemalloc