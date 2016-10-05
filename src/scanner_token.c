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
        return "<empty token>";
    case STT_COMMENT:
        return "<comment (inline or block)>";
    case STT_IDENT:
        return "<identificator>";
    case STT_KEYWORD:
        return "<one of keywords>";
    case STT_PLUS:
        return "<`+`>";
    case STT_MINUS:
        return "<`-`>";
    case STT_MULTIPLY:
        return "<`*`>";
    case STT_DIVIDE:
        return "</>";
    case STT_EQUALS:
        return "<=>";
    case STT_LESS:
        return "<<>";
    case STT_LESS_EQUALS:
        return "<<=>";
    case STT_GREATER:
        return "<>>";
    case STT_GREATER_EQUALS:
        return "<>=>";
    case STT_AND:
        return "<&&>";
    case STT_OR:
        return "<||>";
    case STT_LEFT_PARENTHESE:
        return "<(>";
    case STT_RIGHT_PARENTHESE:
        return "<)>";
    case STT_LEFT_BRACE:
        return "<{>";
    case STT_RIGHT_BRACE:
        return "<}>";
    default:
        return "<unknown>";
    }
}
