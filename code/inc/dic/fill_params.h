#ifndef  FILL_PARAMS_H
#define FILL_PARAMS_H


#include "dic/instruction.h" //instruction et types





int fill_params_default(word in, Instruction *out);

// customisés

int fill_params_pop_push(word in, Instruction *out);

int parse_params(word mot, Plgtab *tab);
int parse_param(word mot, Plage p);

int parse_reg(word in, Instruction *out);
int parse_imm(word in, Instruction *out);
int parse_ext(word in, Instruction *out);



int parse_reglist(word in, Instruction *out);
int parse_push_reglist(word in, Instruction *out);
int parse_pop_reglist(word in, Instruction *out);

#endif