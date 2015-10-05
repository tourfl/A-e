/*
 * fichier qui gere l'initialisation de la memoire
 */

 #include <stdlib.h>

 #include "mem/memory.h"
 #include "elf/elf.h"

 int load_elf(FILE *fo, long va)
 {
 	if (!assert_elf_file(fo))
 		return 1;

 	byte *ehdr = NULL;

 	uint *mem_size = NULL;
 	byte *section_names = NULL;

 	uint **secsz = NULL;
 	byte **scn = NULL;

 	ehdr = __elf_get_ehdr(fo);

 	section_names = elf_extract_section_names(fo, mem_size);

 	for(i = 0; i < *mem_size; i++)
 	{
 		scn[i] = elf_extract_scn_by_name(ehdr, fo, section_names[i], secsz[i], NULL);
 		printf("%s \n", section_names[i]);
 		print_string_table(scn[i], secsz[i]);
 	}

 }

int init_mem_map(char mem_size)
 {

 	/*
 	* On crée mem_size cases pour (.rodata), .text, .data, .bss, [lib], [stack]
 	*/

 	return malloc(mem_size * sizeof(*Mem_seg)); // retourne évidemment NULL si cela n'a pas fonctionné
 }

int add_first_segment(char *name, long va, int size, Mem_map *map, char *content)
{
	return(add_segment(name, 0, &va, size, map))
}

//je crois que je fais de la merde;
void allocation_reg(){
	registre* r;
	int i;
	for (i=0; i<=12; i++){
		strcpy(r[i].name, r[i]);
		malloc (word * sizeof(*r[i].valeur));
	}
	
}


int add_segment(char *name, char map_cursor, long *va, int size, Mem_map *map, char *content)
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
