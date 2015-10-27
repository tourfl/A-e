#include "com/command.h" // inclut memory et interpreteur
#include "com/types.h" // byte
#include "com/notify.h" // messages de contrôle, inclut stdlib (printf)
#include <string.h> // pour strcmp

// fonction affichant les octets d'un segment sur la sortie standard
// parametres :
//   name           : nom de la section dont le contenue est affichée
//   start          : adresse virtuelle d'implantation du contenu de la section dans la memoire 
//   content        : le contenu de la section à afficher
//   taille         : taille en octet de la section à afficher



int dispcmd (interpreteur inter, Memory *mem) {

	// V2 (3 niveaux de conditions max)

	char* what = NULL;
	char *where = NULL;
	char usage[] = "Usage : disp mem <plage>+\n\tdisp mem map\n\tdisp reg <registre>+\n";

	what = get_next_token (inter);
	where = get_next_token(inter);

	if(strcmp(what, "mem") == 0)
	{
		if(where != NULL && strcmp(where, "map") == 0 && get_next_token(inter) == NULL)
		{
			disp_map(mem);
			return 0;
		}

		else if(is_hexa(where) == 0) // Comprend where != NULL
		{
			return disp_some_mem(where, inter, mem);
		}
	}

	else if(strcmp(what, "reg") == 0)
	{
		 if(where != NULL && strcmp(where, "all") == 0)
		{
			if(get_next_token(inter) == NULL)
			{
				here;
				disp_all_regs(mem->reg);
				return 0;
			}

			// Si le token suivant n'est pas nul, on sort sans qu'il ne se soit rien passé
		}

		else if (where != NULL)
			return disp_some_reg(where, inter, mem->reg);			
	}

	WARNING_MSG("Problem with tokens");
	printf("%s", usage);
	return 1;
}


int disp_some_mem(char *token, interpreteur inter, Memory *mem)
{
	int p = 0; // pour les plages
	unsigned int va = 0;

	while(token != NULL)
	{

		if(is_hexa(token) == 0)
		{
			if(p != 0)
			{
				unsigned int va_2 = strtoul(token, NULL, 0);
				if(va < va_2) disp_plage(va, va_2, mem);
				else disp_plage(va_2, va, mem);
				p = 0;
				va = 0;
			}
			else {
				if(va != 0) disp_oct(va, mem);

				va = strtoul(token, NULL, 0);
			}
		}

		else if(strcmp(token, ":") == 0) p++;

		else 
		{
			WARNING_MSG("invalid token (%s)", token);
			return 1;
		}


		token = get_next_token(inter);
	}

	if(va != 0) disp_oct(va, mem);

	return 0;
}

int disp_some_reg(char *token, interpreteur inter, Registre reg[NB_REG])
{
	while (token != NULL) 
	{
		if(disp_reg(token, reg) != 0)
			return 1;

		token = get_next_token(inter);
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
	int k;
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

void disp_map(Memory *mem)
{
	int i;
	for(i = 0; i < NB_SEC; i++)
	{
		if(mem->map[i].size != 0)
		{
			print_section_raw_content(mem->map[i].name, (unsigned int) mem->map[i].va, mem->map[i].content, mem->map[i].size);
		}
	}

	printf("\n");
}

void disp_plage (unsigned int va_1, unsigned int va_2, Memory *mem) // on suppose va_1 > va_2
{
  /* Les octets sont dans des segments sinon ils sont nuls 
  */

	vaddr32 va = va_1;
	vaddr32 va_start;
	vaddr32 va_end;
	vaddr32 size;
	int k = 0;
	int i;

	for (i = 0; i < NB_SEC; i++)
	{
		va_start = mem->map[i].va;
		size = mem->map[i].size;
		va_end = va_start + size;

		while (va < va_start)
		{
			if (k % 16 == 0)
				printf("\n  0x%08x ", va);

			printf("00 ");

			if(va == va_2)
			{
				printf("\n");
				return;
			}

			va++;
			k++;
		}

		while (va <= va_end)
		{

			if (k % 16 == 0)
				printf("\n  0x%08x ", va);

			print_section_bytes(va - va_start, mem->map[i].content);

			if (va == va_2)
			{
				printf("\n");
				return;
			}

			va++;
			k++;
		}
	}

	while (va <= va_2)
	{
		if (k % 16 == 0)
			printf("\n  0x%08x ", va);

		printf("00 ");

		va++;
		k++;
	}

	printf("\n");
}

void disp_oct(unsigned int va, Memory *mem) // V2
{
	printf(" 0x%08x %02x\n", va, get_byte(va, mem));
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
