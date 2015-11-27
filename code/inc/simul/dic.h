
#ifndef  DIC_H
#define DIC_H

#include <stdio.h>
#include "types.h" // word et Strlist
#include <stdlib.h> // FILE
#include "simul/instruction.h"

#define TAILLE_MAX 1000




// pour le problème d'inclusions circulaires
struct ins;
typedef struct ins Instruction;



 struct dic
 {
 	Instruction* ins32;
 	Instruction* ins16;
 	int sz32;
 	int sz16;

 };

 typedef struct dic Dic;







Dic* init_dic();
void del_dic(Dic *dic);

int disp_dic(Dic *dic);

int load_dic(Dic *dic);

int get_nb_ins(FILE *fd);
int load_ins_tab_from_file(Instruction *dic, int dic_sz, FILE *fd);

int find(word in, Instruction *out, Dic *dic);
int find_and_decode(word mot, Instruction *out, Dic *dic);

int get_ins32(word in, Instruction *out, Dic *dic);
int get_ins16(word in, Instruction *out, Dic *dic);


#endif
