
#ifndef  DIC_H
#define DIC_H

#include <stdio.h>
#include "com/types.h" // word et Strlist

#define TAILLE_MAX 1000

#define NB_INS_32 2
#define NB_INS_16 0

/*
 * Explications & exemples : cf lib/instructions.md
 *
 */

 int to_strlist(char *chaine, Strlist *liste); // fonction ultra sécurisée !

// Ins_disasmd = instruction désassemblée, les Strlist contiennent les valeurs extraites du (demi-)mot lu

typedef struct
{
	// Général:
	char* commande;
	char* encoding;
	char* name_in;
	char* name_out;
	unsigned int mask;
	unsigned int opcode;
 

	// Paramètres :
	Strlist reg;
	Strlist imm;
	Strlist ext;
	
	// Pointeur de fonction: 		


} Instruction, Ins_disasmd;

void init_ins(Instruction *ins);
void del_ins(Instruction *ins);

int load_ins(Instruction *ins, char *chaine);
void disp_ins(Instruction ins);

Instruction get_ins(word in, Instruction ins[], int taille);

void init_ins_d(Ins_disasmd *ins_d, Instruction *ins);

int parse_params(word mot, Strlist *strl, Strlist *strl_d);
int parse_param(word mot, char *plage, char **value_d);

int get_bounds(char* plage, int *start, int *end);

 typedef struct 
 {
 	Instruction* ins32;
 	Instruction* ins16;

 } Dic;



void del_dic(Dic *dic);

int disp_dic();

int load_dic(Dic *dic);

// fonctions relatives à Instruction nécessitant Dic (donc déclarée après dic)

Instruction get_ins32(word in, Dic *dic);
Instruction get_ins16(word in, Dic *dic);

int comp_instr(); // TODO


char* int_to_bin(unsigned int n, char taille);

char* bin_x_bin(char bin_1[], char bin_2[], int taille); // TODO

#endif
