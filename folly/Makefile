CC=g++

DEBUG=0
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c++14
LDFLAGS= -lm

<<<<<<< HEAD
CFILES = test_ll_skiplist.cpp
=======
LIBS     = -lfolly -lglog -ljemalloc -l pthread -ldl -ldouble-conversion

CFILES = testFromMe.cpp
>>>>>>> 04c0b47f635ad79e6e870a10f99020566cc30d3c

all: test_ll_skiplist

<<<<<<< HEAD
test_ll_skiplist: $(CFILES) 
	$(CC) $(CFLAGS) -o test_ll_skiplist $(CFILES) $(LDFLAGS) /usr/local/lib/libdouble-conversion.a -lfolly -lglog -ljemalloc
=======
testFromMe: $(CFILES) 
	$(CC) $(CFLAGS) -o testFromMe $(CFILES) $(LDFLAGS) $(LIBS)
>>>>>>> 04c0b47f635ad79e6e870a10f99020566cc30d3c
