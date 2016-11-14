#include <stdio.h>
#include "error.h"
#include "list.h"
#include "parser.h"
#include "scanner_token.h"

List* token_list;
ScannerToken* current_token;

static inline void next_token() {
    list_activate_next(token_list);
    current_token = token_list->active->data.token;
}

void parse(List* _token_list) {
    list_activate_first(_token_list);
    token_list = _token_list;
    current_token = token_list->active->data.token;

    class_list_rule();
}

void class_list_rule() {
    switch (current_token->type) {
        case STT_KEYWORD:
        {
            class_rule();

            if(current_token != NULL) {
                class_list_rule();
            }


            break;
        }
        default:
        {
            token_to_string(token_list->active->data.token);
            break;
        }
    }
}

void class_rule() {
    class_members_rule();
    next_token();
}

void class_members_rule() {
    class_member_rule();
    next_token();
}

void class_member_rule() {
    next_token();
}
