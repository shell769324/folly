CC=g++

DEBUG=0
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c++17
LDFLAGS= -lm

LIBS     = -lfolly -lglog -ljemalloc -l pthread -ldl -ldouble-conversion

CFILES = testFromMe.cpp

all: testFromMe

testFromMe: $(CFILES) 
	$(CC) $(CFLAGS) -o testFromMe $(CFILES) $(LDFLAGS) $(LIBS)