#include <stdio.h>
#include "context.h"
#include "error.h"
#include "expression.h"
#include "ial.h"
#include "list.h"
#include "parser.h"
#include "precedence_table.h"
#include "scanner_token.h"
#include "stack.h"

List* token_list; ///< list of tokens
ScannerToken* current_token; ///< token currently being processed
Context* main_context; ///< main context
Context* current_context; ///< context, which is currently being used
SymbolName current_class_name; ///< name of current class


const TokenPrecedence precedence_table[][STT_STRING] = {
    [EO_PLUS] = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_MINUS] = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_MULTIPLY] = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = H,
        [EO_DIVIDE] = H,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_DIVIDE] = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = H,
        [EO_DIVIDE] = H,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_LESS] = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_GREATER] = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_LESS_EQUALS] = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_GREATER_EQUALS] = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_LOGIC_EQUAL] = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = L,
        [EO_GREATER] = L,
        [EO_LESS_EQUALS] = L,
        [EO_GREATER_EQUALS] = L,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_LOGIC_NOT_EQUAL]   = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = L,
        [EO_GREATER] = L,
        [EO_LESS_EQUALS] = L,
        [EO_GREATER_EQUALS] = L,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_AND]               = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = L,
        [EO_GREATER] = L,
        [EO_LESS_EQUALS] = L,
        [EO_GREATER_EQUALS] = L,
        [EO_LOGIC_EQUAL] = L,
        [EO_LOGIC_NOT_EQUAL] = L,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_OR]                = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = L,
        [EO_GREATER] = L,
        [EO_LESS_EQUALS] = L,
        [EO_GREATER_EQUALS] = L,
        [EO_LOGIC_EQUAL] = L,
        [EO_LOGIC_NOT_EQUAL] = L,
        [EO_AND] = L,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_NEGATE] = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = H,
        [EO_DIVIDE] = H,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = H
    },
    [EO_LEFT_PARENTHESE]   = {
        [EO_PLUS] = L,
        [EO_MINUS] = L,
        [EO_MULTIPLY] = L,
        [EO_DIVIDE] = L,
        [EO_LESS] = L,
        [EO_GREATER] = L,
        [EO_LESS_EQUALS] = L,
        [EO_GREATER_EQUALS] = L,
        [EO_LOGIC_EQUAL] = L,
        [EO_LOGIC_NOT_EQUAL] = L,
        [EO_AND] = L,
        [EO_OR] = L,
        [EO_NEGATE] = L,
        [EO_LEFT_PARENTHESE] = L,
        [EO_RIGHT_PARENTHESE] = E
    },
    [EO_RIGHT_PARENTHESE]  = {
        [EO_PLUS] = H,
        [EO_MINUS] = H,
        [EO_MULTIPLY] = H,
        [EO_DIVIDE] = H,
        [EO_LESS] = H,
        [EO_GREATER] = H,
        [EO_LESS_EQUALS] = H,
        [EO_GREATER_EQUALS] = H,
        [EO_LOGIC_EQUAL] = H,
        [EO_LOGIC_NOT_EQUAL] = H,
        [EO_AND] = H,
        [EO_OR] = H,
        [EO_NEGATE] = H,
        [EO_LEFT_PARENTHESE] = N,
        [EO_RIGHT_PARENTHESE] = H
    },
};


// sets next token as current_token
static inline ScannerToken* next_token() {
    list_activate_next(token_list);
    current_token = token_list->active->data.token;

    return current_token;
}

// sets previous token as current_token
static inline ScannerToken* prev_token() {
    list_activate_prev(token_list);
    current_token = token_list->active->data.token;

    return current_token;
}

// sets active item as current_token
// used when token_list is modified in other modules
static inline ScannerToken* refresh_current_token() {
    current_token = token_list->active->data.token;

    return current_token;
}


void parse(List* _token_list) {
    //prepare
    list_activate_first(_token_list);
    token_list = _token_list;
    current_token = token_list->active->data.token;
    main_context = context_init(NULL);
    current_context = main_context;
    current_class_name = NULL;


    // start the actual parsing
    // first thing in file should be class (or several)
    class_list_rule();
}


