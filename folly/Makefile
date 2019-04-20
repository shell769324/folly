CC=g++

DEBUG=0
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c++17
LDFLAGS= -lm

CFILES = testFromMe.cpp

all: testFromMe

testFromMe: $(CFILES) 
	$(CC) $(CFLAGS) -o testFromMe $(CFILES) $(LDFLAGS) /usr/local/lib/libdouble-conversion.a -lfolly -lglog -ljemalloc