#include "mem/memory_v2.h"

int init_seg(unsigned long va, char name[], byte content[], Segment *seg)
{	
	if (seg == NULL)
	{
		ERROR_MSG("seg is not allocated");
		return 2;
	}

	if (sizeof(content) > CONTENT_SIZE_MAX)
	{
		WARNING_MSG("content is too big");
		return 1;
	}

	strcpy(seg->va, va);
	strcpy(seg->name, name);
	strcpy(seg->content, content);

	return 0;
}

 int load_elf_in_mem(FILE *fo, Memory *mem, unsigned long va)
 {
    char* section_names[NB_SECTIONS]= {TEXT_SECTION_STR,RODATA_SECTION_STR,DATA_SECTION_STR,BSS_SECTION_STR};
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
    for (i=0; i<NB_SECTIONS; i++) {

        printf("Processing section named %s\n", section_names[i]); 

        strcpy(mem->map[i].name, section_names[i]);

        uint32_t taille = 0;
        byte* content = elf_extract_scn_by_name(ehdr, fo, section_names[i], &taille, NULL );
        
        if (content!=NULL){
        	for(int j = 0; j < taille && j < CONTENT_SIZE_MAX; j++)
        		 mem->map[i].content[j] = *(content + j); // On copie proprement le contenu

			mem->map[i].va = next_segment_start;
			next_segment_start += (taille/4096 + 1) * 4096;
        }
        else{

            DEBUG_MSG("section %s not present in the elf file",section_names[i]);
        }

        mem->map[i].size = taille;
    }
		free(ehdr); 

    // on fait le ménage avant de partir
    del_stab( symtab );
    del_scntab( section_table );
 	return 0;
 }
