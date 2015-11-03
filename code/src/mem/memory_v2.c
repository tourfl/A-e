#include "mem/memory_v2.h" // inclut la mémoire
#include "com/notify.h" // messages de contrôle, inclut stdio, stdlib
#include <string.h> // pour strcpy notamment
#include "elf/elf.h" // Pour assert_elf_file entre autres, inclut scntab, stdio, etc.
#include <math.h> // pow
#include "com/command.h" // pour print_section_raw_content







int realloc_seg(int size, int i, Segment map[NB_SEC])
{
    void *p = realloc(map[i].content, size * sizeof(byte));

	if(p == 0)
        return 1;

    else
    {
        map[i].content = p;
		map[i].size = size;

        if(i < NB_SEC && map[i].va + size >= map[i+1].va) // Si le segment empiète sur le suivant, on décale les adresses
        {

        	int j;

        	for (j = i+1; j < NB_SEC; j++)
        		map[j].va = (map[j-1].size/4096 + 1) * 4096;
        }

        return 0;
    }
}






void del_seg(Segment *seg)
{
      free(seg->content);
}






void init_mem(Memory *mem)
{
	int i;

	for (i = 0; i < NB_SEC; ++i)
    {
		mem->map[i].size = 0;
        mem->map[i].perm = 0; // Aucune permission
        mem->map[i].content = NULL; // Sinon problème lors de la désallocation
    }

	for (i = 0; i < NB_REG; ++i)
		mem->reg[i] = 0;
}






void del_mem(Memory *mem)
{
    if(mem == NULL)
        return;

    int i;

    for (i = 0; i < NB_SEC; ++i)
    {
        del_seg(mem->map + i);
    }
}








int which_reg (char *nom, Registre reg[NB_REG], Registre *r) {
	int i;
    char n[4]; // la taille peut aller jusqu'à 4 : r15'\0'

    for (i=0; i<NB_REG-1; i++){

    	sprintf(n, "r%i", i);

    	if (strcmp(nom, n) == 0 ){
    		*r = reg[i];
    		return 0;
    	}

    }
    if (strcmp(nom, "sp") == 0) {
    	*r = reg[13];
    	return 0;
    }
    else if (strcmp(nom, "lr") == 0){
    	*r = reg[14];
    	return 0;
    }
    else if (strcmp(nom, "pc") == 0) {
    	*r = reg[15];
    	return 0;
    }
    else if (strcmp(nom, "aspr") == 0) {
    	*r = reg[16];
    	return 0;
    }

    return 1; 
    
}






int get_reg(char *name, word *content, Registre reg[NB_REG])
{
	char n[4];
    int k = -1; // indice du reg trouvé

    if(name[0] == 'r')
    {
    	int i;
    	for (i = 0; i < NB_REG-4 && k<0; ++i)
    	{
            sprintf(n, "r%i", i); // On stocke "ri" dans n

            if(strcmp(name, n) == 0)
            	k=i;
        }
    }

    else if(strcmp(name, "sp") == 0)
    	k=13;
    
    else if(strcmp(name, "lr") == 0)
    	k=14;

    else if(strcmp(name, "pc") == 0)
    	k=15;

    else if(strcmp(name, "apsr") == 0)
    	k=16;

    if(k < 0)
    	return 1;

    *content = reg[k];
    return 0;
}








int set_reg(char *name, word value, Registre reg[NB_REG])
{
	char n[4];
    int k = -1; // indice du reg trouvé

    if(name[0] == 'r')
    {
    	int i;
    	for (i = 0; i < NB_REG-4 && k<0; ++i)
    	{
            sprintf(n, "r%i", i); // On stocke "ri" dans n

            if(strcmp(name, n) == 0)
            	k=i;
        }
    }

    else if(strcmp(name, "sp") == 0)
    	k=13;
    
    else if(strcmp(name, "lr") == 0)
    	k=14;

    else if(strcmp(name, "pc") == 0)
    	k=15;

    else if(strcmp(name, "apsr") == 0)
    	k=16;

    if(k < 0) {
    	WARNING_MSG ("No '%s' registre\n", name);
    	return 1;
    }

    reg[k] = value;
    return 0;
}









int load_elf_in_mem(FILE *fo, Segment map[NB_SEC], unsigned int va)
{
	char* section_names[NB_SEC]= {TEXT_SECTION_STR,RODATA_SECTION_STR,DATA_SECTION_STR,BSS_SECTION_STR};
	scntab section_table;
	unsigned int nsegments;
	int i=0;
	unsigned int type_machine;
    unsigned int endianness;   //little ou big endian
    unsigned int bus_width;    // 32 bits ou 64bits
    unsigned int next_segment_start = va; // compteur pour designer le début de la prochaine section

    stab symtab= new_stab(0); // table des symboles

    if (!assert_elf_file(fo))
    	ERROR_MSG("this is not an ELF file");


    // recuperation des info de l'architecture
    elf_get_arch_info(fo, &type_machine, &endianness, &bus_width);
    elf_load_scntab(fo, bus_width, &section_table);
    // et des symboles
    elf_load_symtab(fo, bus_width, endianness, &symtab);

    nsegments=0;
    byte *ehdr    = __elf_get_ehdr(fo );    
    for (i=0; i<NB_SEC; i++) {

    	INFO_MSG("Processing section named %s", section_names[i]); 

    	strcpy(map[i].name, section_names[i]);

    	map[i].size = 0;
    	byte *content = elf_extract_scn_by_name(ehdr, fo, section_names[i], &(map[i].size), NULL );

    	if(content != NULL && map[i].size != 0)
    	{
    		map[i].content = malloc(map[i].size * sizeof(byte));

    		if(map[i].content == NULL)
    		{
    			ERROR_MSG("Unable to allocate memory");
    			return 1;
    		}

    		int j;

    		for(j = 0; j < map[i].size; j++)
    			*(map[i].content+j) = *(content+j);

    		map[i].va = next_segment_start;
    		next_segment_start += (map[i].size/4096 + 1) * 4096;
    	}

    	else {
    		DEBUG_MSG("section %s not present in the elf file", section_names[i]);
    	}
    }
    free(ehdr); 

    // on fait le ménage avant de partir
    del_stab( symtab );
    del_scntab( section_table );
    return 0;
}







