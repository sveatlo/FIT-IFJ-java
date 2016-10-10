#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "list.h"
#include "scanner.h"
#include "scanner_token.h"

/**
 *  @brief parses parameters
 */
FILE* parse_parameters(int, char**);

/**
 *  @brief entry-point function
 */
int main(int argc, char** argv) {
    set_error(ERR_NONE);

    FILE *f = parse_parameters(argc, argv);
    List *token_list = list_init();

    token_list = scan_file(f, token_list);

    list_activate_first(token_list);
    while(token_list->active != NULL) {
        printf("%s ", token_to_string(token_list->active->data.token));
        list_activate_next(token_list);
    }
    printf("\n");

    list_dispose(token_list);

    // List *token_list = list_init();
    //
    // ScannerToken *token1 = token_init();
    // token1->type = STT_IDENT;
    // ListItemData data1;
    // data1.token = token1;
    //
    // ScannerToken *token2 = token_init();
    // token2->type = STT_COMMENT;
    // ListItemData data2;
    // data2.token = token2;
    //
    // ScannerToken *token3 = token_init();
    // token3->type = STT_LEFT_PARENTHESE;
    // ListItemData data3;
    // data3.token = token3;
    //
    // list_insert_last(token_list, data1);
    // list_insert_last(token_list, data2);
    // list_insert_last(token_list, data3);
    //
    // list_activate_first(token_list);
    // while(token_list->active != NULL) {
    //     printf("%s ", token_to_string(token_list->active->data.token));
    //     list_activate_next(token_list);
    // }


    fclose(f);
    return 0;
}

FILE* parse_parameters(int argc, char** argv) {
    if(argc != 2 || argc > 3) {
        set_error(ERR_PARAMS);
        return NULL;
    }

    FILE *f = fopen(argv[1], "r");
    if(f == NULL) {
        set_error(ERR_CANNOT_OPEN_FILE);
        return NULL;
    }

    return f;
}
