#include <stdlib.h>
#include "scanner_token.h"

ScannerToken* token_init () {
    ScannerToken *token = (ScannerToken *) malloc(sizeof(ScannerToken));
    token->type = STT_EMPTY;

    return token;
}

void token_delete (ScannerToken *token) {
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
    case STT_PLUS:
        return "<STT_PLUS>";
    case STT_MINUS:
        return "<STT_MINUS>";
    case STT_MULTIPLY:
        return "<STT_MULTIPLY>";
    case STT_DIVIDE:
        return "<STT_DIVIDE>";
    case STT_EQUALS:
        return "<STT_EQUALS>";
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
    case STT_LEFT_PARENTHESE:
        return "<STT_LEFT_PARENTHESE>";
    case STT_RIGHT_PARENTHESE:
        return "<STT_RIGHT_PARENTHESE>";
    case STT_LEFT_BRACE:
        return "<STT_LEFT_BRACE>";
    case STT_RIGHT_BRACE:
        return "<STT_RIGHT_BRACE>";
    default:
        return "<unknown>";
    }
}