void class_list_rule() {
    while (current_token->type != STT_EOF && !get_error()->type) {
        switch (current_token->type) {
            case STT_KEYWORD:
            {
                if(current_token->data->keyword_type != KW_CLASS) return set_error(ERR_SYNTAX);
                next_token();
                class_rule();
                current_context = main_context;
                current_class_name = NULL;
                next_token();

                break;
            }
            default:
            {
                set_error(ERR_SYNTAX);
                return;
                // break;
            }
        }
    }
}

//starts @ STT_IDENT
//finish @ STT_RIGHT_BRACE
void class_rule() {
    if(current_token->type != STT_IDENT) return set_error(ERR_SYNTAX);
    if(table_find_symbol(main_context->symbol_table, current_token->data->id->name) != NULL) {
        set_error(ERR_SEMANTIC);
        return;
    }
    Symbol* new_class = table_insert_class(main_context->symbol_table, current_token->data->id->name, main_context);
    current_class_name = current_token->data->id->name;
    current_context = new_class->data.cls->context;

    if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
    if(next_token()->type != STT_RIGHT_BRACE) {
        // parse inside of the class
        class_members_rule();
        if(get_error()->type) return;
        if(next_token()->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
    } else {
        // empty class => ok
    }
}

//starts @ STT_KEYWORD
//finish @ STT_RIGHT_BRACE || STT_SEMICOLON
void class_members_rule() {
    if(current_token->type == STT_RIGHT_BRACE || get_error()->type) {
        prev_token();
        return;
    }
    class_member_rule();
    class_members_rule();
}


//starts @ STT_KEYWORD (static)
//finish @ STT_RIGHT_BRACE or STT_SEMICOLON
void class_member_rule() {
    if(current_token->type != STT_KEYWORD || current_token->data->keyword_type != KW_STATIC) return set_error(ERR_SYNTAX);
    if(next_token()->type != STT_KEYWORD_TYPE) return set_error(ERR_SYNTAX);
    KeywordType current_type = current_token->data->keyword_type;
    if(next_token()->type != STT_IDENT) return set_error(ERR_SYNTAX);
    ScannerToken* current_ident = current_token;
    next_token();
    if(current_token->type == STT_SEMICOLON) {
        //add variable to context
        context_add_variable(current_context, current_type, current_ident->data->id->name);
        if(get_error()->type) return;
        //syntax ok
    } else if(current_token->type == STT_EQUALS) {
        //add variable to context
        Symbol* symbol = context_add_variable(current_context, current_type, current_ident->data->id->name);
        (void)symbol;
        if(get_error()->type) return;

        next_token();
        expression_rule();
        if(get_error()->type) {
            return;
        }
    } else  if(current_token->type == STT_LEFT_PARENTHESE) {
        // this is a function
        // but also class member
        //    =>
        // to the class context, add name
        // change context to local function's context
        // swith back after the function

        Symbol* fn_symbol = context_add_function(current_context, current_type, current_ident->data->id->name);
        current_context = fn_symbol->data.fn->context;

        next_token();
        params_list_rule(fn_symbol->data.fn->params_list);
        if(get_error()->type) {
            return;
        }
        if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        next_token();
        stat_list_rule();
        if(get_error()->type) {
            return;
        }
        if(current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
        current_context = current_context->parent_context;
    }
    next_token();
}

//starts @ STT_KEYWORD_TYPE || STT_RIGHT_PARENTHESE
//finish @ STT_RIGHT_BRACE
void params_list_rule(List* params_list) {
    if(current_token->type == STT_RIGHT_PARENTHESE || get_error()->type) {
        return;
    }

    if(current_token->type != STT_KEYWORD_TYPE) return set_error(ERR_SYNTAX);
    KeywordType current_type = current_token->data->keyword_type;
    //token must be IDENT and must be simple (no class part)
    if(next_token()->type != STT_IDENT || current_token->data->id->class != NULL) return set_error(ERR_SYNTAX);
    //add variable to current context
    context_add_variable(current_context, current_type, current_token->data->id->name);

    //add to params_list
    ListItemData data;
    switch (current_type) {
        case KW_BOOLEAN:
            data.var_type = VT_BOOL;
            break;
        case KW_INT:
            data.var_type = VT_INTEGER;
            break;
        case KW_DOUBLE:
            data.var_type = VT_DOUBLE;
            break;
        case KW_STRING:
            data.var_type = VT_STRING;
            break;
        default:
            return set_error(ERR_SEMANTIC);
    }
    list_insert_last(params_list, data);

    if(next_token()->type == STT_COMMA) {
        next_token();
        params_list_rule(params_list);
    }
}

//starts @ STT_RIGHT_PARENTHESE || STT_INT || STT_DOUBLE || STT_STRING || STT_KEYWORD (for true,false) || STT_IDENT (for symbol)
//finish @ STT_RIGHT_PARENTHESE
void call_params_list_rule(List *fn_params_list, List *call_params_list) {
    if(current_token->type == STT_RIGHT_PARENTHESE || get_error()->type) {
        return;
    }

    if(fn_params_list->active == NULL) {
        //no more fn params, but more call params => error
        return set_error(ERR_SEMANTIC);
    }

    Expression* expr = expression_init();

    VariableType type = fn_params_list->active->data.var_type;
    if (current_token->type == STT_INT) {
        // is curren param in fn_params_list VT_INTEGER?
        expr->op = EO_CONST_INTEGER;
        if (type == VT_DOUBLE) {
            expr->i = (int)current_token->data->d;
        } else if (type == VT_INTEGER) {
            expr->i = (int)current_token->data->i;
       } else {
           return set_error(ERR_SEMANTIC);
       }
    } else if(current_token->type == STT_STRING) {
        // is curren param in fn_params_list VT_STRING?
        if(type != VT_STRING) {
            expr->op = EO_CONST_STRING;
            expr->str = current_token->data->str;
            return set_error(ERR_SEMANTIC);
        }
    } else if(current_token->type == STT_DOUBLE) {
        // is curren param in fn_params_list VT_DOUBLE?
        if(type != VT_DOUBLE) {
            expr->op = EO_CONST_DOUBLE;
            expr->d = current_token->data->d;
            return set_error(ERR_SEMANTIC);
        }
    } else if(current_token->type == STT_KEYWORD && (
        current_token->data->keyword_type == KW_TRUE ||
        current_token->data->keyword_type == KW_FALSE
    )) {
        // is curren param in fn_params_list VT_BOOL?
        if(type == VT_BOOL) {
            expr->op = EO_CONST_BOOL;
            expr->b = current_token->data->keyword_type == KW_TRUE ? true : false;
        } else {
            return set_error(ERR_SEMANTIC);
        }
    } else if(current_token->type == STT_IDENT) {
        Symbol* symbol = context_find_ident(current_context, main_context, current_token->data->id);
        if(get_error()->type) return;
        if(type != symbol->data.var->type) return set_error(ERR_SEMANTIC);
        expr->op = EO_SYMBOL;
        expr->symbol = symbol;
    } else{
        return set_error(ERR_SYNTAX);
    }

    list_activate_next(fn_params_list);
    if(next_token()->type == STT_COMMA) {
        next_token();
        call_params_list_rule(fn_params_list, call_params_list);
    }
}

//starts @ STT_KEYWORD_TYPE
//finish @ STT_IDENT
void definition_rule() {
    if(current_token->type != STT_KEYWORD_TYPE) return set_error(ERR_SYNTAX);
    KeywordType current_type = current_token->data->keyword_type;
    //token must be IDENT and must be simple (no class part)
    if(next_token()->type != STT_IDENT || current_token->data->id->class != NULL) return set_error(ERR_SYNTAX);

    //add variable to current context
    context_add_variable(current_context, current_type, current_token->data->id->name);
}


//starts @ STT_KEYWORD_TYPE || STT_KEYWORD_TYPE || STT_IDENT
//finish @ STT_RIGHT_BRACE || STT_RIGHT_PARENTHESE || STT_SEMICOLON
void stat_list_rule() {
    // handles empty stat
    // eg. empty if ("if () {<nothing here>}")
    if(current_token->type == STT_RIGHT_BRACE || get_error()->type) {
        return;
    }
    stat_rule();
    stat_list_rule();
}

//starts @ STT_KEYWORD_TYPE || STT_KEYWORD_TYPE || STT_IDENT
//finish @ STT_RIGHT_BRACE || STT_RIGHT_PARENTHESE || STT_SEMICOLON
void stat_rule() {
    // IF
    if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_IF) {
        //(
        if(next_token()->type != STT_LEFT_PARENTHESE) return set_error(ERR_SYNTAX);
        //boolean expression in condition
        next_token();
        bool_expression_rule();
        // )
        if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);

        // {
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        //statements inside IF
        // TODO: no definition inside if
        next_token();
        stat_list_rule();
        // {
        if(current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
    } else if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_WHILE) { // WHILE
        //(
        if(next_token()->type != STT_LEFT_PARENTHESE) return set_error(ERR_SYNTAX);
        //boolean expression in condition
        next_token();
        bool_expression_rule();
        // )
        if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);

        // {
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        //statements inside IF
        // TODO: no definition inside while
        next_token();
        stat_list_rule();
        // {
        if(current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
    } else if(current_token->type == STT_IDENT) {
        //check if symbol exists
        Symbol* symbol = context_find_ident(current_context, main_context, current_token->data->id);
        if(get_error()->type) return;

        next_token();
        if(current_token->type == STT_LEFT_PARENTHESE) {
            //function call
            if(symbol->type != ST_FUNCTION) return set_error(ERR_SEMANTIC);

            //list for params types
            list_activate_first(symbol->data.fn->params_list);
            next_token();
            List *call_params_list = list_init();
            call_params_list_rule(symbol->data.fn->params_list, call_params_list);
            if(symbol->data.fn->params_list->active != NULL) {
                fprintf(stderr, "Not all params supplied for fn: %s\n", str_get_str(symbol->name));
                //fn params but call params ended
                return set_error(ERR_SEMANTIC);
            }
            if(get_error()->type) return;
            if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);
            if(next_token()->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);
        } else if(current_token->type == STT_EQUALS) {
            next_token();
            expression_rule();
            if(current_token->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);
        } else {
            fprintf(stderr, "Unexpected token: %s\n", token_to_string(current_token));
            return set_error(ERR_SYNTAX);
        }
    } else if(current_token->type == STT_KEYWORD_TYPE) {
        // variable definition
        definition_rule();
        next_token();
        if(current_token->type == STT_SEMICOLON) {
            // syntax OK
        } else if(current_token->type == STT_EQUALS) {
            next_token();
            expression_rule();
            if(current_token->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);
        } else {
            set_error(ERR_SYNTAX);
        }
    } else {
        return set_error(ERR_SYNTAX);
    }
    next_token();
}

