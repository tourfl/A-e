
#ifndef PREPROCESS_H
#define PREPROCESS_H


#include "dic/instruction.h" // Instruction


int preprocess_default(Instruction *out);





// customisés

int preprocess_B(Instruction *out);
int preprocess_BL(Instruction *out);
int preprocess_pop_push(Instruction *out);
int preprocess_sub_sp(Instruction *out);
int preprocess_ldr(Instruction *out);

int preprocess_mov_reg_16(Instruction *out);
int preprocess_add_reg_16(Instruction *out);


int preprocess_reglist(Instruction *out);
int preprocess_push_reglist(Instruction *out);
int preprocess_pop_reglist(Instruction *out);



#endif