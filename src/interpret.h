/**
 * @defgroup Interpret
 * @brief Module containing functions for working with instructions
 */

 #ifndef INTERPRET_H
 #define INTERPRET_H

#include "ial.h"
#include "instruction.h"

/**
 * Function for mathematical instruction
 *
 * @param	op1	operand1
 * @param	op2	operand2
 * @param	res	result
 *
 * @return Result of mathematichal instructions
 * @ingroup Interpret
 */
void math_ins(Symbol *op1, Symbol *op2, Symbol *res, char c);

/**
 * Function for comparative instruction
 *
 * @param	op1	operand1
 * @param	op2	operand2
 * @param	res	result
 *
 * @return Result of comparative instructions
 * @ingroup Interpret
 */
 void compare_ins(InstructionCode code, Symbol *op1, Symbol *op2, Symbol *res);

 /**
  * Function for logical instruction
  *
  * @param	op1	operand1
  * @param	op2	operand2
  * @param	res	result
  *
  * @return Result of logical instructions
  * @ingroup Interpret
  */
void logic_ins(Symbol *op1, Symbol *op2, Symbol *res, char x); // namiesto char x moze byt InstructionCode code

/**
 * Main function of Interpret
 *
 * @ingroup Interpret
 */
void interpret();

#endif
