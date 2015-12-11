#ifndef DISPLAY_INS_H
#define DISPLAY_INS_H

#include "dic/instruction.h"



enum display_mode {NOT_DECODED, DECODED};




void display(Instruction ins, int flag);



void disp_name(Instruction ins);
void disp_regs(Plgtab regs);
void disp_reg_name(Plage p);
void disp_imm(Plgtab imm);



void disp_not_decoded(Instruction ins);


// Instructions désassemblées

void disp_default(Instruction ins);

// affichage customisé
void disp_sub_sp(Instruction ins);
void disp_pop_push(Instruction ins);
void disp_ldr(Instruction ins);
void disp_it(Instruction ins);



#endif