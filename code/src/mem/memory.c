/*
 * fichier qui gere l'initialisation de la memoire
 */

 #include "mem/memory.h"

 Memory *init_mem()
 {
 	DEBUG_MSG("init_mem entered");

 	Memory *memory = NULL;
 	Map *memory_map = NULL;
 	Registres *registres = NULL;

 	registres = malloc(sizeof(Registres));

 	if(registres == NULL) return NULL;

 	memory_map = malloc(sizeof(Map));

 	if(memory_map == NULL) return NULL;

 	memory = malloc(sizeof(Memory));

 	if (memory == NULL) return NULL;

 	memory->map = memory_map;
 	memory->reg = registres;

 	return memory;
 }

 Segment * init_seg()
 {
 	Segment *segment = NULL;
	char va[11]; // Une adresse 32 bits en hexa fait 10 caractères + \0

	segment = malloc(sizeof(Segment));
	if (segment == NULL) return NULL;

	segment->va = va;

	return segment;
 }

 Segment * init_seg_with(char *name, char *content)
 {
 }

 int load_elf_in_mem(FILE *fo, char *va, Map *map)
 {
 	/*
 	 * Cela va sans doute être le zbeul avec les **
 	 */

 	 DEBUG_MSG("load_elf_in_mem fonction entered");


 	if (!assert_elf_file(fo))
 		return 1;

 	byte *ehdr = NULL;

 	uint *mem_size = NULL;
 	byte *section_names = NULL;

 	uint *secsz = NULL;

 	ehdr = malloc(sizeof(byte));
 	if (ehdr == NULL) return 4;

 	mem_size = malloc(sizeof(uint));
 	if (mem_size == NULL) return 6;

 	secsz = malloc(sizeof(uint *));
 	if (secsz == NULL) return 5;

 	ehdr = __elf_get_ehdr(fo);
 	if (ehdr == NULL) return 2;

 	section_names = elf_extract_section_names(fo, mem_size);

 	if (section_names == NULL) return 3;

 	// On alloue le nombre de segments mem_size

 	map = malloc(*mem_size * sizeof(Segment *));

 	DEBUG_MSG("%s", section_names);

 	for(int i = 0; i < *mem_size; i++)
 	{
 		Segment *seg = NULL;

 		// Pour que cela soit plus lisible, on crée des copies des variables

 		byte *section_name = &section_name[i];
 		uint *size = &secsz[i];
 		byte *scn = NULL;

 		seg = init_seg();
 		if (seg == NULL) return 8;

 		map[i] = seg;

 		map[i]->content = scn;

 		map[i]->name = section_name; // On donne son nom au segment

 		map[i]->content = elf_extract_scn_by_name(ehdr, fo, section_name, secsz[i], NULL); // On récupère le contenu du segment

 		map[i]->size = size; // on récupère la taille du segment

 		DEBUG_MSG("How many : %i", i);
 	}

 	DEBUG_MSG("here");

 	return 0;
 }

int add_first_segment(char *name, char *va, int size, Map *map, char *content)
{
	return(add_segment(name, 0, va, size, map, content));
}

//je crois que je fais de la merde;
void allocation_reg(){
	Registre* r;
	int i;
	for (i=0; i < 15; i++){
		//strcpy(r[i].name, r[i]);
		//malloc (word * sizeof(*r[i].valeur));
	}
	/*
	strcpy(r[13].name, sp);
	malloc (word * sizeof(*r[13].valeur));
	strcpy(r[14].name, lr);
	malloc (word * sizeof(*r[14].valeur));
	strcpy(r[15].name, pc);
	malloc (word * sizeof(*r[15].valeur));
	strcpy(r[16].name, aspr);
	malloc (word * sizeof(*r[16].valeur));
	*/
}


int add_segment(char *name, char map_cursor, char *va, int size, Map *map, char *content)
{
	Segment *seg = NULL;

	seg = malloc(sizeof(Segment));

	if(seg == NULL) return 1; // La mémoire n'a pas été réservée

	seg->name = name;
	seg->va = *va;
	//seg->size = size;

	map[map_cursor] = seg;

	update_va(va, size);

	return 0;
}


int update_va(char **va, int size)
{
	// TODO
}
