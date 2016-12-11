#include <stdio.h>
#include <stdlib.h>
#include "context.h"
#include "error.h"
#include "expression.h"
#include "ial.h"
#include "instruction.h"
#include "inbuilt.h"
#include "list.h"
#include "parser.h"
#include "precedence_table.h"
#include "scanner_token.h"
#include "stack.h"

static Context* current_context; ///< context, which is currently being used
static Context* main_context; ///< main context
static List* current_instructions; ///< list of instructions of current function
static List* main_instructions;    ///< list of main instructions - these are used for members initialization and to call Main.run fn
static List* token_list; ///< list of tokens
static ScannerToken* current_token; ///< token currently being processed
static Symbol* current_class; ///< class currently being processed

static bool second_run;


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


void parse(List* _token_list, Context** _context, List** _instructions) {
    //prepare
    list_activate_first(_token_list);
    token_list = _token_list;
    current_token = token_list->active->data.token;
    main_context = context_init(NULL);
    current_context = main_context;
    current_class = NULL;
    main_instructions = list_init(LT_INSTRUCTION);
    current_instructions = main_instructions;

    //add builtin fns
    add_builtin_class(main_context);

    current_context = main_context;
    current_instructions = main_instructions;



    // start the actual parsing
    // first thing in file should be class (or several)
    second_run = false;
    class_list_rule();

    // don't do 2nd run if errors occured
    if(get_error()->type) {
        *_context = main_context;
        *_instructions = main_instructions;

        return;
    }

    second_run = true;
    current_context = main_context;
    current_instructions = main_instructions;
    list_activate_first(_token_list);
    token_list = _token_list;
    current_token = token_list->active->data.token;
    class_list_rule();

    if(get_error()->type) return;

    Ident* main_run_id = (Ident*)malloc(sizeof(Ident));
    main_run_id->class = str_init_const("Main");
    main_run_id->name = str_init_const("run");

    Symbol* main_run_fn = context_find_ident(main_context, main_context, main_run_id);
    str_dispose(main_run_id->class);
    str_dispose(main_run_id->name);
    free(main_run_id);
    if(get_error()->type) return;
    instruction_insert_to_list(main_instructions, instruction_generate(IC_CALL, main_run_fn, NULL, NULL));

    *_context = main_context;
    *_instructions = main_instructions;
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
                current_class = NULL;
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
    if(!second_run) {
        if(table_find_symbol(main_context->symbol_table, current_token->data->id->name) != NULL) {
            set_error(ERR_SEMANTIC);
            return;
        }
        current_class = table_insert_class(main_context->symbol_table, current_token->data->id->name, main_context);
    } else {
        current_class = context_find_ident(main_context, main_context, current_token->data->id);
    }
    current_context = current_class->data.cls->context;

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
    Ident* current_ident = (Ident*)malloc(sizeof(Ident));
    current_ident->class = str_init_str(current_class->name);
    current_ident->name = str_init_str(current_token->data->id->name);
    next_token();
    if(current_token->type == STT_SEMICOLON) {
        //add variable to context
        if(!second_run) {
            context_add_variable(current_context, current_type, current_ident);
        }
        if(get_error()->type) return;
        //syntax ok
    } else if(current_token->type == STT_EQUALS) {
        //add variable to context
        Symbol* symbol = NULL;
        if(!second_run) {
            symbol = context_add_variable(current_context, current_type, current_ident);
        } else {
            symbol = context_find_ident(current_context, main_context, current_ident);
        }
        if(get_error()->type) return;

        next_token();
        Expression* expr = expression_rule();
        if(get_error()->type) {
            return;
        }
        if(second_run) {
            instruction_insert_to_list(main_instructions, instruction_generate(IC_EVAL, expr, NULL, symbol));
        }
    } else if(current_token->type == STT_LEFT_PARENTHESE) {
        // this is a function
        // but also class member
        //    =>
        // to the class context, add name
        // change context to local function's context
        // swith back after the function

        Symbol* fn_symbol = NULL;
        if(!second_run) {
            fn_symbol = context_add_function(current_context, current_type, current_ident);
        } else {
            fn_symbol = context_find_ident(current_context, main_context, current_ident);
        }
        if(get_error()->type) return;
        current_context = fn_symbol->data.fn->context;
        current_instructions = fn_symbol->data.fn->instructions;

        next_token();
        params_list_rule(fn_symbol->data.fn->params_types_list, fn_symbol->data.fn->params_ids_list);
        if(get_error()->type) return;
        if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        next_token();
        stat_list_rule(current_type == KW_VOID, true);
        if(get_error()->type) {
            return;
        }
        if(current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
        current_context = current_context->parent_context;
        current_instructions = main_instructions;
    }


    str_dispose(current_ident->class);
    str_dispose(current_ident->name);
    free(current_ident);
    next_token();
}

//starts @ STT_KEYWORD_TYPE || STT_RIGHT_PARENTHESE
//finish @ STT_RIGHT_BRACE
void params_list_rule(List* params_types_list, List* params_ids_list) {
    if(current_token->type == STT_RIGHT_PARENTHESE || get_error()->type) {
        return;
    }

    if(current_token->type != STT_KEYWORD_TYPE) return set_error(ERR_SYNTAX);
    KeywordType current_type = current_token->data->keyword_type;
    //token must be IDENT and must be simple (no class part)
    if(next_token()->type != STT_IDENT || current_token->data->id->class != NULL) return set_error(ERR_SYNTAX);
    //add variable to current context
    if(!second_run) {
        context_add_variable(current_context, current_type, current_token->data->id);
    } else {
        SymbolTableNode* symbol = table_find_symbol(current_class->data.cls->context->symbol_table, current_token->data->id->name);
        if(symbol != NULL && symbol->data->type == ST_FUNCTION) return set_error(ERR_SEMANTIC);
    }

    //add to params_list
    if(!second_run) {
        ListItemData data1;
        ListItemData data2 = {
            .id = (Ident*)malloc(sizeof(Ident))
        };
        data2.id->class = NULL;
        data2.id->name = str_init_str(current_token->data->id->name);
        switch (current_type) {
            case KW_BOOLEAN:
                data1.var_type = VT_BOOL;
                break;
            case KW_INT:
                data1.var_type = VT_INTEGER;
                break;
            case KW_DOUBLE:
                data1.var_type = VT_DOUBLE;
                break;
            case KW_STRING:
                data1.var_type = VT_STRING;
                break;
            default:
                return set_error(ERR_SEMANTIC);
        }
        list_insert_last(params_types_list, data1);
        list_insert_last(params_ids_list, data2);
    }

    if(next_token()->type == STT_COMMA) {
        next_token();
        params_list_rule(params_types_list, params_ids_list);
    }
}

//starts @ STT_RIGHT_PARENTHESE || STT_INT || STT_DOUBLE || STT_STRING || STT_KEYWORD (for true,false) || STT_IDENT (for symbol)
//finish @ STT_RIGHT_PARENTHESE
void call_params_list_rule(List *fn_params_list, List *call_params_list) {
    if(current_token->type == STT_RIGHT_PARENTHESE || get_error()->type) {
        return;
    }

    if(second_run) {
        if(fn_params_list->active == NULL) {
            //no more fn params, but more call params => error
            return set_error(ERR_SEM_PARAMS);
        }
    }


    Expression* expr = general_expression_rule(STT_COMMA, STT_RIGHT_PARENTHESE);
    //insert the expr to call_params_list
    if(second_run) {
        ListItemData data = {
            .expression = expr
        };
        if(second_run) {
            list_insert_last(call_params_list, data);
        }
    }

    if(second_run) {
        list_activate_next(fn_params_list);
    }
    if(current_token->type == STT_COMMA) {
        next_token();
        call_params_list_rule(fn_params_list, call_params_list);
    }
}

//starts @ STT_KEYWORD_TYPE
//finish @ STT_IDENT
Symbol* definition_rule() {
    if(current_token->type != STT_KEYWORD_TYPE) {
        set_error(ERR_SYNTAX);
        return NULL;
    }

    KeywordType current_type = current_token->data->keyword_type;
    //token must be IDENT and must be simple (no class part)
    next_token();
    if(current_token->type != STT_IDENT || current_token->data->id->class != NULL) {
        set_error(ERR_SYNTAX);
        return NULL;
    }

    //add variable to current context
    if(!second_run) {
        return context_add_variable(current_context, current_type, current_token->data->id);
    } else {
        // check whether there is a fn on class level with same name. if so => err
        SymbolTableNode* symbol = table_find_symbol(current_class->data.cls->context->symbol_table, current_token->data->id->name);
        if(symbol != NULL && symbol->data->type == ST_FUNCTION) {
            set_error(ERR_SEMANTIC);
            return NULL;
        }

        SymbolTableNode* node = table_find_symbol(current_context->symbol_table, current_token->data->id->name);
        if(node != NULL) {
            return node->data;
        } else {
            return NULL;
        }
    }
}


//starts @ STT_KEYWORD_TYPE || STT_KEYWORD_TYPE || STT_IDENT
//finish @ STT_RIGHT_BRACE || STT_RIGHT_PARENTHESE || STT_SEMICOLON
void stat_list_rule(bool is_void, bool can_define) {
    // handles empty stat
    // eg. empty if ("if () {<nothing here>}")
    if(current_token->type == STT_RIGHT_BRACE || get_error()->type) {
        return;
    }
    stat_rule(is_void, can_define);
    stat_list_rule(is_void, can_define);
}

//starts @ STT_KEYWORD_TYPE || STT_KEYWORD_TYPE || STT_IDENT
//finish @ STT_RIGHT_BRACE || STT_RIGHT_PARENTHESE || STT_SEMICOLON
void stat_rule(bool is_void, bool can_define) {
    //return in void
    if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_RETURN ) {
        next_token();
        if(current_token->type == STT_SEMICOLON && !is_void) {
            return set_error(ERR_RUN_NON_INIT_VAR);
        } else if(is_void && current_token->type != STT_SEMICOLON) {
            //semicolon right after return in non-void fn OR in void fn, but there is some expression after return => error
            return set_error(ERR_SEM_PARAMS);
        } else {
            Expression* expr = NULL;
            if(current_token->type != STT_SEMICOLON) {
                expr = expression_rule();
            }
            if(second_run) {
                instruction_insert_to_list(current_instructions, instruction_generate(IC_RETURN, expr, NULL, NULL));
            }
        }
    } else if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_IF) {// IF
        // (
        if(next_token()->type != STT_LEFT_PARENTHESE) return set_error(ERR_SYNTAX);
        //boolean expression in condition
        next_token();
        Expression* condition = bool_expression_rule();
        if(get_error()->type) return;
        // )
        if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);
        Instruction* jmp_to_else = NULL;
        if(second_run) {
            jmp_to_else = instruction_generate(IC_JMPFALSE, condition, NULL, NULL); // instruction to be jumped to is not set yet
            instruction_insert_to_list(current_instructions, jmp_to_else);
        }


        bool is_simple = false;
        next_token();
        if(current_token->type == STT_LEFT_BRACE) {
            // {
            // statements inside IF (ELSE)
            next_token();
            stat_list_rule(is_void, false);
            if(get_error()->type) return;
            // }
        } else {
            is_simple = true;
            //simple
            stat_rule(is_void, false);
            if(get_error()->type) return;
            prev_token();
        }
        if(!is_simple && current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
        Instruction* jmp_after_else = NULL;
        Instruction* nop_before_else = NULL;
        Instruction* nop_after_else = NULL;
        ListItem* item;
        if(second_run) {
            jmp_after_else = instruction_generate(IC_JMP, NULL, NULL, NULL); // instruction to be jumped to is not set yet
            instruction_insert_to_list(current_instructions, jmp_after_else);
        }

        if(second_run) {
            nop_before_else = instruction_generate(IC_NOP, NULL, NULL, NULL); // instruction to be jumped to is not set yet
            item = instruction_insert_to_list(current_instructions, nop_before_else);
            jmp_to_else->res = item;
        }

        // ELSE
        next_token();
        if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_ELSE) {
            is_simple = false;
            next_token();
            if(current_token->type == STT_LEFT_BRACE) {
                // {
                // statements inside IF (ELSE)
                next_token();
                stat_list_rule(is_void, false);
                if(get_error()->type) return;
                // }
            } else {
                is_simple = true;
                //simple
                stat_rule(is_void, false);
                if(get_error()->type) return;
                prev_token();
            }
            if(!is_simple && current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
            if(second_run) {
                nop_after_else = instruction_generate(IC_NOP, NULL, NULL, NULL); // instruction to be jumped to is not set yet
                item = instruction_insert_to_list(current_instructions, nop_after_else);
                jmp_after_else->res = item;
            }
        } else {
            prev_token();

            if(second_run) {
                nop_after_else = instruction_generate(IC_NOP, NULL, NULL, NULL); // instruction to be jumped to is not set yet
                item = instruction_insert_to_list(current_instructions, nop_after_else);
                jmp_after_else->res = item;
            }
        }
    } else if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_WHILE) { // WHILE
        //(
        if(next_token()->type != STT_LEFT_PARENTHESE) return set_error(ERR_SYNTAX);
        //boolean expression in condition
        next_token();
        Expression* condition = bool_expression_rule();
        if(get_error()->type) return;
        // )
        if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);


        ListItem* nop_before_while_cond = NULL;
        Instruction* jmp_after_while = NULL;
        if(second_run) {
            nop_before_while_cond = instruction_insert_to_list(current_instructions, instruction_generate(IC_NOP, NULL, NULL, NULL));
            jmp_after_while = instruction_generate(IC_JMPFALSE, condition, NULL, NULL); // instruction to be jumped to is not set yet
            instruction_insert_to_list(current_instructions, jmp_after_while);
        }

        next_token();
        if(current_token->type == STT_LEFT_BRACE) {
            // {
            //statements inside WHILE
            next_token();
            stat_list_rule(is_void, false);
            if(second_run) {
                instruction_insert_to_list(current_instructions, instruction_generate(IC_JMP, NULL, NULL, nop_before_while_cond));
            }
            // }
            if(current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
        } else {
            //simple
            stat_rule(is_void, false);
            prev_token();
            if(second_run) {
                instruction_insert_to_list(current_instructions, instruction_generate(IC_JMP, NULL, NULL, nop_before_while_cond));
            }
        }

        if(second_run) {
            Instruction* nop_after_while = instruction_generate(IC_NOP, NULL, NULL, NULL);
            ListItem* item = instruction_insert_to_list(current_instructions, nop_after_while);
            jmp_after_while->res = item;
        }
    } else if(current_token->type == STT_IDENT) {
        //check if symbol exists
        Ident* current_ident = current_token->data->id;
        Symbol* symbol = NULL;
        if(second_run) {
            symbol = context_find_ident(current_context, main_context, current_token->data->id);
            if(get_error()->type) return;
        }

        next_token();
        if(current_token->type == STT_LEFT_PARENTHESE) {
            if(!second_run) {
                next_token();
                call_params_list_rule(NULL, NULL);
            } else {
                //function call
                if(symbol->type != ST_FUNCTION) return set_error(ERR_OTHER_SEMANTIC);

                //list for params types
                list_activate_first(symbol->data.fn->params_types_list);
                next_token();
                List *call_params_list = list_init(LT_EXPRESSION);
                call_params_list_rule(symbol->data.fn->params_types_list, call_params_list);
                if(get_error()->type) return;
                if(symbol->data.fn->params_types_list->active != NULL) {
                    //fprintf(stderr, "Not all params supplied for fn: %s\n", str_get_str(symbol->name));
                    //fn params but call params ended
                    return set_error(ERR_SEM_PARAMS);
                }

                if(current_ident->class != NULL && str_cmp_const(current_ident->class, "ifj16") == 0) {
                    /*
                     *  substr, compare, find, sort generate a nop instruction because, they have no real use when not in expression
                     */

                    if(str_cmp_const(current_ident->name, "readInt") == 0) {
                        instruction_insert_to_list(current_instructions, instruction_generate(IC_READ_INT, NULL, call_params_list, NULL));
                    } else if(str_cmp_const(current_ident->name, "readDouble") == 0) {
                        instruction_insert_to_list(current_instructions, instruction_generate(IC_READ_DOUBLE, NULL, call_params_list, NULL));
                    } else if(str_cmp_const(current_ident->name, "readString") == 0) {
                        instruction_insert_to_list(current_instructions, instruction_generate(IC_READ_STRING, NULL, call_params_list, NULL));
                    } else if(str_cmp_const(current_ident->name, "print") == 0) {
                        instruction_insert_to_list(current_instructions, instruction_generate(IC_PRINT, NULL, call_params_list, NULL));
                    } else if(str_cmp_const(current_ident->name, "length") == 0) {
                        instruction_insert_to_list(current_instructions, instruction_generate(IC_NOP, NULL, NULL, NULL));
                        // instruction_insert_to_list(current_instructions, instruction_generate(IC_STR_LENGTH, NULL, call_params_list, NULL));
                    } else if(str_cmp_const(current_ident->name, "substr") == 0) {
                        instruction_insert_to_list(current_instructions, instruction_generate(IC_NOP, NULL, NULL, NULL));
                        // instruction_insert_to_list(current_instructions, instruction_generate(IC_STR_SORT, NULL, call_params_list, NULL));
                    } else if(str_cmp_const(current_ident->name, "compare") == 0) {
                        instruction_insert_to_list(current_instructions, instruction_generate(IC_NOP, NULL, NULL, NULL));
                        // instruction_insert_to_list(current_instructions, instruction_generate(IC_STR_FIND, NULL, call_params_list, NULL));
                    } else if(str_cmp_const(current_ident->name, "find") == 0) {
                        instruction_insert_to_list(current_instructions, instruction_generate(IC_NOP, NULL, NULL, NULL));
                        // instruction_insert_to_list(current_instructions, instruction_generate(IC_STR_SUBSTRING, NULL, call_params_list, NULL));
                    } else if(str_cmp_const(current_ident->name, "sort") == 0) {
                        instruction_insert_to_list(current_instructions, instruction_generate(IC_NOP, NULL, NULL, NULL));
                        // instruction_insert_to_list(current_instructions, instruction_generate(IC_STR_COMP, NULL, call_params_list, NULL));
                    } else {
                        // unknown fn
                        // redundat check (first checking in context_find_ident)
                        return set_error(ERR_SEMANTIC);
                    }
                } else {
                    //just general fn call
                    instruction_insert_to_list(current_instructions, instruction_generate(IC_CALL, symbol, call_params_list, NULL));
                }
            }
            if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);
            next_token();
            if(current_token->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);
            //generate CALL instruction
        } else if(current_token->type == STT_EQUALS) {
            if(second_run) {
                if(symbol->type != ST_VARIABLE) return set_error(ERR_OTHER_SEMANTIC);
            }
            next_token();
            Expression* expr = expression_rule();
            if(current_token->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);
            if(second_run) {
                instruction_insert_to_list(current_instructions, instruction_generate(IC_EVAL, expr, NULL, symbol));
            }
        } else {
            //fprintf(stderr, "Unexpected token: %s\n", token_to_string(current_token));
            return set_error(ERR_SYNTAX);
        }
    } else if(current_token->type == STT_KEYWORD_TYPE) {
        if(!can_define) {
            // fprintf(stderr, "Variable definition is not allowed here\n");
            return set_error(ERR_SYNTAX);
        }

        // variable definition
        Symbol* symbol = definition_rule();
        if(get_error()->type) return;
        next_token();
        if(current_token->type == STT_SEMICOLON) {
            // syntax OK
        } else if(current_token->type == STT_EQUALS) {
            next_token();
            Expression* expr = expression_rule();
            if(get_error()->type) return;
            if(current_token->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);

            if(second_run) {
                instruction_insert_to_list(current_instructions, instruction_generate(IC_EVAL, expr, NULL, symbol));
            }
        } else {
            set_error(ERR_SYNTAX);
        }
    } else {
        // fprintf(stderr, "Unexpected token: %s %s %d\n", token_to_string(current_token), __func__, __LINE__);
        return set_error(ERR_SYNTAX);
    }

    next_token();
}

