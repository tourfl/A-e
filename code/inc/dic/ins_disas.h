#ifndef  INSDISASM_H
#define INSDISASM_H

#include "dic/instruction.h"




typedef Instruction Ins_disas;





int get_ins(word in, Instruction *out, Instruction dic[], int sz_dic);


int fill_params(word in, Instruction *out);
int parse_params(word mot, Plgtab *tab);
int parse_param(word mot, Plage p);



#endif