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
	get_last_if_addr(emul->inter, &va); // rien ne se passe si ce n'est pas un hexa

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

    if (va % 0x1000 != 0)
        va = ( (va >> 12) << 12 ) + 0x1000; // on arrondit au ko supérieur

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
    int perm=0;
	uint i, j = 0, size=0;
    char section_name[NAME_SIZE_MAX] = {0};

    /*
    * On ne récupère pas les symboles pour le moment puisque l'on ne s'occupe pas de la relocation
    */

    if (!assert_elf_file(fo))
    	ERROR_MSG("this is not an ELF file");

    byte *ehdr    = __elf_get_ehdr(fo );  

    for (i=0; i<NB_SEC; i++) {

        map[i].size = 0; // initialisation de toutes les sections





    	switch (i)
    	{
    		case 0:
    		{
                perm = 5;
    			strcpy(section_name, TEXT_SECTION_STR);
    			break;
    		}
    		case 1:
    		{
                perm = 4;
    			strcpy(section_name, RODATA_SECTION_STR);
    			break;
    		}
    		case 2:
    		{
                perm = 6;
    			strcpy(section_name, DATA_SECTION_STR);
    			break;
    		}
    		case 3:
    		{
                perm = 6;
    			strcpy(section_name, BSS_SECTION_STR);
    			break;
    		}
    	}

    	// printf("%s\n", map[i].name);


    	byte *content = elf_extract_scn_by_name(ehdr, fo, section_name, &size, NULL);

        if(load_section(content, section_name, &va, size, perm, &map[j]) == 0)
        {
            DEBUG_MSG("%s at 0x%08x", map[j].name, map[j].va);
            j++;
        }
        else if(strcmp(section_name, BSS_SECTION_STR) == 0)
        {
            // on charge une section BSS même s'il n'y en a pas dans le fichier objet

            strcpy(map[j].name, section_name);
            map[j].va = va;
            va += 0x1000;
            map[j].perm = perm;
            j++;
        }
    }
    free(ehdr);

    return 0;
}





int load_section(byte *in, char name[NAME_SIZE_MAX], vaddr32 *start, uint size, int perm, Segment *out)
{
    int i;



    if(size == 0 || in == NULL)
        return 1;

    /*
    * ============ ENDIANNESS ============
    * Quelque soit l'endianness réelle du contenu des sections,
    * On fixe ici que l'on est face à du BIG ENDIAN !
    * Les masks sont en LITTLE ENDIAN ALIGNE
    * Le parsing des paramètres se fait également en LITTLE ENDIAN ALIGNE
    */

    // On convertit le contenu en LITTLE ENDIAN ALIGNE

    else if (change_endianness(in, size, BIG_E, LITTLE_E_ALIGNED) != 0)
        return 31;

    else if( ( out->content = calloc( size, sizeof(byte) ) ) == NULL)
        return 2;


    for (i = 0; i < size; ++i)
    {
        *(out->content + i) = *(in + i);
    }

    out->va = *start;

    *start = ( ( (*start + size) >> 12 ) << 12 ) + 0x1000;

    out->size = size;
    strcpy(out->name, name);
    out->perm = perm;

    return 0;
}



