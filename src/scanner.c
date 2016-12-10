#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <float.h>

#include "scanner.h"
#include "scanner_token.h"
#include "string.h"

//line of file
int line;

char* name_of_file;

void key_or_ident(ScannerToken *token) {
    if ((token->data->id->name->str[0] != '$') && (token->data->id->name->str[0] != '_')) {
        if (str_cmp_const(token->data->id->name, "boolean") == 0) {
            token->type = STT_KEYWORD_TYPE;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_BOOLEAN;
        } else if (str_cmp_const(token->data->id->name, "break") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_BREAK;
        } else if (str_cmp_const(token->data->id->name, "class") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_CLASS;
        } else if (str_cmp_const(token->data->id->name, "continue") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_CONTINUE;
        } else if (str_cmp_const(token->data->id->name, "do") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_DO;
        } else if (str_cmp_const(token->data->id->name, "double") == 0) {
            token->type = STT_KEYWORD_TYPE;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_DOUBLE;
        } else if (str_cmp_const(token->data->id->name, "else") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_ELSE;
        } else if (str_cmp_const(token->data->id->name, "false") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_FALSE;
        } else if (str_cmp_const(token->data->id->name, "for") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_FOR;
        } else if (str_cmp_const(token->data->id->name, "if") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_IF;
        } else if (str_cmp_const(token->data->id->name, "int") == 0) {
            token->type = STT_KEYWORD_TYPE;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_INT;
        } else if (str_cmp_const(token->data->id->name, "return") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_RETURN;
        } else if (str_cmp_const(token->data->id->name, "String") == 0) {
            token->type = STT_KEYWORD_TYPE;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_STRING;
        } else if (str_cmp_const(token->data->id->name, "static") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_STATIC;
        } else if (str_cmp_const(token->data->id->name, "true") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_TRUE;
        } else if (str_cmp_const(token->data->id->name, "void") == 0) {
            token->type = STT_KEYWORD_TYPE;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_VOID;
        } else if (str_cmp_const(token->data->id->name, "while") == 0) {
            token->type = STT_KEYWORD;
            if(token->data->id->class != NULL) {
                str_dispose(token->data->id->class);
            }
            str_dispose(token->data->id->name);
            free(token->data->id);
            token->data->keyword_type = KW_WHILE;
        } else {
            token->type = STT_IDENT;
        }
    } else {
        token->type = STT_IDENT;
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
        set_error(ERR_INTERPRET);
        return NULL;
    }

    String *number = NULL;

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
                } else if ((isalpha(c)) || (c == '_') || (c == '$')) {
                    //keyword or ident
                    token->data = (ScannerTokenData*)malloc(sizeof(ScannerTokenData));
                    if (token->data == NULL) {
                        set_error(ERR_INTERPRET);
                        return NULL;
                    }
                    token->data->id = (Ident*) malloc(sizeof(Ident));
                    if (token->data->id == NULL) {
                        set_error(ERR_INTERPRET);
                        free(token->data);
                        return NULL;
                    }
                    token->data->id->class = NULL;
                    token->data->id->name = str_init();
                    if (token->data->id->name == NULL) {
                        set_error(ERR_INTERPRET);
                        free(token->data->id);
                        free(token->data);
                        return NULL;
                    }
                    str_append(token->data->id->name, c);
                    current_state = SS_KEYWORD_IDENT;
                } else if (isdigit(c)) {
                    //number
                    token->data = (ScannerTokenData*)malloc(sizeof(ScannerTokenData));
                    if (token->data == NULL) {
                        set_error(ERR_INTERPRET);
                        return NULL;
                    }
                    token->data->str = str_init();
                    if (token->data->str == NULL) {
                        set_error(ERR_INTERPRET);
                        free(token->data);
                        return NULL;
                    }
                    str_append(token->data->str, c);
                    if (c == '0') {
                        current_state = SS_NUMERAL_SYS;
                    } else {
                        current_state = SS_NUMBER;
                    }
                } else if (c == '=') {
                    current_state = SS_EQUAL;
                } else if (c == ',') {
                    token->type = STT_COMMA;
                    return token;
                }  else if (c == ';') {
                    token->type = STT_SEMICOLON;
                    return token;
                } else if (c == '"') {
                    //string
                    token->data = (ScannerTokenData*)malloc(sizeof(ScannerTokenData));
                    if (token->data == NULL) {
                        set_error(ERR_INTERPRET);
                        return NULL;
                    }
                    token->data->str = str_init();
                    if (token->data->str == NULL) {
                        set_error(ERR_INTERPRET);
                        free(token->data);
                        return NULL;
                    }
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
                    if ((!isalnum(c)) && (c != '_') && (c != '$')) {
                        if (c == '.') {
                            key_or_ident(token);
                            if (token->type != STT_IDENT) {
                                free(token->data);
                                current_state = SS_LEX_ERROR;
                            } else {
                                token->data->id->class = str_init();
                                if (token->data->id->class == NULL) {
                                    set_error(ERR_INTERPRET);
                                    str_dispose(token->data->id->name);
                                    free(token->data->id);
                                    free(token->data);
                                    return NULL;
                                }
                                //cut content from token->data->id->name to token->data->id->class and ignore .
                                str_copy_string(token->data->id->class, token->data->id->name);
                                str_clear(token->data->id->name);
                                current_state = SS_CLASS_AND_FUNCTION_1;
                            }
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
                if (isalpha(c) || (c == '_') || (c == '$')) {
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
                // append alpha chars for 2. identificator - member
                if (isalnum(c) || (c == '_') || (c == '$')) {
                    str_append(token->data->id->name, c);
                    current_state = SS_CLASS_AND_FUNCTION_2;
                } else {
                    key_or_ident(token);
                    if (token->type != STT_IDENT) {
                        free(token->data);
                        current_state = SS_LEX_ERROR;
                    } else {
                        ungetc(c, f);
                        token->type = STT_IDENT;
                        return token;
                    }
                }

                break;

            case SS_NUMBER:
                //is number
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
                } else if (c == '_') {
                    current_state = SS_NUMBER_1;
                } else {
                    // next token
                    long i = strtol(str_get_str(token->data->str), NULL, 10);
                    str_dispose(token->data->str);
                    if (i > INT_MAX) {
                        free(token->data);
                        current_state = SS_LEX_ERROR;
                    } else {
                        token->data->i = (int)i;
                        token->type = STT_INT;
                        ungetc(c, f);
                        return token;
                    }
                }

                break;

            case SS_NUMBER_1:
                //is number
                if (isdigit(c)) {
                    //is digit => append
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER;
                } else if (c == '_') {
                    current_state = SS_NUMBER_1;
                } else {
                    //is space => error
                    str_dispose(token->data->str);
                    free(token->data);
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
                } else if (c == '_') {
                    current_state = SS_DOUBLE_EX_4;
                } else {
                    // next token
                    ungetc(c, f);
                    double number = strtod(str_get_str(token->data->str), NULL);
                    if (errno == ERANGE) {
                        str_dispose(token->data->str);
                        free(token->data);
                        current_state = SS_LEX_ERROR;
                    } else {
                        str_dispose(token->data->str);
                        token->data->d = number;
                        token->type = STT_DOUBLE;
                        return token;
                    }
                }

                break;

            case SS_DOUBLE_EX_4:
                //is double => must be digit or next token
                // is number
                if (isdigit(c)) {
                    // is digit => append
                    str_append(token->data->str, c);
                    current_state = SS_DOUBLE_EX_3;
                } else if (c == '_') {
                    current_state = SS_DOUBLE_EX_4;
                } else {
                    //is something else => error
                    str_dispose(token->data->str);
                    free(token->data);
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
                } else if (c == '_') {
                    current_state = SS_DOUBLE_DEC_3;
                } else {
                    // next token
                    ungetc(c, f);
                    double number = strtod(str_get_str(token->data->str), NULL);
                    if (errno == ERANGE) {
                        str_dispose(token->data->str);
                        free(token->data);
                        current_state = SS_LEX_ERROR;
                    } else {
                        str_dispose(token->data->str);
                        token->data->d = number;
                        token->type = STT_DOUBLE;
                        return token;
                    }
                }

                break;

            case SS_DOUBLE_DEC_3:
               //is double => must be digit or exponent or next token
               // is number
               if (isdigit(c)) {
                   // is digit => append
                   str_append(token->data->str, c);
                   current_state = SS_DOUBLE_DEC_2;
               } else if (c == '_') {
                   current_state = SS_DOUBLE_DEC_3;
               } else {
                   //is something else => error
                   str_dispose(token->data->str);
                   free(token->data);
                   current_state = SS_LEX_ERROR;
               }

               break;

            case SS_NUMERAL_SYS:
                if (c == 'b') {
                    //is binary
                    str_clear(token->data->str);
                    current_state = SS_NUMBER_BIN_1;
                } else if (c == 'x') {
                    // hex
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_1;
                } else if (isdigit(c) && (c != '9') && (c != '8')) {
                    // oct
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_OCT_1;
                } else if (c == '_') {
                    // oct
                    current_state = SS_NUMBER_OCT_2;
                } else if (c == '.') {
                    // double
                    str_append(token->data->str, c);
                    current_state = SS_DOUBLE_DEC_1;
                } else {
                    //must be zero
                    str_dispose(token->data->str);
                    token->data->i = 0;
                    token->type = STT_INT;
                    ungetc(c, f);
                    return token;
                }

                break;

            case SS_NUMBER_OCT_1:
                if (isdigit(c) && (c != '9') && (c != '8')) {
                    //is oct
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_OCT_1;
                } else if (c == '_') {
                    current_state = SS_NUMBER_OCT_2;
                } else {
                    // return number
                    long i = strtol(str_get_str(token->data->str), NULL, 8);
                    if (i > INT_MAX) {
                        str_dispose(token->data->str);
                        free(token->data);
                        current_state = SS_LEX_ERROR;
                    } else {
                        str_dispose(token->data->str);
                        token->data->i = (int)i;
                        token->type = STT_INT;
                        ungetc(c, f);
                        return token;
                    }
                }

                break;

            case SS_NUMBER_OCT_2:
                if (isdigit(c) && (c != '9') && (c != '8')) {
                    //is oct
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_OCT_1;
                } else if (c == '_') {
                    current_state = SS_NUMBER_OCT_2;
                } else {
                    //error
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_NUMBER_BIN_1:
                if ((c == '0') || (c == '1')) {
                    //is hex
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_BIN_2;
                } else {
                    //error
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_NUMBER_BIN_2:
                if ((c == '0') || (c == '1')) {
                    //is binary
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_BIN_2;
                } else if (c == '_') {
                    current_state = SS_NUMBER_BIN_3;
                } else {
                    // return number
                    long i = strtol(str_get_str(token->data->str), NULL, 2);
                    if (i > INT_MAX) {
                        str_dispose(token->data->str);
                        free(token->data);
                        current_state = SS_LEX_ERROR;
                    } else {
                        str_dispose(token->data->str);
                        token->data->i = (int)i;
                        token->type = STT_INT;
                        ungetc(c, f);
                        return token;
                    }
                }

                break;

            case SS_NUMBER_BIN_3:
                if ((c == '0') || (c == '1')) {
                    //is binary
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_BIN_2;
                } else if (c == '_') {
                    current_state = SS_NUMBER_BIN_3;
                } else {
                    //error
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_NUMBER_HEX_1:
                if ((isdigit(c)) || (c == 'A') || (c == 'B') || (c == 'C') || (c == 'D') || (c == 'E') || (c == 'F') ||
                   (c == 'a') || (c == 'b') || (c == 'c') || (c == 'd') || (c == 'e') || (c == 'f')) {
                    //is hex
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_2;
                } else {
                    //error
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_NUMBER_HEX_2:
                if ((isdigit(c)) || (c == 'A') || (c == 'B') || (c == 'C') || (c == 'D') || (c == 'E') || (c == 'F') ||
                   (c == 'a') || (c == 'b') || (c == 'c') || (c == 'd') || (c == 'e') || (c == 'f')) {
                    //is hex
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_2;
                } else if (c == '_') {
                    current_state = SS_NUMBER_HEX_3;
                } else if (c == '.') {
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_DOUBLE_0;
                } else {
                    // return number
                    long i = strtol(str_get_str(token->data->str), NULL, 16);
                    if (i > INT_MAX) {
                        str_dispose(token->data->str);
                        free(token->data);
                        current_state = SS_LEX_ERROR;
                    } else {
                        str_dispose(token->data->str);
                        token->data->i = (int)i;
                        token->type = STT_INT;
                        ungetc(c, f);
                        return token;
                    }
                }

                break;

            case SS_NUMBER_HEX_3:
                if ((isdigit(c)) || (c == 'A') || (c == 'B') || (c == 'C') || (c == 'D') || (c == 'E') || (c == 'F') ||
                   (c == 'a') || (c == 'b') || (c == 'c') || (c == 'd') || (c == 'e') || (c == 'f')) {
                    //is hex
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_2;
                } else if (c == '_') {
                    current_state = SS_NUMBER_HEX_3;
                } else {
                    //error
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_NUMBER_HEX_DOUBLE_0:
                if ((isdigit(c)) || (c == 'A') || (c == 'B') || (c == 'C') || (c == 'D') || (c == 'E') || (c == 'F')  ||
                   (c == 'a') || (c == 'b') || (c == 'c') || (c == 'd') || (c == 'e') || (c == 'f')) {
                    //is double hex
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_DOUBLE_1;
                } else {
                    //error
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_NUMBER_HEX_DOUBLE_1:
                if ((isdigit(c)) || (c == 'A') || (c == 'B') || (c == 'C') || (c == 'D') || (c == 'E') || (c == 'F')  ||
                   (c == 'a') || (c == 'b') || (c == 'c') || (c == 'd') || (c == 'e') || (c == 'f')) {
                    //is double hex
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_DOUBLE_1;
                } else if (c == '_') {
                    current_state = SS_NUMBER_HEX_DOUBLE_2;
                } else if ((c == 'P') || (c == 'p')) {
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_DOUBLE_P_1;
                } else {
                    //error
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_NUMBER_HEX_DOUBLE_2:
                if ((isdigit(c)) || (c == 'A') || (c == 'B') || (c == 'C') || (c == 'D') || (c == 'E') || (c == 'F')  ||
                   (c == 'a') || (c == 'b') || (c == 'c') || (c == 'd') || (c == 'e') || (c == 'f')) {
                    //is double hex
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_DOUBLE_1;
                } else if (c == '_') {
                    current_state = SS_NUMBER_HEX_DOUBLE_2;
                } else {
                    //error
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_NUMBER_HEX_DOUBLE_P_1:
                if (isdigit(c)) {
                    //after P(p) can be + or -
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_DOUBLE_P_2;
                } else if ((c == '-') || (c == '+')) {
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_DOUBLE_P_SIGN;
                } else {
                    //error
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_NUMBER_HEX_DOUBLE_P_2:
                if (isdigit(c)) {
                    //after number
                    str_append(token->data->str, c);
                    current_state =  SS_NUMBER_HEX_DOUBLE_P_2;
                } else if (c == '_') {
                    current_state = SS_NUMBER_HEX_DOUBLE_P_3;
                } else {
                    // return number
                    int d = strtod(str_get_str(token->data->str), NULL);
                    str_dispose(token->data->str);
                    token->data->d = d;
                    token->type = STT_DOUBLE;
                    ungetc(c, f);
                    return token;
                }

                break;

            case SS_NUMBER_HEX_DOUBLE_P_3:
                //after + or - must be number
                if (isdigit(c)) {
                    //is binary
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_DOUBLE_P_2;
                } else if (c == '_') {
                       current_state = SS_NUMBER_HEX_DOUBLE_P_3;
                } else {
                    //error
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_NUMBER_HEX_DOUBLE_P_SIGN:
                //after + or - must be number
                if (isdigit(c)) {
                    //is binary
                    str_append(token->data->str, c);
                    current_state = SS_NUMBER_HEX_DOUBLE_P_2;
                } else {
                    //error
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_STRING:
                //is string
                //load whole read char !='"'
                if (c == '\n') {
                    //is '"' => end of string => parse
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                } else if (c == '\\') {
                    // '\'
                    current_state = SS_BACKSLASH;
                } else if (c != '"') {
                    //not '"' => append
                    str_append(token->data->str, c);
                    current_state = SS_STRING;
                } else {
                    //is '"' => end of string => parse
                    token->type = STT_STRING;
                    return token;
                }

                break;

            case SS_BACKSLASH:
                if (c == 'n') {
                    str_append(token->data->str, 10);
                    current_state = SS_STRING;
                } else if (c == '"') {
                    str_append(token->data->str, 34);
                    current_state = SS_STRING;
                } else if (c == 't') {
                    str_append(token->data->str, 9);
                    current_state = SS_STRING;
                } else if (c == '\\') {
                    str_append(token->data->str, 92);
                    current_state = SS_STRING;
                } else if (c == '0') {
                    number = str_init();
                    str_append(number, c);
                    current_state = SS_OCT_ESCAPE_MIN_1;
                } else if ((c == '1') || (c == '2')) {
                    number = str_init();
                    str_append(number, c);
                    current_state = SS_OCT_ESCAPE_1;
                } else if (c == '3') {
                    number = str_init();
                    str_append(number, c);
                    current_state = SS_OCT_ESCAPE_MAX_1;
                } else {
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }
                break;

            case SS_OCT_ESCAPE_1:
                if (isdigit(c)) {
                    str_append(number, c);
                    current_state = SS_OCT_ESCAPE_2;
                } else {
                    str_dispose(number);
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }
                break;

            case SS_OCT_ESCAPE_2:
                if (isdigit(c)) {
                    str_append(number, c);
                    int i = (int)strtol(str_get_str(number), NULL, 8);
                    str_append(token->data->str, i);
                    str_dispose(number);
                    current_state = SS_STRING;
                } else {
                    str_dispose(number);
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }
                break;

            case SS_OCT_ESCAPE_MAX_1:
                if (isdigit(c) && (c != '8') && (c != '9')) {
                    str_append(number, c);
                    current_state = SS_OCT_ESCAPE_MAX_2;
                } else {
                    str_dispose(number);
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }
                break;

            case SS_OCT_ESCAPE_MAX_2:
                if (isdigit(c) && (c != '8') && (c != '9')) {
                    str_append(number, c);
                    int i = (int)strtol(str_get_str(number), NULL, 8);
                    str_append(token->data->str, i);
                    str_dispose(number);
                    current_state = SS_STRING;
                } else {
                    str_dispose(number);
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }
                break;

            case SS_OCT_ESCAPE_MIN_1:
                if (c == '0') {
                    str_append(number, c);
                    current_state = SS_OCT_ESCAPE_MIN_2;
                } else if (isdigit(c)) {
                    str_append(number, c);
                    current_state = SS_OCT_ESCAPE_2;
                } else {
                    str_dispose(number);
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
                }
                break;

            case SS_OCT_ESCAPE_MIN_2:
                if (isdigit(c) && (c != '0')) {
                    str_append(number, c);
                    int i = (int)strtol(str_get_str(number), NULL, 8);
                    str_append(token->data->str, i);
                    str_dispose(number);
                    current_state = SS_STRING;
                } else {
                    str_dispose(number);
                    str_dispose(token->data->str);
                    free(token->data);
                    current_state = SS_LEX_ERROR;
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
                    // end of file without proper comment ending => error
                    ungetc(c, f);
                    current_state = SS_LEX_ERROR;
                } else {
                    //stay in state
                    current_state = SS_COMMENT_BLOCK;
                }

                break;

            case SS_COMMENT_BLOCK_END:
                if (c == '/') {
                    // DO NOT RETURN COMMENTS
                    current_state = SS_EMPTY;
                } else if (c == EOF) {
                    // end of file = end of comment
                    ungetc(c, f);
                    current_state = SS_LEX_ERROR;
                } else {
                    //not end => go back to block comment state
                    current_state = SS_COMMENT_BLOCK;
                }

                break;

            case SS_LEX_ERROR:
                // lexical error
                set_error(ERR_LEX);
                // continue with scan file after isspace
                while (!isspace(c)) {
                    c = getc(f);
                    if(c == -1) {
                        break;
                    }
                }
                token->type = STT_EMPTY;

                return token;

                break;

            case SS_EOF:
                //end of file
                token->type = STT_EOF;
                return token;

                break;
        }
    }
}
