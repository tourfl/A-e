#ifndef  INSTRUCTION_H
#define INSTRUCTION_H

#include "types.h" // Plgtab
#include "emulator.h"






/*
 * Explications & exemples : cf lib/instructions.md
 *
 */

// pour le problème d'inclusions circulaires
struct emulator;
typedef struct emulator Emulator;



struct ins
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

	void (*run)(Emulator *emul);


};

typedef struct ins Instruction;

















Instruction* init_ins();
void del_ins(Instruction *ins);
void insclone(Instruction *dest, Instruction *src);

int load_ins(Instruction *ins, char *chaine);



Instruction* init_instab(int sz);


void disp_ins(Instruction ins);

#endif