void bool_expression_rule() {
    general_expression_rule(STT_RIGHT_PARENTHESE);
}

void expression_rule() {
    general_expression_rule(STT_SEMICOLON);
}



char ops[][255] = {
[EO_SYMBOL] = "EO_SYMBOL",[EO_SYMBOL_CALL] = "EO_SYMBOL_CALL",[EO_CONST_INTEGER] = "EO_CONST_INTEGER",[EO_CONST_DOUBLE] = "EO_CONST_DOUBLE",[EO_CONST_STRING] = "EO_CONST_STRING",[EO_CONST_BOOL] = "EO_CONST_BOOL",[EO_PLUS] = "EO_PLUS",[EO_MINUS] = "EO_MINUS",[EO_MULTIPLY] = "EO_MULTIPLY",[EO_DIVIDE] = "EO_DIVIDE",[EO_LESS] = "EO_LESS",[EO_GREATER] = "EO_GREATER",[EO_LESS_EQUALS] = "EO_LESS_EQUALS",[EO_GREATER_EQUALS] = "EO_GREATER_EQUALS",[EO_LOGIC_EQUAL] = "EO_LOGIC_EQUAL",[EO_LOGIC_NOT_EQUAL] = "EO_LOGIC_NOT_EQUAL",[EO_AND] = "EO_AND",[EO_OR] = "EO_OR",[EO_NEGATE] = "EO_NEGATE",[EO_LEFT_PARENTHESE] = "EO_LEFT_PARENTHESE",[EO_RIGHT_PARENTHESE] = "EO_RIGHT_PARENTHESE"
};

