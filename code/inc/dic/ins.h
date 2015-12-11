#ifndef  INS_H
#define INS_H

#include "emulator.h"
#include "elf/bits.h"
#include "dic/code_arm.h"



int mul (Instruction ins, Emulator* emul );
int mul_T1 (Instruction ins,int* d,int* m, int* setflags);
int mul_T2 (Instruction ins, int* d, int* n, int* m, int* setflags);

int movt (Instruction ins , Emulator* emul);
int movt_T1 (Instruction ins, int* d);

int add_imm (Instruction ins, Emulator* emul);
int add_imm_T1(Instruction ins,  int* setflags);
int add_imm_T2(Instruction ins,  int* setflags);
int add_imm_T3(Instruction ins,  int* setflags , int* n, int* d);
int add_imm_T4(Instruction ins,  int* setflags, int* n, int* d );

int cmp_imm (Instruction ins, Emulator* emul);
int cmp_imm_T1 (Instruction ins);
int cmp_imm_T2 (Instruction ins,int* n);

int sub_imm (Instruction ins, Emulator* emul);
int sub_imm_T1(Instruction ins,  int* setflags);
int sub_imm_T2(Instruction ins,  int* setflags);
int sub_imm_T3(Instruction ins,  int* setflags , int* n, int* d);
int sub_imm_T4(Instruction ins,  int* setflags, int* n, int* d );

int mov_imm (Instruction ins, Emulator* emul);
int mov_imm_T1 (Instruction ins, Emulator* emul, int* setflags, int* imm32, int* carry);
int mov_imm_T2 (Instruction ins, int* setflags, int* imm32, int* carry, int* d);
int mov_imm_T3 (Instruction ins, int* setflags, int* imm32, int* d);

int mov_reg (Instruction ins, Emulator* emul);
int mov_reg_T1 (Instruction ins, int* setflags,int* d);
int mov_reg_T2 (Instruction ins, int* setflags);
int mov_reg_T3 (Instruction ins, int* setflags,int* d, int* m);

int pop (Instruction ins, Emulator* emul);
int pop_T1 (Instruction ins, long* registers);
int pop_T2 (Instruction ins, long* registers);
int pop_T3 (Instruction ins , long* registers , int* t);

int ldr_imm (Instruction ins, Emulator* emul);
int ldr_imm_T1 (Instruction ins, int* imm32, int* index, int* add, int* wback, int* n, int* t);
int ldr_imm_T2 (Instruction ins, int* imm32, int* index, int* add, int* wback, int* n, int* t);
int ldr_imm_T3 (Instruction ins, int* imm32, int* index, int* add, int* wback, int* n, int* t);



#endif
