#include <stdlib.h>
#include "scanner.h"
#include "scanner_token.h"
#include "string.h"

//line of file
int line;

int compare_symbols(char c) {
    char symbols[14] = {';', '=', '+', '-', '*', '/', '!', '"', '|', '&', '<', '>', '(', ')'};
    for (int i = 0; i < 14; i++) {
        if (c == symbols[i]) {
            return 1;
        }
    }
    return 0;
}

ScannerToken key_or_ident (ScannerToken *token) {
    if (str_cmp_const(token->data->str, "boolean") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "break") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "class") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "continue") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "do") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "double") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "else") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "false") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "for") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "if") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "int") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "return") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "String") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "static") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "true") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "void") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else if (str_cmp_const(token->data->str, "while") == 0) {
        token->type = STT_KEYWORD;
        return *token;
    } else {
        token->type = STT_IDENT;
        return *token;
    }
}

List *scan_file(FILE *f, List *token_list) {
    while (42) {
        ScannerToken *token = get_next_token(f);
        ListItemData list_item_data = {
            .token = token
        };
        list_insert_last(token_list, list_item_data);

        if (token->type == STT_EOF) {
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
    //string* token_data_str = str_init();

    while (69) {
        //get next char from file
        if (current_state != SS_LEX_ERROR) {
            c = getc(f);
            if (c == '\n') {
                line++;
            }
        }

        switch (current_state) {
            //default state => every token starts here
            case SS_EMPTY:
                if (isspace(c)) {
                    //ignore space
                    current_state = SS_EMPTY;
                } else if (isalpha(c)) {
                    //keyword or ident
                    str_append(token->data->str, c);
                    current_state = SS_KEYWORD_IDENT;
                } else if ( c == '_' || c == '$') {
                    //ident
                    str_append(token->data->str, c);
                    current_state = SS_IDENT;
                } else if (isdigit(c)) {
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER;
                } else if (c == '=') {
                    token->type = STT_EQUALS;
                    return token;
                } else if (c == ';') {
                    token->type = STT_SEMICOLON;
                    return token;
                } else if (c == '"') {
                    str_append(token->data->str, c);
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
                } else if (c == '*') {
                    token->type = STT_MULTIPLY;
                    return token;
                } else if (c == '!') {
                    token->type = STT_NEGATE;
                    return token;
                } else if (c == '+') {
                    current_state = SS_PLUS;
                } else if (c == '-') {
                    current_state = SS_MINUS;
                } else if (c == '&') {
                    current_state = SS_AND;
                } else if (c == '|') {
                    current_state = SS_OR;
                } else if (c == '>') {
                    current_state = SS_GREATER;
                } else if (c == '<') {
                    current_state = SS_LESS;
                } else if (c == '/') {
                    current_state = SS_SLASH;
                } else if (c == EOF) {
                    current_state = SS_EOF;
                } else {
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_KEYWORD_IDENT:
                //is keyword or identificator
                //load whole string
                if (!isspace(c)) {
                    // is not alnum and is identificator without . => compare
                    if (!isalnum(c)) {
                        if (c == '.') {
                            token->type = STT_CLASS_AND_FUNC;
                            //cut content from token->data->str to token->data->id->class and ignore .
                            str_copy_string(token->data->str, token->data->id->class);
                            str_clear(token->data->str);
                            current_state = SS_CLASS_AND_FUNCTION;
                        } else if (compare_symbols(c) == 1) {
                            // next token
                            key_or_ident(token);
                            ungetc(c, f);
                            return token;
                        } else {
                           current_state = SS_LEX_ERROR;
                        }
                    } else {
                        //not space and alnum => append
                        str_append(token->data->str, c);
                    }
                } else {
                    //is space => end of identificator or keyword => parse
                    // key_or_ident(token->data->str);
                    key_or_ident(token);
                    return token;
                }

                break;

            case SS_CLASS_AND_FUNCTION:
                // id is identificator with .
                // append alnum chars for 2. identificator
                if (isalnum(c)) {
                    str_append(token->data->id->function, c);
                    current_state = SS_CLASS_AND_FUNCTION;
                } else if (isspace(c)) {
                    return token;
                } else {
                    current_state = SS_LEX_ERROR;
                }

            case SS_NUMBER:
                //is number
                //load whole string
                if (isdigit(c)) {
                    //is digit => append
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER;
                } else if ((c == 'e') || (c == 'E')) {
                    //is exponent => append
                    str_append(token->data->str, c);
                    current_state = SS_DOUBLE_EX_1;
                } else if (c == '.') {
                    //is decimal mark => append
                    str_append(token->data->str, c);
                    current_state = SS_DOUBLE_DEC_1;
                } else if (isspace(c)) {
                    //is space => integer number
                    token->data->i = atoi(str_get_str(token->data->str));
                    token->type = STT_INT;
                    return token;
                } else if (compare_symbols(c) == 1) {
                    // next token
                    ungetc(c, f);
                    token->data->i = atoi(str_get_str(token->data->str));
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
                    str_append(token->data->str, c);
                    current_state = SS_DOUBLE_EX_2;
                } else if (isdigit(c)) {
                    // is digit => append
                    str_append(token->data->str, c);
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
                    str_append(token->data->str, c);
                    current_state = SS_DOUBLE_EX_3;
                } else {
                    //is something else => error
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_DOUBLE_EX_3:
                //is double => must be digit or next token
                // is number
                if (isdigit(c)) {
                    // is digit => append
                    str_append(token->data->str, c);
                    current_state = SS_DOUBLE_EX_3;
                } else if (isspace(c)) {
                    // is space
                    token->data->d = atof(str_get_str(token->data->str));
                    token->type = STT_DOUBLE;
                    return token;
                } else if (compare_symbols(c) == 1) {
                    // next token
                    ungetc(c, f);
                    token->data->d = atof(str_get_str(token->data->str));
                    token->type = STT_DOUBLE;
                    return token;
                } else {
                    //lex error
                    current_state = SS_LEX_ERROR;
                }

                break;

              case SS_DOUBLE_DEC_1:
                 //is double => must be only digit
                 // is number
                 if (isdigit(c)) {
                     // is digit => append
                     str_append(token->data->str, c);
                     current_state = SS_DOUBLE_DEC_2;
                 } else {
                     //is something else => error
                     current_state = SS_LEX_ERROR;
                 }

                 break;

             case SS_DOUBLE_DEC_2:
                //is double => must be digit or exponent or next token
                // is number
                if (isdigit(c)) {
                    // is digit => append
                    str_append(token->data->str, c);
                    current_state = SS_DOUBLE_DEC_2;
                } else if ((c == 'e') || (c == 'E')) {
                    // is exponent
                    str_append(token->data->str, c);
                    current_state = SS_DOUBLE_EX_1;
                } else if (isspace(c)) {
                    // is space
                    token->data->d = atof(str_get_str(token->data->str));
                    token->type = STT_DOUBLE;
                    return token;
                } else if (compare_symbols(c) == 1) {
                    // next token
                    ungetc(c, f);
                    token->data->d = atof(str_get_str(token->data->str));
                    token->type = STT_DOUBLE;
                    return token;
                } else {
                    //lex error
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_IDENT:
                //is identificator
                //load whole string
                if (!isspace(c)) {
                    //not space => append
                    str_append(token->data->str, c);
                    current_state = SS_IDENT;
                } else if (compare_symbols(c) == 1) {
                    //next token
                    ungetc(c, f);
                    token->type = STT_IDENT;
                    return token;
                } else if (isspace(c)) {
                    //is space => end of identificator => parse
                    // token->data->str = token->data->str;
                    token->type = STT_IDENT;
                    return token;
                } else {
                    //lex error
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_STRING:
                //is string
                //load whole read char !='"'
                if (c != '"') {
                    //not '"' => append
                    str_append(token->data->str, c);
                    current_state = SS_STRING;
                } else {
                    //is '"' => end of string => parse
                    str_append(token->data->str, c);
                    // token->data->str = token->data->str;
                    token->type = STT_STRING;
                    return token;
                }

                break;

            case SS_PLUS:
                //is plus
                if (c == '=') {
                    token->type = STT_PLUS_EQUAL;
                    return token;
                } else {
                    ungetc(c, f);
                    token->type = STT_PLUS;
                    return token;
                }

            case SS_MINUS:
                //is minus
                if (c == '=') {
                    token->type = STT_MINUS_EQUAL;
                    return token;
                } else {
                    ungetc(c, f);
                    token->type = STT_MINUS;
                    return token;
                }



            case SS_AND:
                if (c == '&') {
                    token->type = STT_AND;
                    return token;
                } else {
                    //lex error
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_OR:
                if (c == '|') {
                    token->type = STT_OR;
                    return token;
                } else {
                    //lex error
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_LESS:
                //less equals
                if (c == '=') {
                    token->type = STT_LESS_EQUALS;
                    return token;
                } else {
                    //less
                    ungetc(c, f);
                    token->type = STT_LESS;
                    return token;
                }

                break;

            case SS_GREATER:
                //less greater
                if (c == '=') {
                    token->type = STT_GREATER_EQUALS;
                    return token;
                } else {
                    //less
                    ungetc(c, f);
                    token->type = STT_GREATER;
                    return token;
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
                    ungetc(c, f);
                    return token;
                }

                break;

            case SS_COMMENT_LINE:
                if (c == '\n') {
                    //end of line = end of comment
                    token->type = STT_COMMENT;
                    return token;
                } else {
                    //stay in state
                    current_state = SS_COMMENT_LINE;
                }

                break;

            case SS_COMMENT_BLOCK:
                if (c == '*') {
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
                set_error(ERR_LEX);
                printf("Line error:%d\n",line );

                while (c == isalnum(c)) {
                    c = getc(f);
                    if (c == '\n') {
                        line++;
                    }
                }
                token->type = STT_EMPTY;
                return token;

                break;

            case SS_EOF:
                token->type = STT_EOF;
                return token;

                break;
        }
    }
}
