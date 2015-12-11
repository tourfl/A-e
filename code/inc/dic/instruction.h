#ifndef  INSTRUCTION_H
#define INSTRUCTION_H

#include "types.h" // Plgtab
#include "emulator.h"
#include "elf/bits.h"






/*
 * Explications & exemples : cf lib/instructions.md
 *
 */

 #define MAX_CHAR 10

typedef enum decodage {
	FOUND_16=2,
	FOUND_32=4,
	UNFOUND=1
} Decodage;



// pour le problème d'inclusions circulaires
struct emulator;
typedef struct emulator Emulator;

// pour pouvoir utliser Instruction dans le pointeur de fonction
struct ins;
typedef struct ins Instruction;


// création d'alias du type pointeur de fonction
typedef int (*Fill_pft)(word, Instruction*);
typedef int (*Run_pft)(Instruction, Emulator*); 
typedef void (*Disp_pft)(Instruction);


struct ins
{
	// Général:
	char commande[MAX_CHAR];
	int encoding;
	char name_in[MAX_CHAR];
	char name_out[MAX_CHAR];
	unsigned int mask;
	unsigned int opcode;
 

	// Paramètres :
	Plgtab *reg; // cf types.h
	Plgtab *imm;
	Plgtab *ext;
	
	// Pointeurs de fonction :

	Fill_pft fill_params;
	Disp_pft display_decoded;
	Run_pft run;


};













Instruction* init_ins();
void del_ins(Instruction *ins);
void insclone(Instruction *dest, Instruction *src);

int load_ins(Instruction *ins, char *chaine);
int load_from_string(Instruction *ins, char *chaine);

void init_pft(Instruction *ins);

Instruction* init_instab(int sz);


int get_ins(word in, Instruction *out, Instruction dic[], int sz_dic);

int cmp_ins(const void *ins1, const void *ins2);

#endif
