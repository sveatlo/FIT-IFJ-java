CFLAGS=-std=c99 -Wall -Wextra -g
CC=gcc
SRC_DIR=src/
DIST_DIR=dist/
DOCS_DIR=dist/
ZIPFILENAME=ifj_proj1

all: interpret

docs: $(wildcard $(SRC_DIR)*)
	doxygen

interpret: $(SRC_DIR)main.c error.o
	$(CC) $(CFLAGS) $(SRC_DIR)main.c $(DIST_DIR)error.o -o $(DIST_DIR)ifj16

error.o: $(SRC_DIR)error.c $(SRC_DIR)error.h
	$(CC)  $(CFLAGS) -c $(SRC_DIR)error.c -o $(DIST_DIR)error.o


zip:
	zip $(ZIPFILENAME).zip *.c *.h Makefile Doxyfile
clean:
	rm -f $(DIST_DIR)ifj16 $(DIST_DIR)*.o $(DIST_DIR)*.a $(DIST_DIR)*.so $(SRC_DIR)*.gch $(ZIPFILENAME).zip $(DOCS_DIR)html
