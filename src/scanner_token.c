#include <stdlib.h>
#include "scanner_token.h"
#include "error.h"

ScannerToken* token_init () {
    ScannerToken *token = (ScannerToken *) malloc(sizeof(ScannerToken));
    if (token == NULL) {
        set_error(ERR_ALLOCATION);
        return NULL;
    }
    token->type = STT_EMPTY;
    // token->data = malloc(sizeof(ScannerTokenData));
    // token->data->id = (Ident*) malloc(sizeof(Ident));
    // token->data->id->class = str_init();
    // token->data->id->function = str_init();
    // token->data->str = str_init();

    return token;
}

void token_delete (ScannerToken *token) {
    // str_free(token->data->str);
    // str_free(token->data->id->class);
    // str_free(token->data->id->function);
    // free(token->data->id);
    // free(token->data);
    if (token->type == STT_INT) {
        free(token->data);
    } else if (token->type == STT_DOUBLE) {
        free(token->data);
    } else if (token->type == STT_KEYWORD) {
        free(token->data);
    } else if (token->type == STT_IDENT) {
        if(token->data->id->class != NULL) {
            str_free(token->data->id->class);
        }
        str_free(token->data->id->name);
        free(token->data->id);
        free(token->data);
    } else if (token->type == STT_STRING) {
        str_free(token->data->str);
        free(token->data);
    }
    free(token);
}


char* token_to_string (ScannerToken *token) {
    switch (token->type) {
    case STT_EMPTY:
        return "<STT_EMPTY>";
    case STT_COMMENT:
        return "<STT_COMMENT>";
    case STT_IDENT:
        return "<STT_IDENT>";
    case STT_KEYWORD:
        return "<STT_KEYWORD>";
    case STT_KEYWORD_TYPE:
        return "<STT_KEYWORD_TYPE>";
    case STT_DOUBLE:
        return "<STT_DOUBLE>";
    case STT_STRING:
        return "<STT_STRING>";
    case STT_INT:
        return "<STT_INT>";
    case STT_COMMA:
        return "<STT_COMMA>";
    case STT_SEMICOLON:
        return "<STT_SEMICOLON>";
    case STT_PLUS:
        return "<STT_PLUS>";
    case STT_PLUS_EQUAL:
        return "<STT_PLUS_EQUAL>";
    case STT_MINUS:
        return "<STT_MINUS>";
    case STT_MINUS_EQUAL:
        return "<STT_MINUS_EQUAL>";
    case STT_MULTIPLY:
        return "<STT_MULTIPLY>";
    case STT_DIVIDE:
        return "<STT_DIVIDE>";
    case STT_EQUALS:
        return "<STT_EQUALS>";
    case STT_LOGIC_EQUAL:
        return "<STT_LOGIC_EQUAL>";
    case STT_LOGIC_NOT_EQUAL:
        return "<STT_LOGIC_NOT_EQUAL>";
    case STT_LESS:
        return "<STT_LESS>";
    case STT_LESS_EQUALS:
        return "<STT_LESS_EQUALS>";
    case STT_GREATER:
        return "<STT_GREATER>";
    case STT_GREATER_EQUALS:
        return "<STT_GREATER_EQUALS>";
    case STT_AND:
        return "<STT_AND>";
    case STT_OR:
        return "<STT_OR>";
    case STT_NEGATE:
        return "<STT_NEGATE>";
    case STT_LEFT_PARENTHESE:
        return "<STT_LEFT_PARENTHESE>";
    case STT_RIGHT_PARENTHESE:
        return "<STT_RIGHT_PARENTHESE>";
    case STT_LEFT_BRACE:
        return "<STT_LEFT_BRACE>";
    case STT_RIGHT_BRACE:
        return "<STT_RIGHT_BRACE>";
    case STT_EOF:
        return "<STT_EOF>";
    default:
        return "<unknown>";
    }
}
