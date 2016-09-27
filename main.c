#include <stdio.h>
#include <stdlib.h>

void parseParameters(int, char**);

int main(int argc, char** argv) {
    parseParameters(argc, argv);

    return 0;
}

void parseParameters(int argc, char** argv) {
    if(argc != 2) {
        exit(1);
    }

    printf("Hello IFJ! %s\n", argv[0]);
}
