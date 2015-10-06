/*
 * fichier qui gere l'initialisation de la memoire
 */

 #include <stdlib.h>

 #include "mem/memory.h"
 #include "elf/elf.h"

 int load_elf(FILE *fo, long va, Memory *mem)
 {
 	/*
 	 * Cela va sans doute être le zbeul avec les **
 	 */


 	if (!assert_elf_file(fo))
 		return 1;

 	byte *ehdr = NULL;

 	uint *mem_size = NULL;
 	byte *section_names = NULL;

 	uint **secsz = NULL;
 	byte **scn = NULL;

 	ehdr = __elf_get_ehdr(fo);

 	[ehdr == NULL] ? return 2;

 	section_names = elf_extract_section_names(fo, mem_size);

 	[section_names == NULL] ? return 3;

 	// On alloue le nombre de segments mem_size

 	mem->map = malloc(mem_size * sizeof(*Segment));

 	for(i = 0; i < *mem_size; i++)
 	{
 		mem->map[i]->name = section_names[i]; // On donne son nom au segment

 		mem->map[i]->content = elf_extract_scn_by_name(ehdr, fo, section_names[i], secsz[i], NULL); // On récupère le contenu du segment

 		mem->map[i]->size = secsz[i]; // on récupère la taille du segment

 	}

 	return 0;
 }

int init_Map(char mem_size)
 {

 	/*
 	* On crée mem_size cases pour (.rodata), .text, .data, .bss, [lib], [stack]
 	*/

 	return malloc(mem_size * sizeof(*Segment)); // retourne évidemment NULL si cela n'a pas fonctionné
 }

int add_first_segment(char *name, long va, int size, Map *map, char *content)
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
	strcpy(r[13].name, sp);
	malloc (word * sizeof(*r[13].valeur));
	strcpy(r[14].name, lr);
	malloc (word * sizeof(*r[14].valeur));
	strcpy(r[15].name, pc);
	malloc (word * sizeof(*r[15].valeur));
	strcpy(r[16].name, aspr);
	malloc (word * sizeof(*r[16].valeur));
}


int add_segment(char *name, char map_cursor, long *va, int size, Map *map, char *content)
{
	Segment *seg = NULL;

	seg = malloc(sizeof(Segment));

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
