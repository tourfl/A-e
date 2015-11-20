#ifndef SEGMENT_H
#define SEGMENT_H

#include "types.h" // pour uint32_t

#define NAME_SIZE_MAX 100
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
	int perm; // rwx = 0b111 = 7, rw- = 0b110 = 6, r-- = 0b100 = 4, etc.

} Segment;







void del_map(Segment map[NB_SEC]);

int realloc_seg(int size, int i, Segment map[NB_SEC]);
void del_seg(Segment *seg);

byte get_byte(vaddr32 va, Segment map[NB_SEC]);
int set_byte(vaddr32 va, byte value, Segment map[NB_SEC]);

word get_word(vaddr32 va_1, Segment map[NB_SEC]);
int set_word(vaddr32 va_1, word value, Segment map[NB_SEC]);

byte *get_plage(Plage p, Segment map[NB_SEC]);

#endif