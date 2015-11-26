#ifndef  INSDISASM_H
#define INSDISASM_H

#include "simul/instruction.h"




typedef Instruction Ins_disas;





int get_ins(word in, Instruction *out, Instruction dic[], int sz_dic);

int disasm_ins(word mot, Instruction *ins);
void disp_insd(Instruction ins);

int parse_params(word mot, Plgtab *tab);
int parse_param(word mot, Plage *p);



#endif