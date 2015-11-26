#include "inter/command.h" // inclut memory et interpreteur
#include "types.h" // byte
#include "inter/notify.h" // messages de contrôle, inclut stdlib (printf)
#include <string.h> // pour strcmp

// fonction affichant les octets d'un segment sur la sortie standard
// parametres :
//   name           : nom de la section dont le contenue est affichée
//   start          : adresse virtuelle d'implantation du contenu de la section dans la memoire 
//   content        : le contenu de la section à afficher
//   taille         : taille en octet de la section à afficher



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
	int i;

	printf("Virtual memory map (%u segments)\n", NB_SEC);

	for(i = 0; i < NB_SEC; i++)
		printf("%s\trwx\tVaddr: 0x%08x\tSize: %u bytes\n", map[i].name, map[i].va, map[i].size);
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
			printf("\n  0x%08x ", va);

		printf("%02x ", plage[va - plg.start]);

		va ++;
		k ++;
	}

	free(plage);

	printf("\n");
}

void disp_oct(unsigned int va, Segment map[NB_SEC]) // V2
{
	printf(" 0x%08x %02x\n", va, get_byte(va, map));
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
	for (i = 0; i < NB_REG-4; ++i)
	{
		printf("  r%i : %i \n", i, reg[i]);
	}

	printf("  sp : %i \n", reg[13]);
	printf("  lr : %i \n", reg[14]);
	printf("  pc : %i \n", reg[15]);
	printf("  apsr : %i \n", reg[16]);
}