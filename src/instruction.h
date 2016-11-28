/**
 *  @defgroup Instruction
 *  @brief Helper module defining Instruction and its operations
 */


#ifndef INSTRUCTION_H
#define INSTRUCTION_H

struct ListStruct;
struct ListItemStruct;

#include "list.h"

/**
 *  Code of the instruction => says what to do
 *
 *  @ingroup Instruction
 */
typedef enum {
    IC_NOP, ///< NOP instruction => no action
    IC_EVAL, ///< Evaluates Expression (op1) and saves the result to res
    IC_RETURN,  ///< Special instruction to return from current call scope
    IC_JMP, ///< jump/goto some address
    IC_JMPFALSE, ///< Evaluates Expression (op1) and jump (set as active in list) to Instruction (res) if op1 == false
    IC_JMPTRUE, ///< Evaluates Expression (op1) and jump (set as active in list) to Instruction (res) if op1 == true
    IC_CALL,
    IC_MOV, ///< assign some value to `res`
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
    IC_JUMP_TRUE,
    IC_JUMP_FALSE,
} InstructionCode;

/**
 *  Structure defining an instruction
 *
 *  @ingroup Instruction
 */
typedef struct InstructionStruct {
    InstructionCode code; ///< code/type of instruction. Defines the operation of the instruction
    void* op1; ///< first instruction operand
    void* op2; ///< second instruction operand
    void* res; ///< where to put result
} Instruction;

/**
 *  Allocates memory for new instruction. Initializes it
 *
 *  @ingroup Instruction
 */
Instruction* instruction_init();

/**
 *  Frees the instruction memory
 *
 *  @ingroup Instruction
 */
void instruction_dispose(Instruction* instruction);

/**
 *  Generates new instruction
 *
 *  @ingroup Instruction
 */
Instruction* instruction_generate(InstructionCode code, void* op1, void* op2, void* res);

/**
 *  Insert instruction to `instructions` list
 *
 *  @ingroup Instruction
 */
struct ListItemStruct *instruction_insert_to_list(struct ListStruct* instructions, Instruction* instruction);

void instruction_print(Instruction* instruction);

#endif
