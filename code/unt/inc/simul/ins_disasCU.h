#ifndef INS_DISASCU_H
#define INS_DISASCU_H


#include "simul/dic.h"
#include <CUnit/CUnit.h>




int CU_init_ins_disa_suite();
int CU_del_ins_disa_suite();

void CUfind();
void CUfind_and_disasm();

#endif