/*
 * Course: IFJ
 * Project name: Implementace interpretu imperativního jazyka IFJ16
 * Description: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf
 *
 *
 * Team:
 *  Dominik Križka   (xkrizk02)
 *  Samuel Hulla     (xhulla00)
 *  Svätopluk Hanzel (xhanze10)
 *  Tomáš Haas       (xhaast00)
 */



#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "expression.h"
#include "inbuilt.h"
#include "string.h"

extern Error last_error;

int get_base(String* str) {
    char* cstring = str_get_str(str);
    if(cstring == NULL) {
        return 0;
    }

    if(str_length(str) == 1) {
        return 0;
    }

    if(cstring[0] == '0') {
        if(cstring[1] == 'b') {
            return 2;
        } else if(cstring[1] == 'x') {
            return 16;
        }
    }

    return 0;
}

int read_int() {
    String* number = str_init();
    int c = getchar();
    while (c != '\n') {
        str_append(number, c);
        c = getchar();
    }
    char *err;
    int base = get_base(number);
    int i = strtol((base == 2 || base == 16) ? substr(number, 2, str_length(number) - 2)->str : number->str, &err, base);
    if (*err != 0 || errno == ERANGE || i >= INT_MAX || i <= INT_MIN) {
        set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
        return -1;
    }

    return i;
}

double read_double() {
    String* number = str_init();
    int c = getchar();
    while (c != '\n') {
        str_append(number, c);
        c = getchar();
    }
    char *err;
    double d = strtod(number->str, &err);
    if (*err != 0 || errno == ERANGE || d >= DBL_MAX || d <= DBL_MIN) {
        set_error(ERR_READ_NUM_FROM_STDIN); //error from stdin
        return -1;
    }

    return d;
}

String* read_str() {
    String* str = str_init();

    int c = getchar();
    while (c != '\n') {
        str_append(str, c);
        c = getchar();
    }
    return str;
}

void print_to_stdin(Expression *expr) {
    switch (expr->op) {
        case EO_CONST_STRING:
            printf("%s", str_get_str(expr->str));
            break;
        case EO_CONST_INTEGER:
            printf("%d", expr->i);
            break;
        case EO_CONST_DOUBLE:
            printf("%g", expr->d);
            break;
        case EO_CONST_BOOL:
            printf("%s", expr->b ? "true" : "false");
            break;
        default:
            set_error(ERR_SEM_PARAMS);
            break;
    }
}


