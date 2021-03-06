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
#include "scanner_token.h"
#include "error.h"

ScannerToken* token_init () {
    ScannerToken *token = (ScannerToken *) malloc(sizeof(ScannerToken));
    if (token == NULL) {
        set_error(ERR_INTERPRET);
        return NULL;
    }
    token->type = STT_EMPTY;
    return token;
}

void token_delete (ScannerToken *token) {
    if(token == NULL) {
        return;
    }

    if (token->type == STT_INT) {
        free(token->data);
    } else if (token->type == STT_DOUBLE) {
        free(token->data);
    } else if (token->type == STT_KEYWORD) {
        free(token->data);
    } else if (token->type == STT_KEYWORD_TYPE) {
        free(token->data);
    } else if (token->type == STT_IDENT) {
        if(token->data->id->class != NULL) {
            str_dispose(token->data->id->class);
        }
        str_dispose(token->data->id->name);
        free(token->data->id);
        free(token->data);
    } else if (token->type == STT_STRING) {
        str_dispose(token->data->str);
        free(token->data);
    }
    free(token);
}


char* token_to_string (ScannerToken *token) {
    switch (token->type) {
    case STT_EMPTY:
        return "STT_EMPTY";
    case STT_COMMENT:
        return "STT_COMMENT";
    case STT_IDENT:
        return "STT_IDENT";
    case STT_KEYWORD:
        return "STT_KEYWORD";
    case STT_KEYWORD_TYPE:
        return "STT_KEYWORD_TYPE";
    case STT_DOUBLE:
        return "STT_DOUBLE";
    case STT_STRING:
        return "STT_STRING";
    case STT_INT:
        return "STT_INT";
    case STT_COMMA:
        return "STT_COMMA";
    case STT_SEMICOLON:
        return "STT_SEMICOLON";
    case STT_PLUS:
        return "STT_PLUS";
    case STT_MINUS:
        return "STT_MINUS";
    case STT_MULTIPLY:
        return "STT_MULTIPLY";
    case STT_DIVIDE:
        return "STT_DIVIDE";
    case STT_EQUALS:
        return "STT_EQUALS";
    case STT_LOGIC_EQUAL:
        return "STT_LOGIC_EQUAL";
    case STT_LOGIC_NOT_EQUAL:
        return "STT_LOGIC_NOT_EQUAL";
    case STT_LESS:
        return "STT_LESS";
    case STT_LESS_EQUALS:
        return "STT_LESS_EQUALS";
    case STT_GREATER:
        return "STT_GREATER";
    case STT_GREATER_EQUALS:
        return "STT_GREATER_EQUALS";
    case STT_AND:
        return "STT_AND";
    case STT_OR:
        return "STT_OR";
    case STT_NEGATE:
        return "STT_NEGATE";
    case STT_LEFT_PARENTHESE:
        return "STT_LEFT_PARENTHESE";
    case STT_RIGHT_PARENTHESE:
        return "STT_RIGHT_PARENTHESE";
    case STT_LEFT_BRACE:
        return "STT_LEFT_BRACE";
    case STT_RIGHT_BRACE:
        return "STT_RIGHT_BRACE";
    case STT_EOF:
        return "STT_EOF";
    default:
        return "unknown";
    }
}
