/**
 *  @defgroup Instruction
 *  @brief Helper module defining Instruction and its operations
 */


#ifndef INSTRUCTION_H
#define INSTRUCTION_H

/**
 *  Code of the instruction => says what to do
 *
 *  @ingroup Instruction
 */
typedef enum {
    IC_NOP, ///< NOP instruction => no action
    IC_MOV, ///< assign some value to `res`
    IC_JMP, ///< jump/goto some address
    IC_ADD, ///< res=op1+op2
    IC_SUBSTRACT, ///< res=op1-op2
    IC_MUL, ///< res=op1*op2
    IC_DIV, ///< res=op1/op2
    IC_EQUAL, ///< op1 == op2
    IC_NOTEQUAL, ///< op1 != op2
    IC_LESSER, ///< op1 < op2
    IC_GREATER, ///< op1 > op2
    IC_GREATEREQ, ///< op1 >= op2
    IC_LESSEREQ, ///< op1 <= op2
    IC_AND, ///< res = op1 && op2
    IC_OR, ///< res = op1 || op2
    IC_NOT, ///< res = !op1
    IC_READ_INT, ///< read int from stdout
    IC_READ_DOUBLE, ///< read double from stdout
    IC_READ_STRING, ///< read string from stdout
    IC_PRINT, ///< write to standart output
    IC_STR_LENGTH, ///< res = length of string
    IC_STR_SORT, ///< res = sorted strings
    IC_STR_FIND, ///< res = if find return -1 else 0
    IC_STR_SUBSTRING, ///< res = return substring between i(begin) to i(finish)
    IC_STR_COMP, ///< res = return 0 op1 == op2 , return 1 op1 > op2 , else return -1
    IC_EVAL,
    IC_TRUE_JUMP,
    IC_FALSE_JUMP,
} InstructionCode;

/**
 *  Structure defining an instruction
 *
 *  @ingroup Instruction
 */
typedef struct {
    InstructionCode code; ///< code/type of instruction. Defines the operation of the instruction
    void* op1; ///< first instruction operand
    void* op2; ///< second instruction operand
    void* res; ///< where to put result
} Instruction;

#endif
