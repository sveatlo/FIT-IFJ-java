#include <stdio.h>
#include <stdlib.h>
#include "context.h"
#include "error.h"
#include "expression.h"
#include "ial.h"
#include "interpret.h"
#include "list.h"
#include "parser.h"
#include "scanner_token.h"
#include "scanner.h"
#include "string.h"
#include "interpret.h"

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
    // set_error(ERR_NONE);
    // String* str0 = str_init();
    // str_append(str0, 'a');
    //
    // String* str1 = str_init();
    // str_append(str1, 'b');
    //
    // String* str2 = str_init();
    // str_append(str2, 'c');
    //
    // String* str3 = str_init();
    // str_append(str3, 'x');
    //
    // String* str4 = str_init();
    // str_append(str4, 'z');
    //
    // String* str5 = str_init();
    // str_append(str5, 'y');
    //str_concat(str0, str1);
    // printf("%s\n", str0->str);
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

    // Context* ctx = context_init(NULL);
    // context_delete(ctx);
        // (void)argv;
        // (void)argc;

    // SymbolTableNode* st = table_init(););
    // Symbol* a = symbol_init(str0);
    // Symbol* b = symbol_init(str1);
    // Symbol* c = symbol_init(str2);
    // symbol_new_variable(a, VT_INTEGER);
    // printf("jedna");
    // SymbolName x = str3;
    // SymbolName z = str4;
    // SymbolName y = str5;
    // tree_insert(tree, x, a);
    // tree_insert(tree, z, b);
    // tree_insert(tree, y, c);
    // SymbolTableNode *copy = tree_copy(tree);

    // Expression* expr1 = expression_init();
    // expr1->op = EO_CONST_INTEGER;
    // expr1->i = 5;
    //
    // Expression* expr2 = expression_init();
    // expr2->op = EO_CONST_DOUBLE;
    // expr2->d = 3.0;
    //
    // Expression* expr3 = expression_init();
    // expr3->op = EO_CONST_INTEGER;
    // expr3->i = 2;
    //
    // Expression* expr23 = expression_init();
    // expr23->op = EO_MULTIPLY;
    // expr23->expr1 = expr2;
    // expr23->expr2 = expr3;
    //
    // Expression* expr = expression_init();
    // expr->op = EO_PLUS;
    // expr->expr1 = expr1;
    // expr->expr2 = expr23;
    //
    // expression_print(expr);
    // printf("\n");
    // expression_print(evaluate_expression(expr));
    // printf("\n");



    FILE *f = parse_parameters(argc, argv);
    List *token_list = list_init();

    token_list = scan_file(f, token_list);

    list_activate_first(token_list);
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

    // list_dispose(token_list);

    if(get_error()->type) {
        //lex error => exit
        print_error();
        return -1;
    }

    if(get_error()->type) {
        //lex error => exit
        print_error();
        return get_error()->type;
    }

    Context* context = NULL;
    List* instructions = NULL;
    parse(token_list, &context, &instructions);

    if(get_error()->type) {
        //lex error => exit
        print_error();
        return get_error()->type;
    }

    // list_activate_first(instructions);
    // while(instructions->active != NULL) {
    //     printf("main.c: ");
    //     instruction_print(instructions->active->data.instruction);
    //     list_activate_next(instructions);
    // }
    // printf("\n");
    interpret(context, instructions);
    if(get_error()->type) {
        //lex error => exit
        print_error();
        return get_error()->type;
    }



    fclose(f);


    return 0;
}

FILE* parse_parameters(int argc, char** argv) {
    if(argc != 2 || argc > 3) {
        set_error(ERR_SEM_PARAMS);
        return NULL;
    }

    FILE *f = fopen(argv[1], "r");
    if(f == NULL) {
        set_error(ERR_INTERPRET);
        return NULL;
    }

    return f;
}
