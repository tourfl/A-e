#include "inter/command.h" // inclut memory et interpreteur
#include "types.h" // byte
#include "inter/notify.h" // messages de contrôle, inclut stdlib (printf)
#include <string.h> // pour strcmp





int dispcmd (Emulator *emul) {

	// V2 (3 niveaux de conditions max)

	interpreteur inter = emul->inter;
	int r = 0;
	char* what = get_next_token (inter);
	char *where = get_next_token(inter);
	char usage[] = "Usage : disp mem <plage>+\n\tdisp mem map\n\tdisp reg <registre>+";
	Plage plg;




	if(what != NULL && strcmp(what, "mem") == 0)
	{
		if(where != NULL && strcmp(where, "map") == 0)
		{
			if(get_next_token(inter) != NULL)
			{
				printf("%s\n", usage);
				return 11;
			}

			disp_map(emul->map);
			return 0;
		}

		else
		{
			reset_pos(inter, inter->pos);

			r = parse_plage(inter, &plg);

			while (r == 0)
			{
				printf("start: 0x%08x;\tend: 0x%08x\n", plg.start, plg.end);

				disp_plage(plg, emul->map);

				r = parse_plage(inter, &plg);
			}

			if(r == 11) // équivalent à un else pour le while
			{
				printf("%s\n", usage);
				return 11;
			}
			else
			{
				printf("\n");
				return 0;
			}
		}
	}

	else if(what != NULL && strcmp(what, "reg") == 0)
	{
		if(where != NULL && strcmp(where, "all") == 0)
		{
			if(get_next_token(inter) == NULL)
			{
				disp_all_regs(emul->reg);
				return 0;
			}

			// Si le token suivant n'est pas nul, on sort sans qu'il ne se soit rien passé
		}

		else if (where != NULL)
			return disp_some_reg(where, emul);			
	}

	printf("%s", usage);
	return 11;
}







int disp_some_reg(char *token, Emulator *emul)
{
	while (token != NULL) 
	{
		if(disp_reg(token, emul->reg) != 0)
			return 1;

		token = get_next_token(emul->inter);
	}

	return 0;
}


void print_section_bytes(int start, byte *content)
{
	unsigned char octet =0;
	if (content!=NULL) {
		octet = *((unsigned char *) (content+start));
		printf("%02x ", octet);
	}
}

void print_section_raw_content(char* name, unsigned int start, byte* content, unsigned int taille) {
	uint k;
	unsigned char octet =0;
	printf("\n section %s loaded at %x :\n",name,start); 
	if (content!=NULL && taille>0) {
		for(k=0; k<taille; k+=1) {
           // on affiche le contenu de la section qui devrait se retrouver 
           // en "memoire virtuelle" à l'adresse virtuelle start+k 
           // (*(content + k) dans la mémoire physique)
			if(k%16==0) printf("\n  0x%08x ", start + k);
			octet = *((unsigned char *) (content+k));
			printf("%02x ", octet);
		}
	}
	printf("\n");
}

void disp_map(Segment map[NB_SEC])
{
	int i, n = number_of_loaded_section(map);
	char str_perm[4] = "---";




	if(n <= 1) // rien ou juste .bss
	{
		WARNING_MSG("Not any section loaded");
		return;
	}

	printf("Virtual memory map (%u segments)\n", n);

	for(i = 0; i < n; i++)
	{
		perm_to_str(map[i].perm, str_perm);

		printf("%s\t", map[i].name);
		printf("%s\t", str_perm);
		printf("Vaddr: 0x%08x\tSize: %u bytes\n", map[i].va, map[i].size);
	}
}

void disp_plage (Plage plg, Segment map[NB_SEC]) // on suppose va_1 > va_2
{
  /* Les octets sont dans des segments sinon ils sont nuls 
  */

	vaddr32 va = plg.start;
	byte *plage = NULL;
	int k = 0;

	plage = get_plage(plg, map);

	while(va <= plg.end)
	{
		if (k % 16 == 0)
			printf("\n0x%08x\t", va);

		printf("%02x ", plage[va - plg.start]);

		va ++;
		k ++;
	}

	free(plage);
}

void disp_oct(unsigned int va, Segment map[NB_SEC]) // V2
{
	printf("\n0x%08x\t%02x", va, get_byte(va, map));
}

int disp_reg(char *name, Registre reg[NB_REG])
{
	word r;

	if (get_reg(name, &r, reg) == 0)
	{
		printf(" %s : %i \n",name, r);
		return 0;
	}

	WARNING_MSG ("No '%s' registre\n", name);
	return 1;
}

void disp_all_regs(Registre reg[NB_REG])
{
	int i;

	printf("\n");

	for (i = 0; i < NB_REG-4; ++i)
	{
		if(i < 10)
			printf(" ");
			
		printf(" r%i :  %08x", i, reg[i]);

		if(i%3 == 2)
			printf("\n");
		else
			printf("\t");	
	}

	printf("  sp :  %08x\t", reg[13]);
	printf("  lr :  %08x\n", reg[14]);
	printf("  pc :  %08x\t", reg[15]);
	printf("apsr :  %08x\n", reg[16]);
}
