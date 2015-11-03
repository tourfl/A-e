#ifndef H_MEMORY
#define H_MEMORY

#include <stdio.h> // pour fo
#include "com/types.h" // pour uint32_t

#define NAME_SIZE_MAX 100

#define NB_REG 17
/*
 * de r0 à r15 + apsr (reg[16])
 * r13 : sp 	r14 : lr 	r15 : pc
 */
	
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

typedef struct
{
	vaddr32 va;
	char name[NAME_SIZE_MAX];
	byte *content; // On utilise l'allocation dynamique
	uint32_t size;

} Segment;

/* realloc_seg :
 *
 *	on realloue map[i].content avec la taille size
 * 	le map est nécessaire pour décaler les autres segments, si besoin
 *	on suppose size > 0
 */

int realloc_seg(int size, int i, Segment map[NB_SEC]);

void del_seg(Segment *seg);

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

/*
 * Après réflexion, il eut été plus pratique que la mémoire soit une variable globale
 * static Memory _mem_;
 * ... Mais ce sera trop long à changer
 */


void init_mem(Memory *mem);
void del_mem(Memory *mem);

int load_elf_in_mem(FILE *fo, Segment map[NB_SEC], unsigned int va);

// Les fonctions suivantes risquent d'être assez compliquées

byte get_byte(vaddr32 va, Segment map[NB_SEC]);
int set_byte(vaddr32 va, byte value, Segment map[NB_SEC]);

word get_word(vaddr32 va_1, Memory *mem);
int set_word(vaddr32 va_1, word value, Segment map[NB_SEC]);

byte *get_plage(vaddr32 va_1, vaddr32 va_2, Segment map[NB_SEC]);

#endif