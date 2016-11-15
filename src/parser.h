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
 *  <class_list> -> <class><class_list>
 *
 *  @ingroup Parser
 */
void class_list_rule();

/**
 *  Parses class
 *
 *  Parses <class> rule
 *  <class> -> "class" ID  "{" <class_members> "}"
 *
 *  @ingroup Parser
 */
void class_rule();


/**
 *  Parses class members (properties, functins)
 *
 *  Parses <class_members> rule
 *  <class_members> -> <class_member><class_members>
 *
 *  @ingroup Parser
 */
void class_members_rule();


/**
 *  Parses class member (property, function)
 *
 *  Parses <class_member> rule(s)
 *  <class_member> -> "static" TYPE ID";"
 *  <class_member> -> "static" TYPE ID "=" <expr>";"
 *  <class_member> -> "static" TYPE ID "(" <params_list> ")" "{" <stat_list> "}"
 *
 *  @ingroup Parser
 */
void class_member_rule();

/**
 *  Parses parameters for function
 *
 *  Parses <params_list> rule
 *  <params_list> -> <definition>,<params_list>
 *
 *  @ingroup Parser
 */
void params_list_rule();

/**
 *  Parses variables definitions
 *
 *  Parses <definition> rule
 *  <definition> -> TYPE ID
 *
 *  @ingroup Parser
 */
void definition_rule();

/**
 *  Parses statements
 *
 *  Parses <stat_list> rule
 *  <stat_list> -> <stat><stat_list>
 *
 *  @ingroup Parser
 */
void stat_list_rule();

/**
 *  Parses statements
 *
 *  Parses <stat> rule
 *  <stat> -> "IF" "(" <expr> ")" "{" <stat_list> "}" [<else>]
 *  <stat> -> "while" "(" <expr> ")" "{" <stat_list> "}"
 *  <stat> -> ID "=" <expr> ";"
 *  <stat> -> ID "("<params_list>")"
 *
 *  @ingroup Parser
 */
void stat_rule();

/**
 *  Parses boolean expressions
 *
 *  Parses <bool_expr> rules
 *  <bool_expr> -> "(" <bool_expr> ")"
 *  <bool_expr> -> true
 *  <bool_expr> -> false
 *  <bool_expr> -> INT
 *  <bool_expr> -> ID
 *  <bool_expr> -> ID "(" <params_list> ")"
 *  <bool_expr> -> <bool_expr> "&&" <bool_expr>
 *  <bool_expr> -> <bool_expr> "||" <bool_expr>
 *  <bool_expr> -> <bool_expr> "!=" <bool_expr>
 *  <bool_expr> -> <bool_expr> "==" <bool_expr>
 *
 *  @ingroup Parser
 */
void bool_expression_rule();

/**
 *  Parses boolean expressions
 *
 *  Parses <expr> rules
 *  <expr> -> "(" <expr> ")"
 *  <expr> -> <bool_expr>
 *  <expr> -> <expr> "+" <expr>
 *  <expr> -> <expr> "-" <expr>
 *  <expr> -> <expr> "*" <expr>
 *  <expr> -> <expr> "/" <expr>
 *
 *  @ingroup Parser
 */
void expression_rule();

#endif
