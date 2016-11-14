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

#endif
