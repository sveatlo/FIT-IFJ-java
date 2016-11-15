CFLAGS=-std=c99 -Wall -Wextra -g
CC=gcc
SRC_DIR=src/
DIST_DIR=dist/
DOCS_DIR=dist/
ZIPFILENAME=ifj_proj1

all: interpret

docs: $(wildcard $(SRC_DIR)*)
	doxygen

# Link all the modules together
interpret: $(SRC_DIR)main.c error.o scanner.o scanner_token.o list.o string.o ial.o
	$(CC) $(CFLAGS) \
		$(SRC_DIR)main.c $(DIST_DIR)error.o $(DIST_DIR)scanner.o $(DIST_DIR)scanner_token.o \
		$(DIST_DIR)list.o $(DIST_DIR)string.o $(DIST_DIR)ial.o \
	-o $(DIST_DIR)ifj16

# Make modules independently
error.o: $(SRC_DIR)error.c $(SRC_DIR)error.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)error.c -o $(DIST_DIR)error.o
scanner.o: $(SRC_DIR)scanner.c $(SRC_DIR)scanner.h scanner_token.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)scanner.c -o $(DIST_DIR)scanner.o
scanner_token.o: $(SRC_DIR)scanner_token.c $(SRC_DIR)scanner_token.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)scanner_token.c -o $(DIST_DIR)scanner_token.o
list.o: $(SRC_DIR)list.c $(SRC_DIR)list.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)list.c -o $(DIST_DIR)list.o
string.o: $(SRC_DIR)string.c $(SRC_DIR)string.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)string.c -o $(DIST_DIR)string.o
ial.o: $(SRC_DIR)ial.c $(SRC_DIR)ial.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)ial.c -o $(DIST_DIR)ial.o


zip:
	zip $(ZIPFILENAME).zip *.c *.h Makefile Doxyfile
clean:
	rm -f $(DIST_DIR)ifj16 $(DIST_DIR)*.o $(DIST_DIR)*.a $(DIST_DIR)*.so $(SRC_DIR)*.gch $(ZIPFILENAME).zip $(DOCS_DIR)html
