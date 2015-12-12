
#ifndef PREPROCESS_H
#define PREPROCESS_H


#include "dic/instruction.h" // Instruction


int preprocess_default(Instruction *out, Emulator *emul);





// customisés

int preprocess_B(Instruction *out, Emulator *emul);
int preprocess_BL(Instruction *out, Emulator *emul);
int preprocess_pop_push(Instruction *out, Emulator *emul);
int preprocess_sub_sp(Instruction *out, Emulator *emul);
int preprocess_ldr(Instruction *out, Emulator *emul);

int preprocess_mov_reg_16(Instruction *out, Emulator *emul);
int preprocess_add_reg_16(Instruction *out, Emulator *emul);

int preprocess_it(Instruction *out, Emulator *emul);


int preprocess_reglist(Instruction *out);
int preprocess_push_reglist(Instruction *out);
int preprocess_pop_reglist(Instruction *out);



#endif