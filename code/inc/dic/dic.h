
#ifndef  DIC_H
#define DIC_H

#include <stdio.h>
#include "types.h" // word et Strlist
#include <stdlib.h> // FILE
#include "dic/instruction.h"

#define TAILLE_MAX 1000




// pour le problème d'inclusions circulaires
struct ins;



 struct dic
 {
 	struct ins* ins32;
 	struct ins* ins16;
 	int sz32;
 	int sz16;

 };

 typedef struct dic Dic;







Dic* init_dic();
void del_dic(Dic *dic);

int disp_dic(Dic *dic);

int load_dic(Dic *dic);
int load_from_file(struct ins **p_instab, int *nb_ins, char* filename);
int load_instab(struct ins *instab, int nb_ins, FILE *fd);
int get_nb_ins(FILE *fd);

int decode(word in, struct ins *out, Dic *dic);


int get_ins32(word in, struct ins *out, Dic *dic);
int get_ins16(word in, struct ins *out, Dic *dic);


#endif
