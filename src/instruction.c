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
