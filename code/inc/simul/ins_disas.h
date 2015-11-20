#ifndef  INSDISASM_H
#define INSDISASM_H

#include "simul/instruction.h"
#include "simul/dic.h"




typedef Instruction Ins_disas;







int get_ins(word in, Instruction *out, Instruction dic[], int sz_dic);
int get_ins32(word in, Instruction *out, Dic *dic);
int get_ins16(word in, Instruction *out, Dic *dic);

void cpy_ins(Ins_disas *dest, Instruction *src);

void disp_insd(Instruction ins);


#endif