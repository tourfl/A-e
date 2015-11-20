#ifndef  INSTRUCTION_H
#define INSTRUCTION_H

#include "types.h" // Plgtab






/*
 * Explications & exemples : cf lib/instructions.md
 *
 */

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
	Plgtab *reg; // cf types.h
	Plgtab *imm;
	Plgtab *ext;
	
	// Pointeur de fonction: 		


} Instruction;

















Instruction* init_ins();
void del_ins(Instruction *ins);
int load_ins(Instruction *ins, char *chaine);

Instruction* init_instab(int sz);


void disp_ins(Instruction ins);

#endif
