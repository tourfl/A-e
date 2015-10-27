#include "mem/memory_v2.h" // inclut la mémoire
#include "com/notify.h" // messages de contrôle, inclut stdio, stdlib
#include <string.h> // pour strcpy notamment
#include "elf/elf.h" // Pour assert_elf_file entre autres, inclut scntab, stdio, etc.
#include <math.h> // pow
#include "com/command.h" // pour print_section_raw_content


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



byte get_byte(vaddr32 va, Memory *mem)
{
    vaddr32 va_start;
    vaddr32 va_end;
    vaddr32 size;
    int i;

    for (i = 0; i < NB_SEC; i++)
    {
        va_start = mem->map[i].va;
        size = mem->map[i].size;
        va_end = va_start + size;

        if (va < va_start)
            return 0;

        else if (va < va_end)
            return *(mem->map[i].content + va - va_start);
    }

    return 0;
}


int set_byte(vaddr32 va, byte value, Memory *mem)
{
    vaddr32 va_start;
    vaddr32 va_end;
    vaddr32 size;
    int i;

    here_n(1);

    for (i = 0; i < NB_SEC; i++)
    {
        va_start = mem->map[i].va;
        size = mem->map[i].size;
        va_end = va_start + size;

        if (va < va_start)
            return 1; // Que faire si l'octet n'appartient pas au map ?

        else if (va < va_end)
        {
            *(mem->map[i].content + va - va_start) = value;
            return 0;
        }
    }

    return 1;
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
        va_end = va_start + size;

        while (va < va_start)
        {
            if(va == va_1 + 3) // $(0x1)*16^3 + $(0x2)*16^2 + $(0x3)*16 + 0x4
                return w;

            va++;
        }

        while (va <= va_end)
        {

            w += *(mem->map[i].content + va - va_start) * pow(16, 2*(3-va+va_1));

            if (va == va_1 + 3)
                return w;

            va++;
        }
    }

    return w;
}

int set_word(vaddr32 va, byte value, Memory *mem)
{

}
