CC=gcc
AR=ar
OBJECTS_MAIN=main.o
OBJECTS_LIB=mylib.o
FLAGS= -Wall -g

all: isort txtfind
txtfind: main.o
	$(CC) $(FLAGS) -o graph main.o
main.o: main.c
	$(CC) $(FLAGS) -c main.c
.PHONY: clean all isort txtfind
clean:
	rm -f *.o *.a connections
