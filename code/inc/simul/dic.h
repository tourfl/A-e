
#ifndef  DIC_H
#define DIC_H

#include <stdio.h>
#include "types.h" // word et Strlist
#include "simul/instruction.h"
#include <stdlib.h> // FILE

#define TAILLE_MAX 1000









 typedef struct 
 {
 	Instruction* ins32;
 	Instruction* ins16;
 	int sz32;
 	int sz16;

 } Dic;







Dic* init_dic();
void del_dic(Dic *dic);

int disp_dic(Dic *dic);

int load_dic(Dic *dic);

int get_nb_ins(FILE *fd);
int load_ins_tab_from_file(Instruction *dic, int dic_sz, FILE *fd);

#endif
