CFLAGS=-std=c99 -Wall -Wextra -O2 -g
CC=gcc
SRC_DIR=src/
DIST_DIR=dist/
DOCS_DIR=dist/
ZIPFILENAME=dist/ifj_proj1

all: interpret
	@echo "                                                                      "
	@echo "██╗      █████╗ ███████╗██╗   ██╗    ██████╗ ██████╗  ██████╗ ███████╗"
	@echo "██║     ██╔══██╗╚══███╔╝╚██╗ ██╔╝    ██╔══██╗██╔══██╗██╔═══██╗██╔════╝"
	@echo "██║     ███████║  ███╔╝  ╚████╔╝     ██████╔╝██████╔╝██║   ██║███████╗"
	@echo "██║     ██╔══██║ ███╔╝    ╚██╔╝      ██╔══██╗██╔══██╗██║   ██║╚════██║"
	@echo "███████╗██║  ██║███████╗   ██║       ██████╔╝██║  ██║╚██████╔╝███████║"
	@echo "╚══════╝╚═╝  ╚═╝╚══════╝   ╚═╝       ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚══════╝"
	@echo "                                                                      "
	@echo "                          ██╗███████╗   ██╗                           "
	@echo "                          ██║██╔════╝   ██║                           "
	@echo "                          ██║█████╗     ██║                           "
	@echo "                          ██║██╔══╝██   ██║                           "
	@echo "                          ██║██║   ╚█████╔╝                           "
	@echo "                          ╚═╝╚═╝    ╚════╝                            "
	@echo "                                                                      "



docs: $(wildcard $(SRC_DIR)*)
	doxygen

tests: interpret
	./tests/run.sh -t all $(DIST_DIR)ifj16

stats:
	@echo -n "Lines of code: " && wc -l $(wildcard $(SRC_DIR)*.c $(SRC_DIR)*.h) | tail -n 1 | sed -r "s/[ ]*([0-9]+).*/\1/g"
	@echo -n "Size of code: " && du -hsc $(wildcard $(SRC_DIR)*.c $(SRC_DIR)*.h) | tail -n 1 | cut -f 1


# Link all the modules together
interpret: $(SRC_DIR)main.c error.o scanner.o scanner_token.o list.o string.o parser.o instruction.o \
			ial.o expression.o symbol.o context.o interpret.o inbuilt.o stack.o precedence_table.o callframe.o
	$(CC) $(CFLAGS) \
		$(SRC_DIR)main.c $(DIST_DIR)error.o $(DIST_DIR)scanner.o $(DIST_DIR)scanner_token.o \
		$(DIST_DIR)list.o $(DIST_DIR)string.o $(DIST_DIR)parser.o $(DIST_DIR)instruction.o \
		$(DIST_DIR)ial.o $(DIST_DIR)expression.o $(DIST_DIR)symbol.o $(DIST_DIR)context.o \
		$(DIST_DIR)interpret.o $(DIST_DIR)inbuilt.o $(DIST_DIR)stack.o  $(DIST_DIR)precedence_table.o \
		$(DIST_DIR)callframe.o \
	-o $(DIST_DIR)ifj16

# Make modules independently
error.o: $(SRC_DIR)error.c $(SRC_DIR)error.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)error.c -o $(DIST_DIR)error.o
scanner.o: $(SRC_DIR)scanner.c $(SRC_DIR)scanner.h scanner_token.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)scanner.c -o $(DIST_DIR)scanner.o
scanner_token.o: $(SRC_DIR)scanner_token.c $(SRC_DIR)scanner_token.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)scanner_token.c -o $(DIST_DIR)scanner_token.o
list.o: $(SRC_DIR)list.c $(SRC_DIR)list.h $(SRC_DIR)variable.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)list.c -o $(DIST_DIR)list.o
string.o: $(SRC_DIR)string.c $(SRC_DIR)string.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)string.c -o $(DIST_DIR)string.o
parser.o: $(SRC_DIR)parser.c $(SRC_DIR)parser.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)parser.c -o $(DIST_DIR)parser.o
instruction.o: $(SRC_DIR)instruction.c $(SRC_DIR)instruction.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)instruction.c -o $(DIST_DIR)instruction.o
expression.o: $(SRC_DIR)expression.c $(SRC_DIR)expression.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)expression.c -o $(DIST_DIR)expression.o
interpret.o: $(SRC_DIR)interpret.c $(SRC_DIR)interpret.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)interpret.c -o $(DIST_DIR)interpret.o
ial.o: $(SRC_DIR)ial.c $(SRC_DIR)ial.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)ial.c -o $(DIST_DIR)ial.o
symbol.o: $(SRC_DIR)symbol.c $(SRC_DIR)symbol.h $(SRC_DIR)variable.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)symbol.c -o $(DIST_DIR)symbol.o
context.o: $(SRC_DIR)context.c $(SRC_DIR)context.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)context.c -o $(DIST_DIR)context.o
inbuilt.o: $(SRC_DIR)inbuilt.c $(SRC_DIR)inbuilt.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)inbuilt.c -o $(DIST_DIR)inbuilt.o
stack.o: $(SRC_DIR)stack.c $(SRC_DIR)stack.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)stack.c -o $(DIST_DIR)stack.o
precedence_table.o: $(SRC_DIR)precedence_table.c $(SRC_DIR)precedence_table.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)precedence_table.c -o $(DIST_DIR)precedence_table.o
callframe.o: $(SRC_DIR)callframe.c $(SRC_DIR)callframe.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)callframe.c -o $(DIST_DIR)callframe.o

zip:
	zip $(ZIPFILENAME).zip src/*.c src/*.h Makefile Doxyfile
clean:
	rm -f $(DIST_DIR)ifj16 $(DIST_DIR)*.o $(DIST_DIR)*.a $(DIST_DIR)*.so $(SRC_DIR)*.gch $(ZIPFILENAME).zip $(DOCS_DIR)html