byte get_byte(vaddr32 va, Segment map[NB_SEC])
{
	vaddr32 va_start;
	vaddr32 va_end;
	vaddr32 size;
	int i;

	for (i = 0; i < NB_SEC; i++)
	{
		va_start = map[i].va;
		size = map[i].size;

		if (va < va_start)
			return 0;

		if(size > 0)
		{
            va_end = va_start + size - 1;  // avec 00 ff : size = 2, va_end = va_start + size - 1

            if (va <= va_end)
            	return *(map[i].content + va - va_start);
        }
    }

    return 0;
}









int set_byte(vaddr32 va, byte value, Segment map[NB_SEC])
{
	vaddr32 va_start;
	vaddr32 va_end;
	vaddr32 size;
	int i;

	here_n(1);

	for (i = 0; i < NB_SEC; i++)
	{
		va_start = map[i].va;
		size = map[i].size;

		if (va < va_start)
            return 1; // On ne fait rien si l'octet n'appartient pas au map

        if(size > 0)
        {
            va_end = va_start + size - 1;  // avec 00 ff : size = 2, va_end = va_start + size - 1

            if (va <= va_end)
            {
            	*(map[i].content + va - va_start) = value;
            	return 0;
            }
        }
    }

    return 1; // On ne fait rien si l'octet n'appartient pas au map
}

word get_word(vaddr32 va_1, Memory *mem)
{

  /* Les octets sont dans des segments sinon ils sont nuls 
  */

	vaddr32 va = va_1;
	vaddr32 va_start;
	vaddr32 va_end;
	uint32_t size;
	word w = 0;
	int i;

	for (i = 0; i < NB_SEC; i++)
	{
		va_start = mem->map[i].va;
		size = mem->map[i].size;

		while (va < va_start)
		{
            if(va == va_1 + 3) // $(0x1)*16^3 + $(0x2)*16^2 + $(0x3)*16 + 0x4
            	return w;

            va++;
        }

        if(size > 0)
        {
            va_end = va_start + size - 1;  // avec 00 ff : size = 2, va_end = va_start + size - 1

            while (va <= va_end)
            {

            	w += *(mem->map[i].content + va - va_start) * pow(16, 2*(3-va+va_1));

            	if (va == va_1 + 3)
            		return w;

            	va++;
            }
        }
    }

    return w;
}

int set_word(vaddr32 va_1, word value, Segment map[NB_SEC])
{

  /* Si va n'appartient à aucune section, rien n'est inscrit et la fonction renvoie 1
   * 
   * Cas particulier :
   * Le mot écrit est à la fin d'une section, nécessitant l'ajout d'octets à cette section, une nouvelle allocation est requise
   */

   vaddr32 va = va_1;
   vaddr32 va_start;
   vaddr32 va_end;
   uint32_t size;
   int p;
   int i;

   for (i = 0; i < NB_SEC; i++)
   {
		va_start = map[i].va;
		size = map[i].size;

   		if(va < va_start)
   			return 1;

   		if(size > 0)
   		{
        	va_end = va_start + size - 1;  // avec 00 ff : size = 2, va_end = va_start + size - 1

        	if(va <= va_end && va + 3 > va_end) // On réalloue la section
        	{
            	size = size + va + 3 - va_end; // On met à jour la taille
            	va_end = va_start + size;

            	if (realloc_seg(size, i, map) != 0)
            		return 2;
        	}

        	while (va <= va_1 + 3)
        	{
            	// big endian

        		p = pow(16, 2*(3-va+va_1));

        		*(map[i].content + va - va_start) = value / p;

        		value = value % p;

        		if(va == va_1 + 3)
        			return 0;

        		va++;
        	}
    	}
	}

	return 1;
}

byte *get_plage(vaddr32 va_1, vaddr32 va_2, Segment map[NB_SEC]) // on suppose va_2 >= va_1
{
    if(va_1 > va_2)
    {
        WARNING_MSG("va_2 < va_1");
        return NULL;
    }

    byte *plage = malloc((va_2 - va_1 + 1) * sizeof(byte));

    vaddr32 va = va_1;
    vaddr32 va_start;
    vaddr32 va_end;
    vaddr32 size;
    int i;

  /* Les octets appartiennent à des segments sinon ils sont nuls 
  */

    for (i = 0; i < NB_SEC; i++)
    {
        va_start = map[i].va;
        size = map[i].size;


        while (va < va_start)
        {
            plage[va - va_1] = 0;

            if (va == va_2)
                return plage;

            va++;
        }

        if(size > 0)
        {
            va_end = va_start + size - 1;  // avec 00 ff : size = 2, va_end = va_start + size - 1

            while (va <= va_end)
            {
                plage[va - va_1] = *(map[i].content + va - va_start);

                if (va == va_2)
                    return plage;

                va++;
            }
        }
    }

    while (va <= va_2)
    {
        plage[va - va_1] = 0;

        va++;
    }

    return plage;
}