void add_builtin_class(Context* context) {
        String* name = str_init_const("ifj16");
        Symbol* new_class = table_insert_class(context->symbol_table, name, context);
        str_dispose(name);
        Context* current_context = new_class->data.cls->context;

        Ident* fn_read_int_id = (Ident*)malloc(sizeof(Ident));
        fn_read_int_id->class = str_init_const("ifj16");
        fn_read_int_id->name = str_init_const("readInt");
        /*Symbol* fn_read_int = */context_add_function(current_context, KW_INT, fn_read_int_id);
        str_dispose(fn_read_int_id->class);
        str_dispose(fn_read_int_id->name);
        free(fn_read_int_id);

        Ident* fn_read_double_id = (Ident*)malloc(sizeof(Ident));
        fn_read_double_id->class = str_init_const("ifj16");
        fn_read_double_id->name = str_init_const("readDouble");
        /*Symbol* fn_read_double = */context_add_function(current_context, KW_DOUBLE, fn_read_double_id);
        str_dispose(fn_read_double_id->class);
        str_dispose(fn_read_double_id->name);
        free(fn_read_double_id);

        Ident* fn_read_string_id = (Ident*)malloc(sizeof(Ident));
        fn_read_string_id->class = str_init_const("ifj16");
        fn_read_string_id->name = str_init_const("readString");
        /*Symbol* fn_read_string = */context_add_function(current_context, KW_STRING, fn_read_string_id);
        str_dispose(fn_read_string_id->class);
        str_dispose(fn_read_string_id->name);
        free(fn_read_string_id);

        Ident* fn_length_id = (Ident*)malloc(sizeof(Ident));
        fn_length_id->class = str_init_const("ifj16");
        fn_length_id->name = str_init_const("length");
        Symbol* fn_length = context_add_function(current_context, KW_INT, fn_length_id);
        str_dispose(fn_length_id->class);
        str_dispose(fn_length_id->name);
        free(fn_length_id);
        Ident* id0 = (Ident*)malloc(sizeof(Ident));
        id0->class = NULL;
        id0->name = str_init_const("s");
        ListItemData param_type0, param_id0 = {
            .id = id0
        };
        param_type0.var_type = VT_STRING;
        list_insert_last(fn_length->data.fn->params_types_list, param_type0);
        list_insert_last(fn_length->data.fn->params_ids_list, param_id0);

        Ident* fn_substring_id = (Ident*)malloc(sizeof(Ident));
        fn_substring_id->class = str_init_const("ifj16");
        fn_substring_id->name = str_init_const("substr");
        Symbol* fn_substring = context_add_function(current_context, KW_STRING, fn_substring_id);
        str_dispose(fn_substring_id->class);
        str_dispose(fn_substring_id->name);
        free(fn_substring_id);
        Ident* id1 = (Ident*)malloc(sizeof(Ident));
        id1->class = NULL;
        id1->name = str_init_const("s");
        ListItemData param_type1, param_id1 = {
            .id = id1
        };
        param_type1.var_type = VT_STRING;
        list_insert_last(fn_substring->data.fn->params_types_list, param_type1);
        list_insert_last(fn_substring->data.fn->params_ids_list, param_id1);
        Ident* id2 = (Ident*)malloc(sizeof(Ident));
        id2->class = NULL;
        id2->name = str_init_const("i");
        ListItemData param_type2, param_id2 = {
            .id = id2
        };
        param_type2.var_type = VT_INTEGER;
        list_insert_last(fn_substring->data.fn->params_types_list, param_type2);
        list_insert_last(fn_substring->data.fn->params_ids_list, param_id2);
        Ident* id3 = (Ident*)malloc(sizeof(Ident));
        id3->class = NULL;
        id3->name = str_init_const("n");
        ListItemData param_type3, param_id3 = {
            .id = id3
        };
        param_type3.var_type = VT_INTEGER;
        list_insert_last(fn_substring->data.fn->params_types_list, param_type3);
        list_insert_last(fn_substring->data.fn->params_ids_list, param_id3);

        Ident* fn_compare_id = (Ident*)malloc(sizeof(Ident));
        fn_compare_id->class = str_init_const("ifj16");
        fn_compare_id->name = str_init_const("compare");
        Symbol* fn_compare = context_add_function(current_context, KW_INT, fn_compare_id);
        str_dispose(fn_compare_id->class);
        str_dispose(fn_compare_id->name);
        free(fn_compare_id);
        Ident* id4 = (Ident*)malloc(sizeof(Ident));
        id4->class = NULL;
        id4->name = str_init_const("s1");
        ListItemData param_type4, param_id4 = {
            .id = id4
        };
        param_type4.var_type = VT_STRING;
        list_insert_last(fn_compare->data.fn->params_types_list, param_type4);
        list_insert_last(fn_compare->data.fn->params_ids_list, param_id4);
        Ident* id5 = (Ident*)malloc(sizeof(Ident));
        id5->class = NULL;
        id5->name = str_init_const("s2");
        ListItemData param_type5, param_id5 = {
            .id = id5
        };
        param_type5.var_type = VT_STRING;
        list_insert_last(fn_compare->data.fn->params_types_list, param_type5);
        list_insert_last(fn_compare->data.fn->params_ids_list, param_id5);

        Ident* fn_find_id = (Ident*)malloc(sizeof(Ident));
        fn_find_id->class = str_init_const("ifj16");
        fn_find_id->name = str_init_const("find");
        Symbol* fn_find = context_add_function(current_context, KW_INT, fn_find_id);
        str_dispose(fn_find_id->class);
        str_dispose(fn_find_id->name);
        free(fn_find_id);
        Ident* id6 = (Ident*)malloc(sizeof(Ident));
        id6->class = NULL;
        id6->name = str_init_const("s");
        ListItemData param_type6, param_id6 = {
            .id = id6
        };
        param_type6.var_type = VT_STRING;
        list_insert_last(fn_find->data.fn->params_types_list, param_type6);
        list_insert_last(fn_find->data.fn->params_ids_list, param_id6);
        Ident* id7 = (Ident*)malloc(sizeof(Ident));
        id7->class = NULL;
        id7->name = str_init_const("search");
        ListItemData param_type7, param_id7 = {
            .id = id7
        };
        param_type7.var_type = VT_STRING;
        list_insert_last(fn_find->data.fn->params_types_list, param_type7);
        list_insert_last(fn_find->data.fn->params_ids_list, param_id7);

        Ident* fn_sort_id = (Ident*)malloc(sizeof(Ident));
        fn_sort_id->class = str_init_const("ifj16");
        fn_sort_id->name = str_init_const("sort");
        Symbol* fn_sort = context_add_function(current_context, KW_STRING, fn_sort_id);
        str_dispose(fn_sort_id->class);
        str_dispose(fn_sort_id->name);
        free(fn_sort_id);
        Ident* id8 = (Ident*)malloc(sizeof(Ident));
        id8->class = NULL;
        id8->name = str_init_const("s");
        ListItemData param_type8, param_id8 = {
            .id = id8
        };
        param_type8.var_type = VT_STRING;
        list_insert_last(fn_sort->data.fn->params_types_list, param_type8);
        list_insert_last(fn_sort->data.fn->params_ids_list, param_id8);

        Ident* fn_print_id = (Ident*)malloc(sizeof(Ident));
        fn_print_id->class = str_init_const("ifj16");
        fn_print_id->name = str_init_const("print");
        Symbol* fn_print = context_add_function(current_context, KW_STRING, fn_print_id);
        str_dispose(fn_print_id->class);
        str_dispose(fn_print_id->name);
        free(fn_print_id);
        Ident* id9 = (Ident*)malloc(sizeof(Ident));
        id9->class = NULL;
        id9->name = str_init_const("s");
        ListItemData param_type9, param_id9 = {
            .id = id9
        };
        param_type9.var_type = VT_STRING;
        list_insert_last(fn_print->data.fn->params_types_list, param_type9);
        list_insert_last(fn_print->data.fn->params_ids_list, param_id9);
}
