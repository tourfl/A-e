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
	UNFOUND=1,
	PARSE_ERROR=15,
	PREPROCESS_ERROR=16
} Decodage;





typedef enum it_flag {
	IN, OUT
} It_flag;



// pour le problème d'inclusions circulaires
struct emulator;

// pour pouvoir utliser Instruction dans le pointeur de fonction
struct ins;


// création d'alias du type pointeur de fonction
typedef int (*Prep_pft)(struct ins*);
typedef int (*Run_pft)(struct ins, struct emulator*);
typedef void (*Disp_pft)(struct ins, struct emulator*);


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
	Plgtab reg; // cf types.h
	Plgtab imm;
	Plgtab ext;

	It_flag it_flag;
	
	// Pointeurs de fonction :

	Prep_pft preprocess;
	Disp_pft display_decoded;
	Run_pft run;
};

typedef struct ins Instruction;













Instruction* init_ins();
void del_ins(Instruction *ins);
void insclone(Instruction *dest, Instruction *src);

int load_ins(Instruction *ins, char *chaine);
int load_from_string(Instruction *ins, char *chaine);

void init_pft(Instruction *ins);

Instruction* init_instab(int sz);

void del_instab(Instruction *tab, int size);


int get_ins(word in, Instruction *out, Instruction dic[], int sz_dic);

int cmp_ins(const void *ins1, const void *ins2);

#endif
