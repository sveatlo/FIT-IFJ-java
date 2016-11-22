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
    IC_DIV ///< res=op1/op2
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