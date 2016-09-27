CFLAGS=-std=c99 -Wall -Wextra -pedantic -g
CC=gcc
SRC_DIR=src
DIST_DIR=dist
ZIPFILENAME=ifj_proj1

all: interpret


interpret: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c -o $(DIST_DIR)/interpret


zip:
	zip $(ZIPFILENAME).zip *.c *.h Makefile Doxyfile
clean:
	rm -f $(DIST_DIR)/interpret $(DIST_DIR)/*.o $(DIST_DIR)/*.a $(DIST_DIR)/*.so $(DIST_DIR)/*.gch $(ZIPFILENAME).zip
