#include <stdio.h>
#include <stdlib.h>
#include "context.h"
#include "error.h"
#include "interpret.h"
#include "parser.h"
#include "scanner.h"

extern Error last_error;

/**
 *  @brief parses parameters
 */
FILE* parse_parameters(int, char**);

/**
 *  @brief entry-point function
 */
int main(int argc, char** argv) {
    // (void)argv;
    // (void)argc;
    set_error(ERR_NONE);

    FILE *f = parse_parameters(argc, argv);
    if(get_error()->type) {
        // lex error => exit
        // TODO: cleanup
        print_error();
        return get_error()->type;
    }

    List *token_list = list_init();

    token_list = scan_file(f, token_list);
    if(get_error()->type) {
        // lex error => exit
        // TODO: cleanup
        print_error();
        return get_error()->type;
    }

    // list_activate_first(token_list);
    // while(token_list->active != NULL) {
    //     ScannerToken* token = token_list->active->data.token;
    //     if(token->type == STT_IDENT) {
    //         if(token->data->id->class != NULL) {
    //             printf("%s:%s:%s\n", token_to_string(token), str_get_str(token->data->id->class), str_get_str(token->data->id->name));
    //         } else {
    //             printf("%s:%s\n", token_to_string(token), str_get_str(token->data->id->name));
    //
    //         }
    //     } else {
    //         printf("%s ", token_to_string(token));
    //     }
    //     list_activate_next(token_list);
    // }
    // printf("\n");
    //
    // list_dispose(token_list);

    Context* context = NULL;
    List* instructions = NULL;
    parse(token_list, &context, &instructions);

    if(get_error()->type) {
        // parser error => exit
        // TODO: cleanup
        print_error();
        return get_error()->type;
    }

    interpret(context, instructions);
    if(get_error()->type) {
        // interpret error => exit
        // TODO: cleanup
        print_error();
        return get_error()->type;
    }

    fclose(f);

    return 0;
}

FILE* parse_parameters(int argc, char** argv) {
    if(argc != 2 || argc > 3) {
        set_error(ERR_INTERPRET);
        return NULL;
    }

    FILE *f = fopen(argv[1], "r");
    if(f == NULL) {
        set_error(ERR_INTERPRET);
        return NULL;
    }

    return f;
}
