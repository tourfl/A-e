#include "inter/command.h" // inclut mémoire notamment
#include <string.h>
#include "elf/elf.h" // Pour assert_elf_file entre autres, inclut scntab, stdio, etc.
#include "inter/notify.h"


int loadcmd(Emulator *emul) {

	int v = 0;
	char * name=NULL;
	vaddr32 va = START_MEM;
    FILE *fo = NULL;
    char usage[] = "Usage : load <nom_du_fichier> {<adresse>}\n";

	name = get_next_token(emul->inter);
	get_last_if_hexa(emul->inter, &va); // rien ne se passe si ce n'est pas un hexa

	if(name == NULL || v == 12)
	{
		printf("%s", usage);
		return 12;
	}
	else if (v == 11)
	{
		return 11;
	}

    fo = fopen(name, "r" );

    if ( fo == NULL ) 
    {
        perror( "fopen" );
        WARNING_MSG("file doesn't exist");
        return 2;
    }

    if(va % 4096 != 0) { // on arrondit au ko supérieur
        va = (va/4096 + 1) * 4096;
    }

    // On récupère le contenu du fichier ELF puis on le charge en mémoire
    if(load_elf_in_mem(fo, emul->map, va) == 0)
        INFO_MSG("%s loaded", name);


    // l'adresse du PC est mise à l'adresse du segment text

    emul->reg[15] = emul->map[0].va;

    fclose(fo);

    return 0;
}









int load_elf_in_mem(FILE *fo, Segment map[], unsigned int va)
{
	uint i, j;
    vaddr32 next_segment_start = va; // compteur pour designer le début de la prochaine section

    /*
    * On ne récupère pas les symboles pour le moment puisque l'on ne s'occupe pas de la relocation
    */

    if (!assert_elf_file(fo))
    	ERROR_MSG("this is not an ELF file");

    byte *ehdr    = __elf_get_ehdr(fo );  

    for (i=0; i<NB_SEC; i++) {
    	switch (i)
    	{
    		case 0:
    		{
    			strcpy(map[i].name, TEXT_SECTION_STR);
    			break;
    		}
    		case 1:
    		{
    			strcpy(map[i].name, RODATA_SECTION_STR);
    			break;
    		}
    		case 2:
    		{
    			strcpy(map[i].name, DATA_SECTION_STR);
    			break;
    		}
    		case 3:
    		{
    			strcpy(map[i].name, BSS_SECTION_STR);
    			break;
    		}
    	}

    	// printf("%s\n", map[i].name);

    	map[i].size = 0;
    	byte *content = elf_extract_scn_by_name(ehdr, fo, map[i].name, &(map[i].size), NULL);

    	if(content != NULL && map[i].size != 0)
    	{
    		map[i].content = malloc(map[i].size * sizeof(byte));

    		if(map[i].content == NULL)
    		{
    			return 1;
    		}

    		for (j = 0; j < map[i].size; ++j)
    		{
    			*(map[i].content + j) = *(content + j);
    		}


            /*
            * ============ ENDIANNESS ============
            * Quelque soit l'endianness réelle du contenu des sections,
            * On fixe ici que l'on est face à du BIG ENDIAN !
            * Les masks sont en LITTLE ENDIAN ALIGNE
            * Le parsing des paramètres se fait également en LITTLE ENDIAN ALIGNE
            */

            // On convertit le contenu en LITTLE ENDIAN ALIGNE
            if (change_endianness(map[i].content, map[i].size, BIG_E, LITTLE_E_ALIGNED) != 0)
                return 31;

    		map[i].va = next_segment_start;

    		DEBUG_MSG("%s at 0x%08x", map[i].name, map[i].va);
    		next_segment_start += (map[i].size/4096 + 1) * 4096;
    	}
    }
    free(ehdr);

    return 0;
}
