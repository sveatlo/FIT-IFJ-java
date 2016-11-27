#include <stdio.h>
#include "context.h"
#include "error.h"
#include "expression.h"
#include "ial.h"
#include "instruction.h"
#include "list.h"
#include "parser.h"
#include "precedence_table.h"
#include "scanner_token.h"
#include "stack.h"

static List* token_list; ///< list of tokens
static ScannerToken* current_token; ///< token currently being processed
static Context* main_context; ///< main context
static Context* current_context; ///< context, which is currently being used
static SymbolName current_class_name; ///< name of current class
static List* main_instructions;    ///< list of main instructions - these are used for members initialization and to call Main.run fn
static List* current_instructions; ///< list of instructions of current function


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
    current_class_name = NULL;
    main_instructions = list_init();
    current_instructions = main_instructions;


    // start the actual parsing
    // first thing in file should be class (or several)
    class_list_rule();

    Ident id = {
        .class = str_init_const("Main"),
        .name = str_init_const("run")
    };

    Symbol* main_run_fn = context_find_ident(main_context, main_context, &id);
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
        Expression* expr = expression_rule();
        if(get_error()->type) {
            return;
        }
        instruction_insert_to_list(main_instructions, instruction_generate(IC_EVAL, expr, NULL, symbol));
    } else  if(current_token->type == STT_LEFT_PARENTHESE) {
        // this is a function
        // but also class member
        //    =>
        // to the class context, add name
        // change context to local function's context
        // swith back after the function

        Symbol* fn_symbol = context_add_function(current_context, current_type, current_ident->data->id->name);
        current_context = fn_symbol->data.fn->context;
        current_instructions = fn_symbol->data.fn->instructions;

        next_token();
        params_list_rule(fn_symbol->data.fn->params_list);
        if(get_error()->type) {
            return;
        }
        if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        next_token();
        stat_list_rule(current_type == KW_VOID, true);
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
Symbol* definition_rule() {
    if(current_token->type != STT_KEYWORD_TYPE) {
        set_error(ERR_SYNTAX);
        return NULL;
    }

    KeywordType current_type = current_token->data->keyword_type;
    //token must be IDENT and must be simple (no class part)
    if(next_token()->type != STT_IDENT || current_token->data->id->class != NULL) {
        set_error(ERR_SYNTAX);
        return NULL;
    }

    //add variable to current context
    return context_add_variable(current_context, current_type, current_token->data->id->name);
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
        if(is_void) {
            //error
            fprintf(stderr, "Return in void fn\n");
            return set_error(ERR_SEMANTIC);
        } else {
            Expression* expr = expression_rule();
            instruction_insert_to_list(current_instructions, instruction_generate(IC_RETURN, expr, NULL, NULL));
        }
    } else if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_IF) {// IF
        // (
        if(next_token()->type != STT_LEFT_PARENTHESE) return set_error(ERR_SYNTAX);
        //boolean expression in condition
        next_token();
        Expression* condition = bool_expression_rule();
        // )
        if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);
        Instruction* jmp_to_else = instruction_generate(IC_JMPFALSE, condition, NULL, NULL); // instruction to be jumped to is not set yet
        instruction_insert_to_list(current_instructions, jmp_to_else);

        // {
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        // statements inside IF
        next_token();
        stat_list_rule(is_void, false);
        Instruction* jmp_after_else = instruction_generate(IC_JMP, NULL, NULL, NULL); // instruction to be jumped to is not set yet
        instruction_insert_to_list(current_instructions, jmp_after_else);
        // }
        if(current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);

        Instruction* nop_before_else = instruction_generate(IC_NOP, NULL, NULL, NULL); // instruction to be jumped to is not set yet
        ListItem* item = instruction_insert_to_list(current_instructions, nop_before_else);
        jmp_to_else->res = item;

        // ELSE
        next_token();
        if(current_token->type != STT_KEYWORD || current_token->data->keyword_type != KW_ELSE) return set_error(ERR_SYNTAX);
        // {
        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        // statements inside IF
        next_token();
        stat_list_rule(is_void, false);
        // }
        if(current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
        Instruction* nop_after_else = instruction_generate(IC_NOP, NULL, NULL, NULL); // instruction to be jumped to is not set yet
        item = instruction_insert_to_list(current_instructions, nop_after_else);
        jmp_after_else->res = item;
    } else if(current_token->type == STT_KEYWORD && current_token->data->keyword_type == KW_WHILE) { // WHILE
        //(
        if(next_token()->type != STT_LEFT_PARENTHESE) return set_error(ERR_SYNTAX);
        //boolean expression in condition
        next_token();
        Expression* condition = bool_expression_rule();
        // )
        if(current_token->type != STT_RIGHT_PARENTHESE) return set_error(ERR_SYNTAX);

        // {
        ListItem* nop_before_while_cond = instruction_insert_to_list(current_instructions, instruction_generate(IC_NOP, NULL, NULL, NULL));
        Instruction* jmp_after_while = instruction_generate(IC_JMPFALSE, condition, NULL, NULL); // instruction to be jumped to is not set yet
        instruction_insert_to_list(current_instructions, jmp_after_while);

        if(next_token()->type != STT_LEFT_BRACE) return set_error(ERR_SYNTAX);
        //statements inside WHILE
        next_token();
        stat_list_rule(is_void, false);
        instruction_insert_to_list(current_instructions, instruction_generate(IC_JMP, NULL, NULL, nop_before_while_cond));
        // }

        if(current_token->type != STT_RIGHT_BRACE) return set_error(ERR_SYNTAX);
        Instruction* nop_after_while = instruction_generate(IC_NOP, NULL, NULL, NULL);
        ListItem* item = instruction_insert_to_list(current_instructions, nop_after_while);
        jmp_after_while->res = item;
    } else if(current_token->type == STT_IDENT) {
        //check if symbol exists
        Symbol* symbol = context_find_ident(current_context, main_context, current_token->data->id);
        if(get_error()->type) {
            //ident not found
            //try built-in fns
            if(current_token->data->id && current_token->data->id->class && str_cmp_const(current_token->data->id->class, "ifj16") == 0) {
                //this really is a built-in function => remove error
                set_error(ERR_NONE);

                // TODO: add builtin fn support
            } else {
                return;
            }
        }

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
            //generate CALL instruction
            instruction_insert_to_list(current_instructions, instruction_generate(IC_CALL, symbol, call_params_list, NULL));
        } else if(current_token->type == STT_EQUALS) {
            next_token();
            Expression* expr = expression_rule();
            if(current_token->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);
            instruction_insert_to_list(current_instructions, instruction_generate(IC_EVAL, expr, NULL, symbol));
        } else {
            fprintf(stderr, "Unexpected token: %s\n", token_to_string(current_token));
            return set_error(ERR_SYNTAX);
        }
    } else if(current_token->type == STT_KEYWORD_TYPE) {
        if(!can_define) {
            fprintf(stderr, "Variable definition is not allowed here\n");
            return set_error(ERR_SEMANTIC);
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
            if(current_token->type != STT_SEMICOLON) return set_error(ERR_SYNTAX);

            instruction_insert_to_list(current_instructions, instruction_generate(IC_EVAL, expr, NULL, symbol));
        } else {
            set_error(ERR_SYNTAX);
        }
    } else {
        return set_error(ERR_SYNTAX);
    }
    next_token();
}

Expression* bool_expression_rule() {
    return general_expression_rule(STT_RIGHT_PARENTHESE);
}

Expression* expression_rule() {
    return general_expression_rule(STT_SEMICOLON);
}

Expression* general_expression_rule(ScannerTokenType end_token) {
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
            default:
                set_error(ERR_SYNTAX);
                break;
        }

        if(is_term) {
            stack_push(term_stack, data);
        } else {
            StackItemData* top = stack_top(nonterm_stack);
            if(data.expression->op == EO_RIGHT_PARENTHESE) {
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
                    stack_pop(term_stack);

                    //set terms for current top
                    top->expression->expr2 = term_top2->expression;
                    top->expression->expr1 = term_top1->expression;

                    //push the top to term
                    stack_push(term_stack, *top);
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
                    //pop it from nonterm
                    stack_pop(nonterm_stack);


                    // set new top
                    top = stack_top(nonterm_stack);
                }

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

    return res->data.expression;
}
