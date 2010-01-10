# Link together C and C++

all: hello

hello: hello.o cpp.o
	g++ -Wall `pkg-config fuse --libs` hello.o cpp.o -o hello

hello.o: hello.c
	gcc -Wall `pkg-config fuse --cflags` -c hello.c -o hello.o

cpp.o: cpp.cpp
	g++ -c cpp.cpp -o cpp.o

clean:
	rm -f *.o hello
