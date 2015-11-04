
#ifndef  DIC_H
#define DIC_H

#include <stdio.h>
#include "com/types.h" // word

#define TAILLE_MAX 1000

#define NB_INS_32 20
#define NB_INS_16 20

typedef struct
{
	char *commande;
	char *encoding; // T1, T2, T3 ou T4
	char *name_in_IT;
	char *name_out_IT;
	word mask;
	word opcode;
	int nb_param;


} Instruction;

void init_ins(Instruction *ins);
void del_ins(Instruction *ins);

int load_ins(Instruction *ins, char *chaine);
void disp_ins(Instruction ins);

Instruction get_ins(word in, Instruction ins[]);

 typedef struct 
 {
 	Instruction ins32[NB_INS_32];
 	Instruction ins16[NB_INS_16];

 } Dic;


void init_dic(Dic *dic);
void del_dic(Dic *dic);

int disp_dic();

int load_dic(Dic *dic);

// fonctions relatives à Instruction nécessitant Dic

Instruction get_ins32(word in, Dic *dic);
Instruction get_ins16(word in, Dic *dic);

int comp_instr(); // TODO


char* int_to_bin(unsigned int n, char taille);

char* bin_x_bin(char bin_1[], char bin_2[], int taille); // TODO

#endif
