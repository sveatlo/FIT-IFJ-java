/**
 *  @defgroup Parser
 *  @brief Module responsible for syntax analysis and code generation
 */


#ifndef PARSER_H
#define PARSER_H

#include "list.h"

/**
 *  Main parser function
 *
 *  Starts analysis, controls the whole process of parsing
 *
 *  @ingroup Parser
 */
void parse(List*);

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
 *  @param[out] params_list List of `VariableType`s (for detecting sem errors)
 *
 *  @ingroup Parser
 */
void params_list_rule(List* params_list);

/**
 *  Parses parameters for function
 *
 *  Parses <params_list> rule<br>
 *  <call_params_list> -> <definition>,<call_params_list><br>
 *
 *  @param[out] params_list List of `VariableType`s (for detecting sem errors)
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
void definition_rule();

/**
 *  Parses statements
 *
 *  Parses <stat_list> rule<br>
 *  <stat_list> -> <stat><stat_list><br>
 *
 *  @ingroup Parser
 */
void stat_list_rule();

/**
 *  Parses statements
 *
 *  Parses <stat> rule<br>
 *  <stat> -> "IF" "(" <expr> ")" "{" <stat_list> "}" [<else>]<br>
 *  <stat> -> "while" "(" <expr> ")" "{" <stat_list> "}"<br>
 *  <stat> -> ID "=" <expr> ";"<br>
 *  <stat> -> ID "("<params_list>")"<br>
 *
 *  @ingroup Parser
 */
void stat_rule();

/**
 *  Parses boolean expressions
 *
 *  Parses <bool_expr> rules<br>
 *  <bool_expr> -> "(" <bool_expr> ")"<br>
 *  <bool_expr> -> true<br>
 *  <bool_expr> -> false<br>
 *  <bool_expr> -> INT<br>
 *  <bool_expr> -> ID<br>
 *  <bool_expr> -> ID "(" <params_list> ")"<br>
 *  <bool_expr> -> <bool_expr> "&&" <bool_expr><br>
 *  <bool_expr> -> <bool_expr> "||" <bool_expr><br>
 *  <bool_expr> -> <bool_expr> "!=" <bool_expr><br>
 *  <bool_expr> -> <bool_expr> "==" <bool_expr><br>
 *
 *  @param[in] is_static_class_member Should the ID be added to main context's symbol table as "`current_class`.`id`"?
 *
 *  @ingroup Parser
 */
void definition_rule();

void bool_expression_rule();

/**
 *  Parses expressions
 *
 *  Parses <expr> rules<br>
 *  <expr> -> "(" <expr> ")"<br>
 *  <expr> -> <bool_expr><br>
 *  <expr> -> <expr> "+" <expr><br>
 *  <expr> -> <expr> "-" <expr><br>
 *  <expr> -> <expr> "*" <expr><br>
 *  <expr> -> <expr> "/" <expr><br>
 *
 *  @ingroup Parser
 */
void expression_rule();

#endif
