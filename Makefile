CC=g++
CXXFLAGS=-O3 -o test

INCLUDES=-I./

FILES=main.cpp

LIBS=-lpthread

all:
	$(CC) $(CXXFLAGS) $(FILES) $(LIBS)

clean:
	rm test
