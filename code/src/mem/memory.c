/*
 * fichier qui gere l'initialisation de la memoire
 */

 #include <stdlib.h>

 #include "mem/memory.h"

 int load_elf(FILE *fo, long va)
 {
 	// On va réutiliser les fonctions fournies
 }

int init_mem_map(char mem_size)
 {

 	/*
 	* On crée mem_size cases pour (.rodata), .text, .data, .bss, [lib], [stack]
 	*/

 	return malloc(mem_size * sizeof(*Mem_seg)); // retourne évidemment NULL si cela n'a pas fonctionné
 }

int add_first_segment(char *name, long va, int size, Mem_map *map)
{
	return(add_segment(name, 0, &va, size, map))
}

int add_segment(char *name, char map_cursor, long *va, int size, Mem_map *map)
{
	Mem_seg *seg = NULL;

	seg = malloc(sizeof(Mem_seg));

	(seg == NULL) ? return 1; // La mémoire n'a pas été réservée

	seg->name = name;
	seg->va = *va;
	//seg->size = size;

	map[map_cursor] = first_seg;

	update_va(va, size);

	return 0;
}


int update_va(long *va, int size)
{
	// TODO
}