Expression* bool_expression_rule() {
    if(current_token->type == STT_RIGHT_PARENTHESE) {
        set_error(ERR_SYNTAX);
        return NULL;
    }

    return general_expression_rule(STT_RIGHT_PARENTHESE, STT_EOF);
}

Expression* expression_rule() {
    if(current_token->type == STT_SEMICOLON) {
        set_error(ERR_SYNTAX);
        return NULL;
    }

    return general_expression_rule(STT_SEMICOLON, STT_EOF);
}

Expression* general_expression_rule(ScannerTokenType end_token, ScannerTokenType or_end_token) {
    Stack* term_stack = stack_init(StT_EXPRESSION);
    Stack* nonterm_stack = stack_init(StT_EXPRESSION);

    while(current_token->type != end_token && current_token->type != or_end_token) {
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
                data.expression->str = str_init_str(current_token->data->str);
                break;
            case STT_KEYWORD:
                //is keyword => check if is "true" or "false"
                if(current_token->data->keyword_type == KW_TRUE || current_token->data->keyword_type == KW_FALSE) {
                    is_term = true;
                    data.expression->op = EO_CONST_BOOL;
                    data.expression->b = current_token->data->keyword_type == KW_TRUE ? true : false;
                } else {
                    expression_dispose(data.expression);
                    set_error(ERR_SYNTAX);
                    return NULL;
                }
                break;
            case STT_IDENT:
            {
                is_term = true;
                Ident* current_ident = (Ident*)malloc(sizeof(Ident));
                current_ident->name = str_init_str(current_token->data->id->name);
                current_ident->class = NULL;
                if(current_token->data->id->class != NULL) {
                    current_ident->class = str_init_str(current_token->data->id->class);
                }

                Symbol* symbol = NULL;
                if(second_run) {
                    symbol = context_find_ident(current_context, main_context, current_ident);
                }
                if(get_error()->type) return NULL;
                if(next_token()->type == STT_LEFT_PARENTHESE) {
                    //function call
                    if(second_run) {
                        if(symbol->type != ST_FUNCTION) {
                            expression_dispose(data.expression);
                            set_error(ERR_SEMANTIC);
                            return NULL;
                        }
                    }

                    data.expression->op = EO_SYMBOL_CALL;

                    //list for params types
                    if(!second_run) {
                        next_token();
                        call_params_list_rule(NULL, NULL);
                    } else {
                        list_activate_first(symbol->data.fn->params_types_list);
                        next_token();
                        List *call_params_list = list_init(LT_EXPRESSION);
                        call_params_list_rule(symbol->data.fn->params_types_list, call_params_list);
                        if(get_error()->type) return NULL;
                        if(symbol->data.fn->params_types_list->active != NULL) {
                            //fprintf(stderr, "Not all params supplied for fn: %s\n", str_get_str(symbol->name));
                            //fn params but call params ended
                            expression_dispose(data.expression);
                            set_error(ERR_SEM_PARAMS);
                            return NULL;
                        }


                        data.expression->symbol = symbol;
                        data.expression->symbol->id = current_ident;
                        data.expression->call_params = call_params_list;
                    }
                    if(get_error()->type) return NULL;
                    if(current_token->type != STT_RIGHT_PARENTHESE) {
                        set_error(ERR_SYNTAX);
                        return NULL;
                    }
                } else {
                    prev_token();
                    data.expression->op = EO_SYMBOL;
                    if(second_run) {
                        if(symbol->type != ST_VARIABLE) {
                            expression_dispose(data.expression);
                            set_error(ERR_SEMANTIC);
                            return NULL;
                        }
                        data.expression->symbol = symbol;
                    }
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
                is_term = true;
                expression_dispose(data.expression);
                next_token();
                data.expression = general_expression_rule(STT_RIGHT_PARENTHESE, end_token);
                if(get_error()->type) {
                    expression_dispose(data.expression);
                    return NULL;
                }
                break;
            case STT_AND:
                data.expression->op = EO_AND;
                break;
            case STT_OR:
                data.expression->op = EO_OR;
                break;
            case STT_LESS:
                data.expression->op = EO_LESS;
                break;
            case STT_GREATER:
                data.expression->op = EO_GREATER;
                break;
            case STT_LESS_EQUALS:
                data.expression->op = EO_LESS_EQUALS;
                break;
            case STT_GREATER_EQUALS:
                data.expression->op = EO_GREATER_EQUALS;
                break;
            case STT_LOGIC_EQUAL:
                data.expression->op = EO_LOGIC_EQUAL;
                break;
            case STT_LOGIC_NOT_EQUAL:
                data.expression->op = EO_LOGIC_NOT_EQUAL;
                break;
            case STT_NEGATE:
                data.expression->op = EO_NEGATE;
                break;
            default:
                // fprintf(stderr, " Unexpected token %s\n", token_to_string(current_token));
                expression_dispose(data.expression);
                set_error(ERR_SYNTAX);
                return NULL;
                break;
        }

        if(is_term) {
            stack_push(term_stack, data);
        } else {
            StackItemData* top = stack_top(nonterm_stack);
            if(top != NULL && precedence_table[top->expression->op][data.expression->op] == N) {
                set_error(ERR_SYNTAX);
                return NULL;
            }

            //while there are ops on stack with priority equal or higher than current op => reduce
            while(top != NULL && precedence_table[top->expression->op][data.expression->op] > E) {
                // reducing nonterm array means poping 2 (or generally x) values from term stack
                // and assigning them to the top expr
                // the top expr will then be poped from nonterm stack and pushed to term stack

                switch (top->expression->op) {
                    case EO_NEGATE:
                    {
                        // negate only takes one expression
                        StackItem* term_top1 = stack_pop(term_stack);
                        if(term_top1 == NULL) {
                            set_error(ERR_SYNTAX);
                            return NULL;
                        }
                        top->expression->expr1 = term_top1->data.expression;
                        free(term_top1);

                        break;
                    }
                    default:
                    {
                        // first to ->expr2
                        StackItem* term_top2 = stack_pop(term_stack);
                        if(term_top2 == NULL) {
                            set_error(ERR_SYNTAX);
                            return NULL;
                        }
                        // second to ->expr1
                        StackItem* term_top1 = stack_pop(term_stack);
                        if(term_top1 == NULL) {
                            set_error(ERR_SYNTAX);
                            return NULL;
                        }

                        //set terms for current top
                        top->expression->expr1 = term_top1->data.expression;
                        top->expression->expr2 = term_top2->data.expression;
                        free(term_top1);
                        free(term_top2);

                        break;
                    }
                }

                //push the top to term
                stack_push(term_stack, *top);
                //pop it from nonterm
                stack_pop(nonterm_stack);


                // set new top
                top = stack_top(nonterm_stack);
                if(top != NULL && precedence_table[top->expression->op][data.expression->op] == N) {
                    set_error(ERR_SYNTAX);
                    return NULL;
                }
            }

            stack_push(nonterm_stack, data);
        }

        next_token();
    }

    //pop all remaining nonterms
    StackItemData* top = stack_top(nonterm_stack);
    while(top != NULL) {
        // reducing nonterm array means poping 2 (or generally x) values from term stack
        // and assigning them to the top expr
        // the top expr will then be poped from nonterm stack and pushed to term stack

        switch (top->expression->op) {
            case EO_NEGATE:
            {
                // negate only takes one expression
                StackItem* term_top1 = stack_pop(term_stack);
                if(term_top1 == NULL) {
                    set_error(ERR_SYNTAX);
                    return NULL;
                }
                top->expression->expr1 = term_top1->data.expression;
                free(term_top1);

                break;
            }
            default:
            {
                // first to ->expr2
                StackItem* term_top2 = stack_pop(term_stack);
                if(term_top2 == NULL) {
                    set_error(ERR_SYNTAX);
                    return NULL;
                }
                // second to ->expr1
                StackItem* term_top1 = stack_pop(term_stack);
                if(term_top1 == NULL) {
                    set_error(ERR_SYNTAX);
                    return NULL;
                }

                //set terms for current top
                top->expression->expr1 = term_top1->data.expression;
                top->expression->expr2 = term_top2->data.expression;
                free(term_top1);
                free(term_top2);

                break;
            }
        }

        //push the top to term
        stack_push(term_stack, *top);
        //pop it from nonterm
        stack_pop(nonterm_stack);


        // set new top
        top = stack_top(nonterm_stack);
    }

    StackItem* res = stack_pop(term_stack);
    if(stack_top(term_stack) != NULL) {
        // fprintf(stderr, "Cannot parse expression\n");
        set_error(ERR_SYNTAX);
        expression_dispose(res->data.expression);
        return NULL;
    }

    stack_dispose(term_stack);
    stack_dispose(nonterm_stack);
    return res->data.expression;
}
