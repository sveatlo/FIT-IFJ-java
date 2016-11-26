#include <stdio.h>
#include <stdlib.h>
#include "context.h"
#include "error.h"
#include "expression.h"
#include "ial.h"
#include "list.h"
#include "parser.h"
#include "scanner_token.h"
#include "scanner.h"
#include "string.h"

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

    // String* str0 = str_init();
    // str_append(str0, 'a');
    //
    // String* str1 = str_init();
    // str_append(str1, 'b');
    //
    // str_concat(str0, str1);
    // printf("%s\n", str_get_str(str0));

    // // printf("%s\n", str_get_str(str0));
    // // str_clear(str0);
    // // printf("%s\n", str0->str);
    // printf("%d\n", str_cmp_const(str0, "abc\0"));
    //
    // str_dispose(str0);

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

    // Context* ctx = context_init();
    // context_delete(ctx);
        (void)argv;
        (void)argc;
    // SymbolTableNode* table = table_init();
    // table_insert_integer(table, str0, 42);
    // table_insert_integer(table, str1, 69);
    // table_dispose(table);


    // FILE *f = parse_parameters(argc, argv);
    // List *token_list = list_init();
    //
    // token_list = scan_file(f, token_list);
    //
    // // list_activate_first(token_list);
    // // while(token_list->active != NULL) {
    // //     ScannerToken* token = token_list->active->data.token;
    // //     if(token->type == STT_IDENT) {
    // //         if(token->data->id->class != NULL) {
    // //             printf("%s:%s:%s\n", token_to_string(token), str_get_str(token->data->id->class), str_get_str(token->data->id->name));
    // //         } else {
    // //             printf("%s:%s\n", token_to_string(token), str_get_str(token->data->id->name));
    // //
    // //         }
    // //     } else {
    // //         printf("%s ", token_to_string(token));
    // //     }
    // //     list_activate_next(token_list);
    // // }
    // // printf("\n");
    //
    // // list_dispose(token_list);
    //
    // if(get_error()->type) {
    //     //lex error => exit
    //     print_error();
    //     return -1;
    // }
    //
    // parse(token_list);
    //
    // if(get_error()->type) {
    //     //lex error => exit
    //     print_error();
    //     return -1;
    // }
    //
    //
    // fclose(f);

//     Expression* expr1 = expression_init();
//     expr1->op = EO_CONST_BOOL;
//     expr1->b = true;
//
//     Expression* expr2 = expression_init();
//     expr2->op = EO_CONST_DOUBLE;
//     expr2->d = 35.1;
//
//     Expression* expr3 = expression_init();
//     expr3->op = EO_CONST_INTEGER;
//     expr3->i = 35;
//
//     Expression* expr23 = expression_init();
//     expr23->op = EO_LOGIC_EQUAL;
//     expr23->expr1 = expr2;
//     expr23->expr2 = expr3;
//
//     Expression* expr = expression_init();
//     expr->op = EO_LOGIC_AND;
//     expr->expr1 = expr1;
//     expr->expr2 = expr23;
//
//     expression_print(expr);
//     printf("\n");
//     expression_print(expression_evaluate(expr));
//     printf("\n");
//
//
//
//     return 0;
// }

// FILE* parse_parameters(int argc, char** argv) {
//     if(argc != 2 || argc > 3) {
//         set_error(ERR_PARAMS);
//         return NULL;
//     }
//
//     FILE *f = fopen(argv[1], "r");
//     if(f == NULL) {
//         set_error(ERR_CANNOT_OPEN_FILE);
//         return NULL;
//     }
//
//     return f;
// }
