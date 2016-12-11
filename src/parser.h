/*
 * Course: IFJ
 * Project name: Implementace interpretu imperativního jazyka IFJ16
 * Description: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf
 *
 *
 * Team:
 *  Dominik Križka   (xkrizk02)
 *  Samuel Hulla     (xhulla00)
 *  Svätopluk Hanzel (xhanze10)
 *  Tomáš Haas       (xhaast00)
 */



/**
 *  @defgroup Parser
 *  @brief Module responsible for syntax analysis and code generation
 */


#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "list.h"

/**
 *  Main parser function
 *
 *  Starts analysis, controls the whole process of parsing
 *
 *  @param[in] _token_list List of tokens to parse
 *  @param[out] _context Main context - will be used by interpret
 *  @param[out] _instructions List of Main instructions. Used to initialize class member variables and call Main.run
 *
 *  @ingroup Parser
 */
void parse(List* _token_list, Context** _context, List** _instructions);

/**
 *  Starts the actual parsing
 *
 *  Starts by parseing the <class_list> rule
 *  <class_list> -> <class><class_list><br>
 *
 *  @ingroup Parser
 */
void class_list_rule();

/**
 *  Parses class
 *
 *  Parses <class> rule<br>
 *  <class> -> "class" ID  "{" <class_members> "}"<br>
 *
 *  @ingroup Parser
 */
void class_rule();


/**
 *  Parses class members (properties, functins)
 *
 *  Parses <class_members> rule<br>
 *  <class_members> -> <class_member><class_members><br>
 *
 *  @ingroup Parser
 */
void class_members_rule();


/**
 *  Parses class member (property, function)
 *
 *  Parses <class_member> rule(s)<br>
 *  <class_member> -> "static" TYPE ID";"<br>
 *  <class_member> -> "static" TYPE ID "=" <expr>";"<br>
 *  <class_member> -> "static" TYPE ID "(" <params_list> ")" "{" <stat_list> "}"<br>
 *
 *  @ingroup Parser
 */
void class_member_rule();

/**
 *  Parses parameters for function
 *
 *  Parses <params_list> rule<br>
 *  <params_list> -> <definition>,<params_list><br>
 *
 *  @param[out] params_names_list List of `VariableType`s (for detecting sem errors)
 *  @param[out] params_ids_list List of `Ident`s
 *
 *  @ingroup Parser
 */
 void params_list_rule(List* params_names_list, List* params_ids_list);

/**
 *  Parses parameters for function
 *
 *  Parses <params_list> rule<br>
 *  <call_params_list> -> <expr>,<call_params_list><br>
 *
 *  @param[out] fn_params_list List of `VariableType`s (for detecting sem errors)
 *  @param[out] call_params_list List of `Expressions`s for call params
 *
 *  @ingroup Parser
 */
void call_params_list_rule(List *fn_params_list, List *call_params_list);

/**
 *  Parses variables definitions
 *
 *  Parses <definition> rule<br>
 *  <definition> -> TYPE ID<br>
 *
 *  @ingroup Parser
 */
Symbol* definition_rule();

/**
 *  Parses statements
 *
 *  Parses <stat_list> rule<br>
 *  <stat_list> -> <stat><stat_list><br>
 *
 *  @ingroup Parser
 */
void stat_list_rule(bool is_void, bool can_define);

/**
 *  Parses statements
 *
 *  Parses <stat> rule<br>
 *  <stat> -> "IF" "(" <expr> ")" "{" <stat_list> "}" [<else>]<br>
 *  <stat> -> "while" "(" <expr> ")" "{" <stat_list> "}"<br>
 *  <stat> -> TYPE ID";"<br>
 *  <stat> -> TYPE ID "=" <expr> ";"<br>
 *  <stat> -> ID "=" <expr> ";"<br>
 *  <stat> -> ID "("<params_list>")"<br>
 *
 *  @ingroup Parser
 */
void stat_rule(bool is_void, bool can_define);

/**
 *  Parses expression in conditions
 *
 *  @param[in] is_static_class_member Should the ID be added to main context's symbol table as "`current_class`.`id`"?
 *
 *  @ingroup Parser
 */
Expression* bool_expression_rule();

/**
 *  Parses expressions in assignments
 *
 *  @ingroup Parser
 */
Expression* expression_rule();

/**
 * Abstraction of expression rules
 *
 *  <expr> -> "(" <expr> ")"<br>
 *  <expr> -> true<br>
 *  <expr> -> false<br>
 *  <expr> -> INT or DOUBLE or STRING literal <br>
 *  <expr> -> ID<br>
 *  <expr> -> ID "(" <params_list> ")"<br>
 *  <expr> -> <expr> "&&" <expr><br>
 *  <expr> -> <expr> "||" <expr><br>
 *  <expr> -> <expr> "!=" <expr><br>
 *  <expr> -> <expr> "==" <expr><br>
 *  <expr> -> <bool_expr><br>
 *  <expr> -> <expr> "+" <expr><br>
 *  <expr> -> <expr> "-" <expr><br>
 *  <expr> -> <expr> "*" <expr><br>
 *  <expr> -> <expr> "/" <expr><br>
 *
 */
Expression* general_expression_rule (ScannerTokenType end_token, ScannerTokenType or_end_token);

#endif
