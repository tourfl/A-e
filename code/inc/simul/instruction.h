#ifndef  INSTRUCTION_H
#define INSTRUCTION_H

#include "types.h" // Plgtab






/*
 * Explications & exemples : cf lib/instructions.md
 *
 */

typedef struct i
{
	// Général:
	char* commande;
	char* encoding;
	char* name_in;
	char* name_out;
	unsigned int mask;
	unsigned int opcode;
 

	// Paramètres :
	Plgtab *reg; // cf types.h
	Plgtab *imm;
	Plgtab *ext;
	
	// Pointeur de fonction :

	void (*run)(void);


} Instruction;

















Instruction* init_ins();
void del_ins(Instruction *ins);
void insclone(Instruction *dest, Instruction *src);

int load_ins(Instruction *ins, char *chaine);



Instruction* init_instab(int sz);


void disp_ins(Instruction ins);

#endif
