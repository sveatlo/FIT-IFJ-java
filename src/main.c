/*
 * Course: IFJ
 * Project name: Implementace interpretu imperativního jazyka IFJ16
 * Description: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf
 *
 *
 * Team:
 *  Dominik Križka   (xkrizk02)
 *  Matúš Juhász     (xjuhas02)
 *  Samuel Hulla     (xhulla00)
 *  Svätopluk Hanzel (xhanze10)
 *  Tomáš Haas       (xhaast00)
 */



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
        // print_error();
        return get_error()->type;
    }

    List *token_list = list_init(LT_TOKEN);

    token_list = scan_file(f, token_list);
    //dispose after lex error from scanner
    if(get_error()->type) {
        // lex error => exit
        list_dispose(token_list);
        // print_error();
        return get_error()->type;
    }
    fclose(f);

    // list_activate_first(token_list);
    // char keywords[][255] = {
    //     [KW_BOOLEAN] = "KW_BOOLEAN",
    //     [KW_BREAK] = "KW_BREAK",
    //     [KW_CLASS] = "KW_CLASS",
    //     [KW_CONTINUE] = "KW_CONTINUE",
    //     [KW_DO] = "KW_DO",
    //     [KW_DOUBLE] = "KW_DOUBLE",
    //     [KW_ELSE] = "KW_ELSE",
    //     [KW_FALSE] = "KW_FALSE",
    //     [KW_FOR] = "KW_FOR",
    //     [KW_IF] = "KW_IF",
    //     [KW_INT] = "KW_INT",
    //     [KW_RETURN] = "KW_RETURN",
    //     [KW_STATIC] = "KW_STATIC",
    //     [KW_STRING] = "KW_STRING",
    //     [KW_TRUE] = "KW_TRUE",
    //     [KW_VOID] = "KW_VOID",
    //     [KW_WHILE] = "KW_WHILE"
    // };
    // for(ScannerToken* current_token = token_list->active->data.token; token_list->active != NULL; list_activate_next(token_list), current_token = token_list->active != NULL ? token_list->active->data.token : NULL) {
    //     switch (current_token->type) {
    //         case STT_IDENT:
    //             if(current_token->data->id->class) {
    //                 printf("<%s:%s.%s>", token_to_string(current_token), str_get_str(current_token->data->id->class), str_get_str(current_token->data->id->name));
    //             } else {
    //                 printf("<%s:%s>", token_to_string(current_token), str_get_str(current_token->data->id->name));
    //             }
    //             break;
    //         case STT_KEYWORD:
    //             printf("<%s:%s>", token_to_string(current_token), keywords[current_token->data->keyword_type]);
    //             break;
    //         default:
    //             printf("<%s>", token_to_string(current_token));
    //     }
    // }
    // return -1;

    Context* context = NULL;
    List* instructions = NULL;
    parse(token_list, &context, &instructions);
    list_dispose(token_list);

    if(get_error()->type) {
        // parser error => exit
        list_dispose(instructions);
        context_dispose(context);
        // print_error();
        return get_error()->type;
    }

    interpret(context, instructions);
    if(get_error()->type) {
        // interpret error => exit
        list_dispose(instructions);
        context_dispose(context);
        // print_error();
        return get_error()->type;
    }


    list_dispose(instructions);
    context_dispose(context);

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
