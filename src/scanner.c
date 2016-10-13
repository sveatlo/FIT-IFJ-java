#include <stdlib.h>
#include "scanner.h"
#include "scanner_token.h"
#include "string.h"

List *scan_file(FILE *f, List *token_list) {
    while (42) {
        ScannerToken *token = get_next_token(f);
        ListItemData list_item_data = {
            .token = token
        };
        list_insert_last(token_list, list_item_data);

        if (token->type == STT_EMPTY) {
            break;
        }
    }

    return token_list;
}

ScannerToken* get_next_token(FILE *f) {
    //stores current state
    ScannerState current_state = SS_EMPTY;
    //used for getc storage
    int c;
    //initialize token we will return
    ScannerToken *token = token_init();
    //initialize string
    string *str = str_init();

    while (69) {
        //get next char from file
        if(current_state != SS_LEX_ERROR) {
            c = getc(f);
        }

        switch (current_state) {
            //default state => every token starts here
            case SS_EMPTY:
                if (isspace(c)) {
                    //ignore space
                    current_state = SS_EMPTY;
                } else if (isalpha(c)) {
                    //keyword or id
                    str = str_append(c);
                    current_state = SS_ALNUM;
                } else if ((c == '_') || (c == '$')) {
                    str = str_append(c);
                    current_state = SS_IDENT;
                } else if (isdigit(c)) {
                    str = str_append(c);
                    current_state = SS_NUMBER;
                } else if (c == '"') {
                    str = str_append(c);
                    current_state = SS_STRING;
                } else if (c == '(') {
                    token->type = STT_LEFT_PARENTHESE;
                    return token;
                } else if (c == ')') {
                    token->type = STT_RIGHT_PARENTHESE;
                    return token;
                } else if (c == '{') {
                    token->type = STT_LEFT_BRACE;
                    return token;
                } else if (c == '}') {
                    token->type = STT_RIGHT_BRACE;
                    return token;
                } else if (c == '+') {
                    token->type = STT_PLUS;
                    return token;
                } else if (c == '-') {
                    token->type = STT_MINUS;
                    return token;
                } else if (c == '*') {
                    token->type = STT_MULTIPLY;
                    return token;
                } else if (c == '&') {
                    current_state = SS_AND;
                } else if (c == '/') {
                    current_state = SS_SLASH;
                } else {
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_ALNUM:
                //is keyword or identificator
                //load whole string
                if (!isspace(c)) {
                    //not space => append
                } else {
                    //is space => end of identificator => parse
                }

                break;

            case SS_NUMBER:
                //is number
                //load whole string
                if (isdigit(c)) {
                    //is digit => append
                    str = str_append(c);
                    current_state = SS_NUMBER;
                } else if ((c == 'e') || (c == 'E')) {
                    //is exponent => append
                    str = str_append(c);
                    current_state = SS_DOUBLE_EX_1;
                } else if (c == '.') {
                    //is decimal mark => append
                    str = str_append(c);
                    current_state = SS_DOUBLE_DEC_1;
                } else if (isspace(c)) {
                    //is space => integer number
                    token->data = atoi(str);
                    token->type = STT_INT;
                    return token;
                } else {
                    //lex error
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_DOUBLE_EX_1:
                //is double with exponent
                // posible chars
                if ((c == '-') || (c == '+')) {
                    // +,- => must be append number
                    str = str_append(c);
                    current_state = SS_DOUBLE_EX_2;
                } else if (isdigit(c)) {
                    // is digit => append
                    str = str_append(c);
                    current_state = SS_DOUBLE_EX_3;
                } else {
                    //is space => error
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_DOUBLE_EX_2:
                //is double with exponent with +,-
                // must be number
                if (isdigit(c)) {
                    // is digit => append
                    str = str_append(c);
                    current_state = SS_DOUBLE_EX_3;
                } else {
                    //is something else => error
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_DOUBLE_EX_3:
                //is double => must be digit or space
                // is number
                if (isdigit(c)) {
                    // is digit => append
                    str = str_append(c);
                    current_state = SS_DOUBLE_EX_3;
                } else if (isspace(c)) {
                    // is space
                    token->data = atof(str);
                    token->type = STT_DOUBLE;
                    return token;
                } else {
                    //is something else => error
                    current_state = SS_LEX_ERROR;
                }

                break;

              case SS_DOUBLE_DEC_1:
                 //is double => must be only digit
                 // is number
                 if (isdigit(c)) {
                     // is digit => append
                     str = str_append(c);
                     current_state = SS_DOUBLE_DEC_2;
                 } else {
                     //is something else => error
                     current_state = SS_LEX_ERROR;
                 }

                 break;

             case SS_DOUBLE_DEC_2:
                //is double => must be digit or exponent
                // is number
                if (isdigit(c)) {
                    // is digit => append
                    str = str_append(c);
                    current_state = SS_DOUBLE_DEC_2;
                } else if ((c == 'e') || (c == 'E')) {
                    // is exponent
                    str = str_append(c);
                    current_state = SS_DOUBLE_EX_1;
                } else if (isspace(c)){
                    // is space
                    token->data = atof(str);
                    token->type = STT_DOUBLE;
                    return token;
                } else {
                    //is something else => error
                    current_state = SS_LEX_ERROR;
                }

                break;


            case SS_IDENT:
                //is identificator
                //load whole string
                if (!isspace(c)) {
                    //not space => append
                    str = str_append(c);
                    current_state = SS_IDENT;
                } else {
                    //is space => end of identificator => parse
                    token->data = str;
                    token->type = STT_IDENT;
                    return token;
                }

                break;

            case SS_STRING:
                //is string
                //load whole read char !='"'
                if (c != '"') {
                    //not '"' => append
                    str = str_append(c);
                    current_state = SS_STRING;
                } else {
                    //is '"' => end of string => parse
                    token->data = str;
                    token->type = STT_STRING;
                    return token;
                }

                break;


            case SS_AND:
                if (c == '&') {
                    token->type = STT_AND;
                    return token;
                } else {
                    //lex error
                    current_state = SS_LEX_ERROR;
                }
                break;

            case SS_SLASH:
                //after SS_EMPTY we got a slash char => this could be a comment
                if (c == '/') {
                    //definitely inline comment
                    current_state = SS_COMMENT_LINE;
                } else if (c == '*') {
                    //definitely block comment
                    current_state = SS_COMMENT_BLOCK;
                } else {
                    token->type = STT_DIVIDE;
                    return token;
                }
                break;

            case SS_COMMENT_LINE:
                if (c == '\n'){
                    //end of line = end of comment
                    token->type = STT_COMMENT;
                    return token;
                } else {
                    //stay in state
                    current_state = SS_COMMENT_LINE;
                }

                break;

            case SS_COMMENT_BLOCK:
                if (c == '*'){
                    //could be end of block comment
                    current_state = SS_COMMENT_BLOCK_END;
                } else {
                    //stay in state
                    current_state = SS_COMMENT_BLOCK;
                }

                break;

            case SS_COMMENT_BLOCK_END:
                if (c == '/') {
                    token->type = STT_COMMENT;
                    return token;
                } else {
                    //not end => go back to block comment state
                    current_state = SS_COMMENT_BLOCK;
                }

                break;

            case SS_LEX_ERROR:
                ungetc(c, f);
                free(str);
            default:
                set_error(ERR_LEX);
                token->type = STT_EMPTY;
                return token;
        }
    }
}
