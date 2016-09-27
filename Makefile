CFLAGS=-std=c99 -Wall -Wextra -pedantic -g
CC=gcc
ZIPFILENAME=ifj_proj1

all: interpret


interpret: main.c
	$(CC) $(CFLAGS) main.c -o interpret


zip:
	zip $(ZIPFILENAME).zip *.c *.h Makefile Doxyfile
clean:
	rm -f tail tail2 wordcount wordcount2 wordcount-dynamic *.o *.a *.so *.gch $(ZIPFILENAME).zip
