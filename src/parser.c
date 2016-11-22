#include <stdio.h>
#include "context.h"
#include "error.h"
#include "expression.h"
#include "ial.h"
#include "list.h"
#include "parser.h"
#include "scanner_token.h"

List* token_list;
ScannerToken* current_token;
Context* main_context;
Context* current_context;

// sets next token as current_token
static inline ScannerToken* next_token() {
    list_activate_next(token_list);
    current_token = token_list->active->data.token;

    return current_token;
}

// sets previous token as current_token
static inline ScannerToken* prev_token() {
    list_activate_prev(token_list);
    current_token = token_list->active->data.token;

    return current_token;
}

// sets active item as current_token
// used when token_list is modified in other modules
static inline ScannerToken* refresh_current_token() {
    current_token = token_list->active->data.token;

    return current_token;
}

void parse(List* _token_list) {
    //prepare
    list_activate_first(_token_list);
    token_list = _token_list;
    current_token = token_list->active->data.token;
    main_context = context_init();
    current_context = main_context;


    //first thing in file should be class (or several)
    class_list_rule();
}


void class_list_rule() {
    while (current_token->type != STT_EOF && !get_error()->type) {
        switch (current_token->type) {
            case STT_KEYWORD:
            {
                if(current_token->data->keyword_type != KW_CLASS) return set_error(ERR_SYNTAX);
                next_token();
                class_rule();
                next_token();

                break;
            }
            default:
            {
                set_error(ERR_SYNTAX);
                return;
                // break;
            }
        }
    }
}

//starts @ STT_IDENT
//finish @ STT_RIGHT_BRACE
void class_rule() {
    if(current_token->type != STT_IDENT) return set_error(ERR_SYNTAX);
    // if(table_find_symbol(main_context->symbol_table, current_token->data->id->name) != NULL) {
    //     set_error(ERR_SEMANTIC);
    //     return;
    // }
    // table_insert_class(main_context->symbol_table, current_token->data->id->name);

    if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
    if(next_token()->type != STT_RIGHT_BRACE) {
        // parse inside of the class
        class_members_rule();
        if(next_token()->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
    } else {
        // empty class => ok
    }
}

//starts @ STT_KEYWORD
//finish @ STT_RIGHT_BRACE || STT_SEMICOLON
void class_members_rule() {
    if(current_token->type == STT_RIGHT_BRACE || get_error()->type) {
        prev_token();
        return;
    }
    class_member_rule();
    class_members_rule();
}


//starts @ STT_KEYWORD (static)
//finish @ STT_RIGHT_BRACE or STT_SEMICOLON
void class_member_rule() {
    if(current_token->type != STT_KEYWORD || current_token->data->keyword_type != KW_STATIC) return set_error(ERR_SYNTAX);
    if(next_token()->type != STT_KEYWORD_TYPE) return set_error(ERR_SYNTAX);
    if(next_token()->type != STT_IDENT) return set_error(ERR_SYNTAX);
    next_token();
    if(current_token->type == STT_SEMICOLON) {
        //syntax ok
    } else if(current_token->type == STT_EQUALS) {
        next_token();
        expression_rule();
    } else  if(current_token->type == STT_LEFT_PARENTHESE) {
        next_token();
        params_list_rule();
        if(next_token()->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        next_token();
        stat_list_rule();
        if(current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
    }
    next_token();
}

//starts @ STT_KEYWORD_TYPE || STT_RIGHT_PARENTHESE
//finish @ STT_RIGHT_BRACE or semicolon
void params_list_rule() {
    if(current_token->type == STT_RIGHT_PARENTHESE || get_error()->type) {
        prev_token();
        return;
    }

    definition_rule();

    if(next_token()->type == STT_COMMA) {
        next_token();
        params_list_rule();
    } else {
        prev_token();
    }
}

//starts @ STT_KEYWORD_TYPE
//finish @ STT_IDENT
void definition_rule() {
    if(current_token->type != STT_KEYWORD_TYPE) return set_error(ERR_SYNTAX);
    if(next_token()->type != STT_IDENT) return set_error(ERR_SYNTAX);
}


//starts @ STT_KEYWORD_TYPE || STT_KEYWORD_TYPE || STT_IDENT
//finish @ STT_RIGHT_BRACE || STT_RIGHT_PARENTHESE || STT_SEMICOLON
void stat_list_rule() {
    // handles empty stat
    // eg. empty if ("if () {<nothing here>}")
    if(current_token->type == STT_RIGHT_BRACE || get_error()->type) {
        return;
    }
    stat_rule();
    stat_list_rule();
}

//starts @ STT_KEYWORD_TYPE || STT_KEYWORD_TYPE || STT_IDENT
//finish @ STT_RIGHT_BRACE || STT_RIGHT_PARENTHESE || STT_SEMICOLON
void stat_rule() {
    // IF
    if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_IF) {
        //(
        if(next_token()->type != STT_LEFT_PARENTHESE) return set_error(ERR_SYNTAX);
        //boolean expression in condition
        next_token();
        bool_expression_rule();
        // )
        if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);

        // {
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        //statements inside IF
        // TODO: no definition inside if
        next_token();
        stat_list_rule();
        // {
        if(current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
    } else if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_WHILE) { // WHILE
        //(
        if(next_token()->type != STT_LEFT_PARENTHESE) return set_error(ERR_SYNTAX);
        //boolean expression in condition
        next_token();
        bool_expression_rule();
        // )
        if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);

        // {
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        //statements inside IF
        // TODO: no definition inside while
        next_token();
        stat_list_rule();
        // {
        if(current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
    } else if(current_token->type == STT_IDENT) {
        next_token();
        if(current_token->type == STT_LEFT_PARENTHESE) {
            next_token();
            params_list_rule();
            if(next_token()->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);
            if(next_token()->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);
        } else if(current_token->type == STT_EQUALS) {
            next_token();
            expression_rule();
            if(current_token->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);
        } else {
            return set_error(ERR_SYNTAX);
        }
    } else if(current_token->type == STT_KEYWORD_TYPE) {
        definition_rule();
        next_token();
        if(current_token->type == STT_SEMICOLON) {
            // syntax OK
        } else if(current_token->type == STT_EQUALS) {
            next_token();
            expression_rule();
            if(current_token->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);
        } else {
            set_error(ERR_SYNTAX);
        }
    } else {
        return set_error(ERR_SYNTAX);
    }
    next_token();
}

void bool_expression_rule() {
    while(current_token->type != STT_RIGHT_PARENTHESE) {
        next_token();
    }
}

void expression_rule() {
    // parse_expression_tokens(token_list);
    // refresh_current_token();
    while(current_token->type != STT_SEMICOLON) {
        next_token();
    }
}
