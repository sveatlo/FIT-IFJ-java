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



#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"

Instruction* instruction_init() {
    Instruction* instruction = (Instruction*)malloc(sizeof(Instruction));
    instruction->code = IC_NOP;
    instruction->op1 = NULL;
    instruction->op2 = NULL;
    instruction->res = NULL;

    return instruction;
}

void instruction_dispose(Instruction* instruction) {
    if(instruction != NULL) {
        free(instruction);
    }
}

Instruction* instruction_generate(InstructionCode code, void* op1, void* op2, void* res) {
    Instruction* instruction = instruction_init();
    instruction->code = code;
    instruction->op1 = op1;
    instruction->op2 = op2;
    instruction->res = res;

    return instruction;
}

ListItem *instruction_insert_to_list(struct ListStruct* instructions, Instruction* instruction) {
    ListItemData item_data = {
        .instruction = instruction
    };

    return list_insert_last(instructions, item_data);
}

void instruction_print(Instruction* instruction) {
    char instructions[][255] = {
        [IC_NOP] = "IC_NOP",
        [IC_EVAL] = "IC_EVAL",
        [IC_RETURN] = "IC_RETURN",
        [IC_JMP] = "IC_JMP",
        [IC_JMPFALSE] = "IC_JMPFALSE",
        [IC_JMPTRUE] = "IC_JMPTRUE",
        [IC_CALL] = "IC_CALL",
        [IC_MOV] = "IC_MOV",
        [IC_ADD] = "IC_ADD",
        [IC_SUBSTRACT] = "IC_SUBSTRACT",
        [IC_MUL] = "IC_MUL",
        [IC_DIV] = "IC_DIV",
        [IC_EQUAL] = "IC_EQUAL",
        [IC_NOTEQUAL] = "IC_NOTEQUAL",
        [IC_LESSER] = "IC_LESSER",
        [IC_GREATER] = "IC_GREATER",
        [IC_GREATEREQ] = "IC_GREATEREQ",
        [IC_LESSEREQ] = "IC_LESSEREQ",
        [IC_AND] = "IC_AND",
        [IC_OR] = "IC_OR",
        [IC_NOT] = "IC_NOT",
        [IC_READ_INT] = "IC_READ_INT",
        [IC_READ_DOUBLE] = "IC_READ_DOUBLE",
        [IC_READ_STRING] = "IC_READ_STRING",
        [IC_PRINT] = "IC_PRINT",
        [IC_STR_LENGTH] = "IC_STR_LENGTH",
        [IC_STR_SORT] = "IC_STR_SORT",
        [IC_STR_FIND] = "IC_STR_FIND",
        [IC_STR_SUBSTRING] = "IC_STR_SUBSTRING",
        [IC_STR_COMP] = "IC_STR_COMP"
    };

    printf("%s\n", instructions[instruction->code]);
}
