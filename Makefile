CC=gcc
CXX=g++
# See http://www.gnu.org/s/libc/manual/html_mono/libc.html#index-off_005ft-1223
CFLAGS=-Wall -O2 -D_FILE_OFFSET_BITS=64 
DARLIBS=$(shell pkg-config --libs libdar64)
FUSELIBS=$(shell pkg-config --libs fuse)
DARCFLAGS=$(shell pkg-config --cflags libdar64)
FUSECFLAGS=$(shell pkg-config --cflags fuse)
EXE=darfs

all: ${EXE}

darfs: dar.o fuse.o
	${CXX} ${DARLIBS} ${FUSELIBS} ${DARLIBS} dar.o fuse.o -o darfs

fuse.o: fuse.c
	${CC} ${FUSECFLAGS} ${CFLAGS} -c fuse.c -o fuse.o

dar.o: dar.cpp
	${CXX} ${DARCFLAGS} ${CFLAGS} -c dar.cpp -o dar.o

clean:
	rm -f *.o ${EXE}
