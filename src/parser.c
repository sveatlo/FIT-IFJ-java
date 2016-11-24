#include <stdio.h>
#include "context.h"
#include "error.h"
#include "expression.h"
#include "ial.h"
#include "list.h"
#include "parser.h"
#include "scanner_token.h"

List* token_list; ///< list of tokens
ScannerToken* current_token; ///< token currently being processed
Context* main_context; ///< main context
Context* current_context; ///< context, which is currently being used
SymbolName current_class_name; ///< name of current class

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

static inline Symbol* context_add_variable(Context* context, KeywordType type, String* name) {
    if(table_find_symbol(context->symbol_table, name) != NULL) {
        fprintf(stderr, "Symbol \"%s\" already defined\n", str_get_str(name));
        set_error(ERR_SEMANTIC);
        return;
    }

    switch (type) {
        case KW_INT:
            return table_insert_integer(context->symbol_table, name, 0);
            break;
        case KW_DOUBLE:
            return table_insert_double(context->symbol_table, name, 0);
            break;
        case KW_BOOLEAN:
            return table_insert_bool(context->symbol_table, name, NULL);
            break;
        case KW_STRING:
            return table_insert_string(context->symbol_table, name, NULL);
            break;
        case KW_VOID:
            return table_insert_function(context->symbol_table, name, context);
            break;
        default:
            return set_error(ERR_SYNTAX);
    }
}

static inline Symbol* find_ident_in_context(Context* context, Ident* token_ident) {
    if(token_ident->class != NULL) {
        Symbol* class_symbol = context_find_symbol(main_context, token_ident->class);
        if(class_symbol == NULL) {
            // TODO: Error - unused var
            set_error(ERR_SEMANTIC);
            fprintf(stderr, "Symbol \"%s\" is not defined.\n", str_get_str(token_ident->class));
            return NULL;
        }
        context = class_symbol->data.cls->context;
    }

    Symbol* symbol = context_find_symbol(context, token_ident->name);
    if(symbol == NULL) {
        // TODO: Error - unused var
        set_error(ERR_SEMANTIC);
        if(token_ident->class != NULL) {
            fprintf(stderr, "Symbol \"%s\" is not defined in class \"%s\".\n", str_get_str(token_ident->name), str_get_str(token_ident->class));
        } else {
            fprintf(stderr, "Symbol \"%s\" is not defined.\n", str_get_str(token_ident->name));
        }
        return NULL;
    } else {
        return symbol;
    }
}

static inline Symbol* context_add_function(Context* context, KeywordType type, String* name) {
    if(table_find_symbol(context->symbol_table, name) != NULL) {
        fprintf(stderr, "Symbol \"%s\" already defined\n", str_get_str(name));
        set_error(ERR_SEMANTIC);
        return NULL;
    }



    //type to return_type (VariableType)
    VariableType return_type;
    switch (type) {
        case KW_INT:
            return_type = VT_INTEGER;
            break;
        case KW_DOUBLE:
            return_type = VT_DOUBLE;
            break;
        case KW_BOOLEAN:
            return_type = VT_BOOL;
            break;
        case KW_STRING:
            return_type = VT_STRING;
            break;
        case KW_VOID:
            return_type = VT_VOID;
            break;
        default:
            set_error(ERR_SYNTAX);
            return NULL;
    }

    Symbol* sym = table_insert_function(context->symbol_table, name, context);
    sym->data.fn->return_type = return_type;

    return sym;
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
void call_params_list_rule(List* fn_params_list) {
    if(current_token->type == STT_RIGHT_PARENTHESE || get_error()->type) {
        return;
    }

    if(fn_params_list->active == NULL) {
        //no more fn params, but more call params => error
        return set_error(ERR_SEMANTIC);
    }

    VariableType type = fn_params_list->active->data.var_type;
    if(current_token->type == STT_INT) {
        // is curren param in fn_params_list VT_INTEGER?
        if(type != VT_INTEGER) {
            return set_error(ERR_SEMANTIC);
        }
    } else if(current_token->type == STT_STRING) {
        // is curren param in fn_params_list VT_STRING?
        if(type != VT_STRING) {
            return set_error(ERR_SEMANTIC);
        }
    } else if(current_token->type == STT_DOUBLE) {
        // is curren param in fn_params_list VT_DOUBLE?
        if(type != VT_DOUBLE) {
            return set_error(ERR_SEMANTIC);
        }
    } else if(current_token->type == STT_KEYWORD && (
        current_token->data->keyword_type == KW_TRUE ||
        current_token->data->keyword_type == KW_FALSE
    )) {
        // is curren param in fn_params_list VT_BOOL?
        if(type != VT_BOOL) {
            return set_error(ERR_SEMANTIC);
        }
    } else if(current_token->type == STT_IDENT) {
        Symbol* symbol = find_ident_in_context(current_context, current_token->data->id);
        if(get_error()->type) return;
        if(type != symbol->data.var->type) return set_error(ERR_SEMANTIC);
    } else{
        return set_error(ERR_SYNTAX);
    }

    if(next_token()->type == STT_COMMA) {
        next_token();
        list_activate_next(fn_params_list);
        call_params_list_rule(fn_params_list);
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
        Symbol* symbol = find_ident_in_context(current_context, current_token->data->id);
        if(get_error()->type) return;

        next_token();
        if(current_token->type == STT_LEFT_PARENTHESE) {
            //function call
            if(symbol->type != ST_FUNCTION) return set_error(ERR_SEMANTIC);

            //list for params types
            list_activate_first(symbol->data.fn->params_list);
            next_token();
            call_params_list_rule(symbol->data.fn->params_list);
            if(symbol->data.fn->params_list->active != NULL) {
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
    while(current_token->type != STT_RIGHT_PARENTHESE) {
        next_token();
    }
}

void expression_rule() {
    // parse_expression_tokens(token_list);
    // refresh_current_token();
    while(current_token->type != STT_SEMICOLON) {
        next_token();
    }
}
