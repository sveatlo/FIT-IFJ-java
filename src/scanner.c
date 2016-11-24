#include <stdlib.h>
#include "scanner.h"
#include "scanner_token.h"
#include "string.h"

//line of file
int line;

char* name_of_file;

ScannerToken key_or_ident (ScannerToken *token) {
    if (str_cmp_const(token->data->id->name, "boolean") == 0) {
        token->type = STT_KEYWORD_TYPE;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_BOOLEAN;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "break") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_BREAK;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "class") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_CLASS;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "continue") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_CONTINUE;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "do") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_DO;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "double") == 0) {
        token->type = STT_KEYWORD_TYPE;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_DOUBLE;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "else") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_ELSE;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "false") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_FALSE;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "for") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_FOR;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "if") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_IF;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "int") == 0) {
        token->type = STT_KEYWORD_TYPE;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_INT;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "return") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_RETURN;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "String") == 0) {
        token->type = STT_KEYWORD_TYPE;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_STRING;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "static") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_STATIC;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "true") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_TRUE;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "void") == 0) {
        token->type = STT_KEYWORD_TYPE;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_VOID;
        return *token;
    } else if (str_cmp_const(token->data->id->name, "while") == 0) {
        token->type = STT_KEYWORD;
        str_dispose(token->data->id->name);
        free(token->data->id);
        token->data->keyword_type = KW_WHILE;
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
    if (token == NULL) {
        set_error(ERR_ALLOCATION);
        return NULL;
    }
    //initialize string
    //string* token_data_str = str_init();
    //if lexical error => kind of free

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
                    token->data = (ScannerTokenData*)malloc(sizeof(ScannerTokenData));
                    if (token->data == NULL) {
                        set_error(ERR_ALLOCATION);
                        return NULL;
                    }
                    token->data->id = (Ident*) malloc(sizeof(Ident));
                    if (token->data->id == NULL) {
                        set_error(ERR_ALLOCATION);
                        free(token->data);
                        return NULL;
                    }
                    token->data->id->name = str_init();
                    if (token->data->id->name == NULL) {
                        set_error(ERR_ALLOCATION);
                        free(token->data->id);
                        free(token->data);
                        return NULL;
                    }
                    str_append(token->data->id->name, c);
                    current_state = SS_KEYWORD_IDENT;
                } else if ( c == '_' || c == '$') {
                    //ident
                    token->data = (ScannerTokenData*)malloc(sizeof(ScannerTokenData));
                    if (token->data == NULL) {
                        set_error(ERR_ALLOCATION);
                        return NULL;
                    }
                    token->data->id = (Ident*) malloc(sizeof(Ident));
                    if (token->data->id == NULL) {
                        set_error(ERR_ALLOCATION);
                        free(token->data);
                        return NULL;
                    }
                    token->data->id->name = str_init();
                    if (token->data->id->name == NULL) {
                        set_error(ERR_ALLOCATION);
                        free(token->data->id);
                        free(token->data);
                        return NULL;
                    }
                    str_append(token->data->id->name, c);
                    current_state = SS_IDENT;
                } else if (isdigit(c)) {
                    token->data = (ScannerTokenData*)malloc(sizeof(ScannerTokenData));
                    if (token->data == NULL) {
                        set_error(ERR_ALLOCATION);
                        return NULL;
                    }
                    token->data->str = str_init();
                    if (token->data->str == NULL) {
                        set_error(ERR_ALLOCATION);
                        free(token->data);
                        return NULL;
                    }
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER;
                } else if (c == '=') {
                    token->type = STT_EQUALS;
                    return token;
                } else if (c == ',') {
                    token->type = STT_COMMA;
                    return token;
                }  else if (c == ';') {
                    token->type = STT_SEMICOLON;
                    return token;
                } else if (c == '"') {
                    token->data = (ScannerTokenData*)malloc(sizeof(ScannerTokenData));
                    token->data->str = str_init();
                    if (token->data->str == NULL) {
                        set_error(ERR_ALLOCATION);
                        free(token->data);
                        return NULL;
                    }
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
                    current_state = SS_NEGATE;
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
                            token->type = STT_IDENT;
                            token->data->id->class = str_init();
                            if (token->data->id->class == NULL) {
                                set_error(ERR_ALLOCATION);
                                str_dispose(token->data->id->name);
                                free(token->data->id);
                                free(token->data);
                                return NULL;
                            }
                            //cut content from token->data->id->name to token->data->id->class and ignore .
                            str_copy_string(token->data->id->class, token->data->id->name);
                            str_clear(token->data->id->name);
                            current_state = SS_CLASS_AND_FUNCTION_1;
                        } else {
                            // next token
                            key_or_ident(token);
                            ungetc(c, f);
                            return token;
                        }
                    } else {
                        //not space and alnum => append
                        str_append(token->data->id->name, c);
                    }
                } else {
                    //is space => end of identificator or keyword => parse
                    key_or_ident(token);
                    return token;
                }

                break;

            case SS_CLASS_AND_FUNCTION_1:
                // id is identificator with .
                // append alnum chars for 2. identificator
                if (isalpha(c)) {
                    str_append(token->data->id->name, c);
                    current_state = SS_CLASS_AND_FUNCTION_2;
                } else {
                    str_dispose(token->data->id->class);
                    str_dispose(token->data->id->name);
                    free(token->data->id);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;


            case SS_CLASS_AND_FUNCTION_2:
                // id is identificator with .
                // append alnum chars for 2. identificator
                if (isalnum(c)) {
                    str_append(token->data->id->name, c);
                    current_state = SS_CLASS_AND_FUNCTION_2;
                } else {
                    ungetc(c, f);
                    token->type = STT_IDENT;
                    return token;
                }

                break;

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
                } else {
                    // next token
                    int i = atoi(str_get_str(token->data->str));
                    str_dispose(token->data->str);
                    token->data->i = i;
                    token->type = STT_INT;
                    ungetc(c, f);
                    return token;
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
                    str_dispose(token->data->str);
                    free(token->data);
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
                    str_dispose(token->data->str);
                    free(token->data);
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
                } else {
                    // next token
                    ungetc(c, f);
                    double number = atof(str_get_str(token->data->str));
                    str_dispose(token->data->str);
                    token->data->d = number;
                    token->type = STT_DOUBLE;
                    return token;
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
                     str_dispose(token->data->str);
                     free(token->data);
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
                } else {
                    // next token
                    ungetc(c, f);
                    double number = atof(str_get_str(token->data->str));
                    str_dispose(token->data->str);
                    token->data->d = number;
                    token->type = STT_DOUBLE;
                    return token;
                }

                break;

            case SS_IDENT:
                //is identificator
                //load whole string
                if (!isspace(c)) {
                    //not space => append
                    str_append(token->data->id->name, c);
                    current_state = SS_IDENT;
                } else {
                    //next token
                    ungetc(c, f);
                    token->type = STT_IDENT;
                    return token;
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

            case SS_EQUAL:
                if (c == '=') {
                    token->type = STT_LOGIC_EQUAL;
                    return token;
                } else {
                    ungetc(c, f);
                    token->type = STT_EQUALS;
                    return token;
                }

                break;

            case SS_NEGATE:
                if (c == '=') {
                    token->type = STT_LOGIC_NOT_EQUAL;
                    return token;
                } else {
                    ungetc(c, f);
                    token->type = STT_NEGATE;
                    return token;
                }

                break;

            case SS_COMMENT_LINE:
                if (c == '\n') {
                    // DO NOT RETURN COMMENTS
                    current_state = SS_EMPTY;

                    //end of line = end of comment
                    // token->type = STT_COMMENT;
                    // return token;
                } else {
                    //stay in state
                    current_state = SS_COMMENT_LINE;
                }

                break;

            case SS_COMMENT_BLOCK:
                if (c == '*') {
                    //could be end of block comment
                    current_state = SS_COMMENT_BLOCK_END;
                } else if (c == EOF) {
                    // end of file = end of comment
                    ungetc(c, f);
                    set_error_lex(ERR_LEX,name_of_file,line);
                    token->type = STT_EMPTY;
                    return token;
                } else {
                    //stay in state
                    current_state = SS_COMMENT_BLOCK;
                }

                break;

            case SS_COMMENT_BLOCK_END:
                if (c == '/') {
                    // DO NOT RETURN COMMENTS
                    current_state = SS_EMPTY;

                    // token->type = STT_COMMENT;
                    // return token;
                } else if (c == EOF) {
                    // end of file = end of comment
                    ungetc(c, f);
                    set_error_lex(ERR_LEX,name_of_file,line);
                    token->type = STT_EMPTY;
                    return token;
                } else {
                    //not end => go back to block comment state
                    current_state = SS_COMMENT_BLOCK;
                }

                break;

            case SS_LEX_ERROR:
                if (c == '\n') {
                    line--;
                }
                set_error_lex(ERR_LEX,name_of_file,line);
                if (c == '\n') {
                    line++;
                }
                while (c == isalnum(c)) {
                    c = getc(f);
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
