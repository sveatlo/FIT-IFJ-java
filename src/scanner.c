#include <stdlib.h>
#include "scanner.h"
#include "scanner_token.h"

List *scan_file(FILE *f, List *token_list) {
    while (42) {
        ScannerToken *token = get_next_token(f);
        if(token->type == STT_EMPTY) {
            break;
        }

        ListItemData list_item_data = {
            .token = token
        };
        list_insert_last(token_list, list_item_data);
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

    while (69) {
        //get next char from file
        c = getc(f);

        switch (current_state) {
            //default state => every token starts here
            case SS_EMPTY:
                if (isspace(c)) {
                    //ignore space
                    current_state = SS_EMPTY;
                } else if (c == '/') {
                    current_state = SS_SLASH;
                } else {
                    current_state = SS_LEX_ERROR;
                }

                break;

            case SS_SLASH:
                //after SS_EMPTY we got a slash char => this could be a comment
                if(c == '/') {
                    //definitely inline comment
                    current_state = SS_COMMENT_LINE;
                } else if (c == '*') {
                    //definitely block comment
                    current_state = SS_COMMENT_BLOCK;
                } else {
                    //lex error
                    current_state = SS_LEX_ERROR;
                }
                break;

            case SS_COMMENT_LINE:
                if(c == '\n'){
                    //end of line = end of comment
                    token->type = STT_COMMENT;
                    return token;
                } else {
                    //stay in state
                    current_state = SS_COMMENT_LINE;
                }

                break;

            case SS_COMMENT_BLOCK:
                if(c == '*'){
                    //could be end of block comment
                    current_state = SS_COMMENT_BLOCK_END;
                } else {
                    //stay in state
                    current_state = SS_COMMENT_BLOCK;
                }

                break;

            case SS_COMMENT_BLOCK_END:
                if(c == '/') {
                    token->type = STT_COMMENT;
                    return token;
                } else {
                    //not end => go back to block comment state
                    current_state = SS_COMMENT_BLOCK;
                }

                break;

            case SS_LEX_ERROR:
            default:
                set_error(ERR_LEX);
                token->type = STT_EMPTY;
                return token;
        }
    }
}
