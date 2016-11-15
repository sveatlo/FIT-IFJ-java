#include <stdio.h>
#include "error.h"
#include "list.h"
#include "parser.h"
#include "scanner_token.h"

List* token_list;
ScannerToken* current_token;

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

void parse(List* _token_list) {
    list_activate_first(_token_list);
    token_list = _token_list;
    current_token = token_list->active->data.token;

    //first thing in file should be class (or several)
    class_list_rule();
}


void class_list_rule() {
    switch (current_token->type) {
        case STT_KEYWORD:
        {
            if(current_token->data->keyword_type != KW_CLASS) return set_error(ERR_SYNTAX);
            next_token();
            class_rule();

            break;
        }
        default:
        {
            set_error(ERR_SYNTAX);
            break;
        }
    }
}

void class_rule() {
    if(current_token->type != STT_IDENT) return set_error(ERR_SYNTAX);
    if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
    if(next_token()->type != STT_RIGHT_BRACE) {
        // parse inside of the class
        class_members_rule();
        if(next_token()->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
    } else {
        // empty class => ok
    }
}

void class_members_rule() {
    class_member_rule();
    if(next_token()->type != STT_RIGHT_BRACE) {
        class_members_rule();
    } else {
        prev_token();
    }
}


void class_member_rule() {
    if(current_token->type != STT_KEYWORD || current_token->data->keyword_type != KW_STATIC) return set_error(ERR_SYNTAX);
    if(next_token()->type != STT_KEYWORD_TYPE) return set_error(ERR_SYNTAX);
    if(next_token()->type != STT_IDENT) return set_error(ERR_SYNTAX);
    next_token();
    if(current_token->type == STT_SEMICOLON) {
        //syntax ok
        return;
    } else if(current_token->type == STT_EQUALS) {
        // TODO: parse expression
    } else  if(current_token->type == STT_LEFT_PARENTHESE) {
        next_token();
        params_list_rule();
        if(next_token()->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        next_token();
        stat_list_rule();
        if(next_token()->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
    }
}

void params_list_rule() {
    if(current_token->type == STT_RIGHT_PARENTHESE) {
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

void definition_rule() {
    if(current_token->type != STT_KEYWORD_TYPE) return set_error(ERR_SYNTAX);
    if(next_token()->type != STT_IDENT) return set_error(ERR_SYNTAX);
}

void stat_list_rule() {
    stat_rule();
}

void stat_rule() {
    if(current_token->type == STT_RIGHT_BRACE) {
        prev_token();
        return;
    }

    if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_IF) {
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        next_token();
        bool_expression_rule();
        if(next_token()->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
    } else if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_WHILE) {
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        next_token();
        bool_expression_rule();
        if(next_token()->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);

        if(next_token()->type != STT_LEFT_PARENTHESE) return set_error(ERR_SYNTAX);
        next_token();
        stat_list_rule();
        if(next_token()->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);
    } else if(current_token->type == STT_IDENT) {
        if(next_token()->type == STT_LEFT_BRACE) {
            next_token();
            params_list_rule();
            if(next_token()->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
        } else if(next_token()->type == STT_EQUALS) {
            next_token();
            expression_rule();
        } else {
            return set_error(ERR_SYNTAX);
        }
    } else if(current_token->type == STT_KEYWORD_TYPE) {
        definition_rule();
        next_token();
        if(current_token->type == STT_SEMICOLON) {
            // this was a local variable definition
            // syntax ok => return
            return;
        } else if (current_token->type == STT_EQUALS) {
            // this is definition as well as assignment
            // parse expression, which will be assigned
            next_token();
            expression_rule();
            if(next_token()->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);
        }
    } else {
        printf(" stat_rule: %s\n", token_to_string(current_token));
        return set_error(ERR_SYNTAX);
    }
}

void bool_expression_rule() {

}

void expression_rule() {

}
