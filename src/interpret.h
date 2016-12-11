/*
 * Course: IFJ
 * Project name: Implementace interpretu imperativního jazyka IFJ16
 * Description: https://wis.fit.vutbr.cz/FIT/st/course-files-st.php/course/IFJ-IT/projects/ifj2016.pdf
 *
 *
 * Team:
 *  Dominik Križka   (xkrizk02)
 *  Matúš Juhász     (xjuhas02)
 *  Samuel Hulla     (xhulla00)
 *  Svätopluk Hanzel (xhanze10)
 *  Tomáš Haas       (xhaast00)
 */



/**
 * @defgroup Interpret
 * @brief Module containing functions for working with instructions
 */

 #ifndef INTERPRET_H
 #define INTERPRET_H

#include "ial.h"
#include "instruction.h"

/**
 * Main function of Interpret
 *
 * @ingroup Interpret
 */
void interpret(Context* main_context, List* instructions);
void interpretation_loop();
void process_frame();

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
 * Function for assign
 *
 * @param	op1	operand1
 * @param	res	result
 *
 * @return Result after assign
 * @ingroup Interpret
 */
void mov(Symbol* op1, Symbol* res);

/**
 * Wrapper Function for read integer from stdout
 *
 * @param	op1	operand1
 *
 * @return Read integer
 * @ingroup Interpret
 */
void read_int_stdin(Symbol* op1);

/**
 * Wrapper Function for read double from stdout
 *
 * @param	op1	operand1
 *
 * @return Read double
 * @ingroup Interpret
 */
void read_double_stdin(Symbol* op1);

/**
 * Wrapper Function for read string from stdout
 *
 * @param	op1	operand1
 *
 * @return Read string
 * @ingroup Interpret
 */
void read_str_stdin(Symbol* op1);

/**
 * Wrapper Function for detection length of string
 *
 * @param	op1	operand1
 * @param	res	result
 *
 * @return Return length of string
 * @ingroup Interpret
 */
void length_str(Symbol* op1, Symbol* res);

/**
 * Wrapper Function for search substring in string
 *
 * @param	op1	operand1
 * @param	op2	operand2
 * @param	op1	operand3
 * @param	res	result
 *
 * @return Return substring
 * @ingroup Interpret
 */
//void substring(Symbol* op1, Symbol* op2, Symbol* op3, Symbol *res);

/**
 * Wrapper Function for sompare  strings
 *
 * @param	op1	operand1
 * @param	op2	operand2
 * @param	res	result
 *
 * @return Return  0 for equal; value < 0 s1 is less than s2 ; value > 0 s2 is less than s1
 * @ingroup Interpret
 */
void compare_str(Symbol* op1, Symbol* op2, Symbol* res);

/**
 * Wrapper Function for sort of string in ascending order
 *
 * @param	op1	operand1
 * @param	res	result
 *
 * @return Return sorted string
 * @ingroup Interpret
 */
void sort_str(Symbol* op1, Symbol* res);

/**
 * Wrapper Function find string
 *
 * @param	op1	operand1
 * @param	res	result
 *
 * @return Return finded string
 * @ingroup Interpret
 */
void find_str(Symbol* op1, Symbol* op2, Symbol* res);

void call(Symbol* fn_symbol, List* params, Symbol* return_var, bool manage_frames);
void assign_value_to_variable(Symbol* symbol, Expression* expr);


#endif
