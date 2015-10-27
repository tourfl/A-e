#ifndef H_MEMORY
#define H_MEMORY

#include <stdio.h> // pour fo
#include "com/types.h" // pour uint32_t

#define NAME_SIZE_MAX 100
#define CONTENT_SIZE_MAX 1000

#define NB_REG 17
/*
 * de r0 à r15 + apsr (reg[16])
 * r13 : sp 	r14 : lr 	r15 : pc
 */

#define REG_SIZE_MAX 1000
	
 // On fixe ici une adresse basse dans la mémoire virtuelle. Le premier segment
// ira se loger à cette adresse.
#define START_MEM 0x1000

// nombre max de sections que l'on extraira du fichier ELF
#define NB_SEC 4

// nom de chaque section
#define TEXT_SECTION_STR ".text"
#define RODATA_SECTION_STR ".rodata"
#define DATA_SECTION_STR ".data"
#define BSS_SECTION_STR ".bss"

typedef struct segment
{
	vaddr32 va;
	char name[NAME_SIZE_MAX];
	byte content[CONTENT_SIZE_MAX];
	uint32_t size;

} Segment;

// un registre est un unsigned int

typedef word Registre;

int get_reg(char *name, word *content, Registre reg[NB_REG]); // Il faut une valeur de retour pour savoir si name appartient bien aux registres
int set_reg(char *name, word value, Registre reg[NB_REG]);

int which_reg (char *nom, Registre reg[NB_REG], Registre *r); // obsolète, inutilisée

typedef struct
{
	Segment map[NB_SEC];
	Registre reg[NB_REG];

} Memory;

int load_elf_in_mem(FILE *fo, Memory *mem, unsigned int va);

// Les fonctions suivantes risquent d'être assez compliquées

byte get_byte(vaddr32 va, Memory *mem);
int set_byte(vaddr32 va, byte value, Memory *mem);

word get_word(vaddr32 va, Memory *mem);
int set_word(vaddr32 va, byte value, Memory *mem);

#endif