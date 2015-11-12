
#ifndef  DIC_H
#define DIC_H

#include <stdio.h>
#include "com/types.h" // word et Strlist

#define TAILLE_MAX 1000

/*
 * Explications & exemples : cf lib/instructions.md
 *
 */

 int to_strtab(char *chaine, char **tab);
 int to_wrdtab(char *chaine, word tab[]);

 int to_strlist(char *chaine, Strlist *liste); // fonction ultra sécurisée !

int to_plgtab(char *chaine, Plgtab *plgt);

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
	Plgtab reg; // cf types.h
	Plgtab imm;
	Plgtab ext;
	
	// Pointeur de fonction: 		


} Instruction, Ins_disasmd;

void init_ins(Instruction *ins);
void del_ins(Instruction *ins);

int load_ins(Instruction *ins, char *chaine);

void disp_ins(Instruction ins);
void disp_insd(Instruction ins);

int get_ins(word in, Instruction *out, Instruction dic[], int sz_dic);

void cpy_ins(Ins_disasmd *dest, Instruction *src);

int parse_params(word mot, Plgtab *tab);
int parse_param(word mot, Plage *p);

 typedef struct 
 {
 	Instruction* ins32;
 	Instruction* ins16;
 	int sz32;
 	int sz16;

 } Dic;



void init_dic(Dic *dic);
void del_dic(Dic *dic);

int disp_dic(Dic *dic);

int load_dic(Dic *dic);
int load_ins_tab_from_file(Instruction **dic, int *dic_sz, FILE *fd);

// fonctions relatives à Instruction nécessitant Dic (donc déclarée après dic)

int get_ins32(word in, Instruction *out, Dic *dic);
int get_ins16(word in, Instruction *out, Dic *dic);

int comp_instr(); // TODO


char* int_to_bin(unsigned int n, char taille); // peut servir pour faire des tests

#endif
