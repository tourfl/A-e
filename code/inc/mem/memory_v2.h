#ifndef H_MEMORY
#define H_MEMORY

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "common/types.h"
#include "common/notify.h"
#include "elf/elf.h"

#define NAME_SIZE_MAX 100
#define CONTENT_SIZE_MAX 1000

#define REG_NB 16 // de 0 à 15
#define REG_SIZE_MAX 1000
	
 // On fixe ici une adresse basse dans la mémoire virtuelle. Le premier segment
// ira se loger à cette adresse.
#define START_MEM 0x1000

// nombre max de sections que l'on extraira du fichier ELF
#define NB_SECTIONS 4

// nom de chaque section
#define TEXT_SECTION_STR ".text"
#define RODATA_SECTION_STR ".rodata"
#define DATA_SECTION_STR ".data"
#define BSS_SECTION_STR ".bss"

typedef struct
{
	unsigned long va;
	char name[NAME_SIZE_MAX];
	byte content[CONTENT_SIZE_MAX];
	uint32_t size;

} Segment;

int init_seg(unsigned long va, char name[], byte content[], Segment *seg); // OK

// un registre est un banal tableau de caractères

typedef char Registre[REG_SIZE_MAX];

typedef struct {
	Registre r[REG_NB];
	Registre sp;
	Registre lr;
	Registre pc;
	Registre apsr;
} Registres;

typedef struct
{
	Segment map[NB_SECTIONS];
	Registres *reg;
} Memory;

int load_elf_in_mem(FILE *fo, Memory *mem);

#endif