Expression* general_expression_rule (ScannerTokenType end_token) {
    Stack* term_stack = stack_init();
    Stack* nonterm_stack = stack_init();

    while(current_token->type != end_token) {
        bool is_term = false;
        StackItemData data;
        data.expression = expression_init();

        switch (current_token->type) {
            case STT_INT:
                is_term = true;
                data.expression->op = EO_CONST_INTEGER;
                data.expression->i = current_token->data->i;
                break;
            case STT_DOUBLE:
                is_term = true;
                data.expression->op = EO_CONST_DOUBLE;
                data.expression->d = current_token->data->d;
                break;
            case STT_STRING:
                is_term = true;
                data.expression->op = EO_CONST_STRING;
                data.expression->str = current_token->data->str;
                break;
            case STT_KEYWORD:
                //is keyword => check if is "true" or "false"
                if(current_token->data->keyword_type == KW_TRUE || current_token->data->keyword_type == KW_FALSE) {
                    is_term = true;
                    data.expression->op = EO_CONST_STRING;
                    data.expression->b = current_token->data->keyword_type == KW_TRUE ? true : false;
                } else {
                    set_error(ERR_SYNTAX);
                    return NULL;
                }
                break;
            case STT_IDENT:
            {
                is_term = true;
                Symbol* symbol = context_find_ident(current_context, main_context, current_token->data->id);
                if(get_error()->type) return NULL;
                if(next_token()->type == STT_LEFT_PARENTHESE) {
                    //function call
                    if(symbol->type != ST_FUNCTION) {
                        set_error(ERR_SEMANTIC);
                        return NULL;
                    }

                    //list for params types
                    list_activate_first(symbol->data.fn->params_list);
                    next_token();
                    List *call_params_list = list_init();
                    call_params_list_rule(symbol->data.fn->params_list, call_params_list);
                    if(symbol->data.fn->params_list->active != NULL) {
                        fprintf(stderr, "Not all params supplied for fn: %s\n", str_get_str(symbol->name));
                        //fn params but call params ended
                        set_error(ERR_SEMANTIC);
                        return NULL;
                    }
                    if(get_error()->type) return NULL;
                    if(current_token->type != STT_RIGHT_PARENTHESE) {
                        set_error(ERR_SYNTAX);
                        return NULL;
                    }

                    data.expression->op = EO_SYMBOL_CALL;
                    data.expression->symbol = symbol;
                    data.expression->call_params = call_params_list;
                } else {
                    prev_token();
                    data.expression->op = EO_SYMBOL;
                    data.expression->symbol = symbol;
                }
                break;
            }
            case STT_PLUS:
                data.expression->op = EO_PLUS;
                break;
            case STT_MINUS:
                data.expression->op = EO_MINUS;
                break;
            case STT_MULTIPLY:
                data.expression->op = EO_MULTIPLY;
                break;
            case STT_DIVIDE:
                data.expression->op = EO_DIVIDE;
                break;
            case STT_LEFT_PARENTHESE:
                data.expression->op = EO_LEFT_PARENTHESE;
                break;
            case STT_RIGHT_PARENTHESE:
                data.expression->op = EO_RIGHT_PARENTHESE;
                break;
            default:
                set_error(ERR_SYNTAX);
                break;
        }

        if(is_term) {
            printf("pushing term: %s\n", token_to_string(current_token));
            stack_push(term_stack, data);
        } else {
            StackItemData* top = stack_top(nonterm_stack);
            if(data.expression->op == EO_RIGHT_PARENTHESE) {
                printf("nonterm ) => push all the way to (\n");
                //treat "(" and ")"
                // TODO: expression_dispose data.expression
                while(top != NULL && top->expression->op != EO_LEFT_PARENTHESE) {
                    //reduce

                    // first to ->expr2
                    StackItemData* term_top2 = stack_top(term_stack);
                    if(term_top2 == NULL) {
                        set_error(ERR_SYNTAX);
                        return NULL;
                    }
                    stack_pop(term_stack);
                    // second to ->expr1
                    StackItemData* term_top1 = stack_top(term_stack);
                    if(term_top1 == NULL) {
                        set_error(ERR_SYNTAX);
                        return NULL;
                    }
                    printf("poping nonterm %s %d\n", ops[stack_top(nonterm_stack)->expression->op], __LINE__);
                    stack_pop(term_stack);

                    //set terms for current top
                    top->expression->expr2 = term_top2->expression;
                    top->expression->expr1 = term_top1->expression;

                    //push the top to term
                    stack_push(term_stack, *top);
                    printf("poping nonterm %s %d\n", ops[stack_top(nonterm_stack)->expression->op], __LINE__);
                    //pop it from nonterm
                    stack_pop(nonterm_stack);


                    // set new top
                    top = stack_top(nonterm_stack);
                }

                if(top->expression->op == EO_LEFT_PARENTHESE) {
                    // TODO: expression_dispose top.expression
                    // pop the "("
                    stack_pop(nonterm_stack);
                } else {
                    // missing "(" => SYNTAX error
                    set_error(ERR_SYNTAX);
                    return NULL;
                }
            } else {
                //while there are ops on stack with priority equal or higher than current op => reduce
                while(top != NULL && precedence_table[top->expression->op][data.expression->op] > E) {
                    // reducing nonterm array means poping 2 (or generally x) values from term stack
                    // and assigning them to the top expr
                    // the top expr will then be poped from nonterm stack and pushed to term stack

                    // first to ->expr2
                    StackItemData* term_top2 = stack_top(term_stack);
                    if(term_top2 == NULL) {
                        set_error(ERR_SYNTAX);
                        return NULL;
                    }
                    stack_pop(term_stack);
                    // second to ->expr1
                    StackItemData* term_top1 = stack_top(term_stack);
                    if(term_top1 == NULL) {
                        set_error(ERR_SYNTAX);
                        return NULL;
                    }
                    stack_pop(term_stack);

                    //set terms for current top
                    top->expression->expr2 = term_top2->expression;
                    top->expression->expr1 = term_top1->expression;

                    //push the top to term
                    stack_push(term_stack, *top);
                    printf("poping nonterm %s %d\n", ops[stack_top(nonterm_stack)->expression->op], __LINE__);
                    //pop it from nonterm
                    stack_pop(nonterm_stack);


                    // set new top
                    top = stack_top(nonterm_stack);
                }

                printf("pushing nonterm %s\n", token_to_string(current_token));
                stack_push(nonterm_stack, data);
            }
        }

        next_token();
    }

    //pop all remaining nonterms
    StackItemData* top = stack_top(nonterm_stack);
    while(top != NULL) {
        // reducing nonterm array means poping 2 (or generally x) values from term stack
        // and assigning them to the top expr
        // the top expr will then be poped from nonterm stack and pushed to term stack

        // first to ->expr2
        StackItemData* term_top2 = stack_top(term_stack);
        if(term_top2 == NULL) {
            set_error(ERR_SYNTAX);
            return NULL;
        }
        stack_pop(term_stack);
        // second to ->expr1
        StackItemData* term_top1 = stack_top(term_stack);
        if(term_top1 == NULL) {
            set_error(ERR_SYNTAX);
            return NULL;
        }
        stack_pop(term_stack);

        //set terms for current top
        top->expression->expr2 = term_top2->expression;
        top->expression->expr1 = term_top1->expression;

        //push the top to term
        stack_push(term_stack, *top);
        printf("poping nonterm %s %d\n", ops[stack_top(nonterm_stack)->expression->op], __LINE__);
        //pop it from nonterm
        stack_pop(nonterm_stack);


        // set new top
        top = stack_top(nonterm_stack);
    }

    StackItem* res = stack_pop(term_stack);
    if(stack_pop(term_stack) != NULL) {
        fprintf(stderr, "Cannot parse expression\n");
        set_error(ERR_SYNTAX);
        return NULL;
    }

    expression_print(res->data.expression);
    printf("\n");
    return res->data.expression;
}
