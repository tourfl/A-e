
#ifndef MOV_H
#define MOV_H

#include "dic/instruction.h" // Instruction et emulator
#include "simul/registre.h" // Registre



int mov_imm (Instruction ins, Emulator *emul);
 
int mov_imm_T1 (Instruction ins, Registre reg[NB_REG], int* setflags, int* imm32, int* carry);
int mov_imm_T2 (Instruction ins, Registre reg[NB_REG], int* setflags, int* imm32, int* carry, int* d);
int mov_imm_T3 (Instruction ins, Registre reg[NB_REG], int* setflags, int* imm32, int* d);

#endif