#include "com/dic.h" // inclut Strlist et word
#include <stdlib.h> // Pour free
#include <string.h> // Pour strtok
#include "com/bits.h" // flip_endianness
#include "com/notify.h"
#include <math.h> // pow






// reprise de to_strlist

int to_strtab(char *chaine, char **tab) {
	char *token = NULL, *str = NULL, *saveptr = NULL;
	int i;




	for (i = 0, str = chaine; i < 2; ++i, str = NULL)
	{
		token = strtok_r(str, "/", &saveptr);

		// DISP_TOKEN(i, token);

		if (token != NULL) {
			tab[i] = malloc(strlen(token) * sizeof(char));

			if(tab[i] == NULL)
				return 2;

			if(strcpy(tab[i], token) == NULL)
				return 5;
		}
		else {
			tab[i] = calloc(1, sizeof(char));

			if(tab[i] == NULL)
				return 2;
		}
	}

	return 0;
}






// Même principe que to_wrdtab

int to_wrdtab(char *chaine, word tab[]) {
	char *token = NULL, *str = NULL, *saveptr = NULL;
	int i;



	for (i = 0, str = chaine; i < 2; ++i, str = NULL)
	{
		token = strtok_r(str, "/", &saveptr);

		// DISP_TOKEN(i, token);

		if(token == NULL) // comprend le cas chaine == NULL
			tab[i] = 0; // 0 == '\0'

		else tab[i] = strtoul(token, NULL, 16);

		// printf("tab[%u] = %8x", i, tab[i]);
	}

	return 0;
}





int to_strlist(char *chaine, Strlist *l) // p doit être initialisée
{
	char *token = NULL, *str = NULL, *saveptr = NULL;
	int i;



	// V3



	// V2
	// Nécessite une fonction pour exploiter les valeurs de retour (non-écrite)
	for (i = 0, str = chaine; ; ++i, str = NULL)
	{
		token = strtok_r(str, "/", &saveptr);

		if(token == NULL) // comprend le cas chaine == NULL
			break;

		if(i == 0) {
			l->size = strtoul(token, NULL, 10);

			if(l->size >= 0) {
				l->content = malloc(l->size * sizeof(char *));

				if(l->content == NULL)
					return 2;
			}

			else return 3;
		}
		else if (i <= l->size) {
			l->content[i-1] = calloc(strlen(token), sizeof(char));

			if(l->content[i-1] == NULL)
				return 2;

			if(strcpy(l->content[i-1], token) == NULL)
				return 5;
		}
	}

	// disp_strlist(*l);

	if(i != l->size + 1) // l->size
		return 4; // Problème dans le nombre de tokens


	return 0;
}

int to_plgtab(char *chaine, Plgtab *plgt)
{
	Strlist strl;
	int i, r = 0;


	init_strlist(&strl);
	r = to_strlist(chaine, &strl);

	// disp_strlist(strl);

	if(r != 0)
		return r;

	plgt->plages = malloc(strl.size * sizeof(Plage));

	if(plgt->plages == NULL)
		return 2;

	plgt->size = strl.size;

	for (i = 0; i < strl.size; ++i)
	{
		if(sscanf(strl.content[i], "%u:%u", &(plgt->plages[i].start), &(plgt->plages[i].end)) == 1)
			plgt->plages[i].end = plgt->plages[i].start;


		// printf("  \nplage %u:\tstart: %u\tend: %u\n", i, plgt->plages[i].start, plgt->plages[i].end);
	}

	del_strlist(&strl);

	return 0;
}




void init_ins(Instruction *ins) // pour éviter les bugs lors de la désallocation
{
	ins->commande = NULL;
	ins->encoding = NULL;
	ins->name_in = NULL;
	ins->name_out = NULL;

	ins->mask = 0;
	ins->opcode = 0;

	// Paramètres :

	init_plgtab(&(ins->reg));
	init_plgtab(&(ins->imm));
	init_plgtab(&(ins->ext));
}






void del_ins(Instruction *ins) // Les éléments alloués sont libérés
{
	free(ins->commande);
	free(ins->encoding);
	free(ins->name_in);
	free(ins->name_out);


	// Paramètres :

	del_plgtab(&(ins->reg));
	del_plgtab(&(ins->imm));
	del_plgtab(&(ins->ext));
}







int load_ins(Instruction *ins, char *chaine)
{
	char *token = NULL, *str = NULL, *saveptr = NULL;
	int i, p = 0;




	// V4

	init_ins(ins);


	for (i = 0, str = chaine; i < 6; ++i, str = NULL)
	{
		token = strtok_r(str, " ", &saveptr);

		// DISP_TOKEN(i, token);

		if(token == NULL) // Comprend le cas chaine == NULL
			break;

		switch (i)
		{
			case 0: {
				Str str_tab[2]; // tableau de 2 char*

				p = to_strtab(token, str_tab);

				ins->commande = str_tab[0]; // alloué dynamiquement
				ins->encoding = str_tab[1];

				break;
			}

			case 1: {
				Str str_tab[2]; // tableau de 2 char*

				p = to_strtab(token, str_tab);

				ins->name_in = str_tab[0];
				ins->name_out = str_tab[1]; // juste un \0 s'il n'est pas dans la définition de l'instruction
				break;
			}
			
			case 2: {
				word wrd_tab[2];

				p = to_wrdtab(token, wrd_tab);

				ins->mask = wrd_tab[0]; // On suppose qu'il n'y a pas d'erreur
				ins->opcode = wrd_tab[1];
				break;
			}
			
			case 3 :
			p = to_plgtab(token, &(ins->reg));
			break;
			
			case 4 :
			p = to_plgtab(token, &(ins->imm));
			break;
			
			case 5 :
			p = to_plgtab(token, &(ins->ext));
			break;
		}

		if(p != 0 && p != 3)
			return p;
	}

	return 0;
}





// Pour une instruction extraite du dictionnaire

void disp_ins(Instruction ins)
{
	printf("\n\n\n  command: %s\tencoding: %s\n", ins.commande, ins.encoding);
	printf("  name_in: %s\tname_out: %s\n", ins.name_in, ins.name_out);
	printf("  mask: %8x\topcode: %8x\n", ins.mask, ins.opcode);
	printf("\n  reg ");
	disp_plgtab(ins.reg);
	printf("\n  imm ");
	disp_plgtab(ins.imm);
	printf("\n  ext ");
	disp_plgtab(ins.ext);
}





// Pour une instruction désassemblée

void disp_insd(Instruction ins)
{
	int i;
	char imm[33] = {0};



	printf("\n%s", ins.name_in);
	// printf("(%s/%s)", ins.commande, ins.encoding);

	for(i = 0; i < ins.reg.size; i++)
	{
		if(i != 0)
			printf(",");

		switch (ins.reg.plages[i].value)
		{
			case 13: {
				printf(" sp");
				break;
			}
			case 14: {
				printf(" lr");
				break;
			}
			case 15: {
				printf(" pc");
				break;
			}
			case 16: {
				printf(" apsr");
				break;
			}
			default: {
				printf(" r%u", ins.reg.plages[i].value);				
				break;
			}
		}
	}

	for(i = 0; i < ins.imm.size; i++)
		strcat(imm, int_to_bin(ins.imm.plages[i].value, ins.imm.plages[i].end - ins.imm.plages[i].start));

	// printf("\nimm:%s\n", imm);

	if(ins.imm.size > 0)
	{
		printf(", #%lu", strtoul(imm, NULL, 2));
	}
	// printf("\n");
}






int get_ins32(word in, Instruction *out, Dic *dic) {
	return get_ins(in, out, dic->ins32, dic->sz32);
}
int get_ins16(word in, Instruction *out, Dic *dic) {
	return get_ins(in, out, dic->ins16, dic->sz16);
}


/*
 * in : mot à décoder
 * out : instruction de sortie
 * dic : tableau d'instructions
 * sz_dic : nb d'instructions dans le tableau
 */


int get_ins(word in, Instruction *out, Instruction dic[], int sz_dic) // retourne l'instruction en question s'il y a match, NULL sinon;
{
	/*
	 * Problème : les fichiers .o sont codés en little endian aligné (cf 2.4)
	 * Les masques sont en big endian
	 *
	 * ce problème est résolu dans la fonction disasm_plage
	 */

	int i=0;



	//V2

	while (i<sz_dic && dic[i].commande != NULL) {

		if((in & dic[i].mask) == (dic[i].opcode & dic[i].mask)) {
			cpy_ins(out, &(dic[i]));
			return 0;
		}

		i++;
	}

	return 10;
}





void cpy_ins(Ins_disasmd *dest, Instruction *src)
{
	dest->commande = src->commande;
	dest->encoding = src->encoding;
	dest->name_in = src->name_in;
	dest->name_out = src->name_out;
	dest->opcode = src->opcode;

	dest->reg = src->reg;
	dest->imm = src->imm;
	dest->ext = src->ext;
}





int parse_params(word mot, Plgtab *tab)
{
	int i;




	if(tab->size == 0)
		return 3;

	for(i = 0; i < tab->size; i++)
	{
		parse_param(mot, &(tab->plages[i])); // Valeur de retour non-utilisée
		// printf("\nvalue[%u] = %u", i, tab_d->plages[i].value);
	}


	return 0;
}




int parse_param(word mot, Plage *p) // On utilise un paramètre de sortie pour renvoyer des codes d'erreur en valeur de retour
{
	int i, t = 16;
	char *word_bin = NULL, *val_bin = NULL;

	if(mot > pow(2, 16))
		t = 32;


	word_bin = int_to_bin(mot, t);
	val_bin = calloc(p->end - p->start + 2, sizeof(char));

	to_good_endianness(&word_bin, t);
	// printf("\nAfter flip: %s \n", word_bin);

	if(p->start > p->end)
		return 3;

	for(i = p->start; i <= p->end; i++)
	{
		val_bin[p->end - i] = word_bin[i]; // A cause de l'endianness
	}

	p->value = strtoul(val_bin, NULL, 2);

	// printf("\nval_bin : %s et value : %u\n", val_bin, p->value);

	return 0;
}






void init_dic(Dic *dic)
{
	dic->ins32 = NULL;
	dic->ins16 = NULL;

	dic->sz32 = 0;
	dic->sz16 = 0;
}




void del_dic(Dic *dic)
{
	int i;

	for (i = 0; i < dic->sz32; ++i) {
		del_ins(dic->ins32 + i); // instructions 32 bits
	}

	for (i = 0; i < dic->sz16; ++i) {
		del_ins(dic->ins16 + i); // instructions 16 bits
	}
}







int disp_dic(Dic *dic)
{
	int i;

	for (i = 0; i < dic->sz32; ++i) {
		disp_ins(*(dic->ins32 + i)); // instructions 32 bits
	}

	for (i = 0; i < dic->sz16; ++i) {
		disp_ins(*(dic->ins16 + i)); // instructions 16 bits
	}

	return 0;
}







char* int_to_bin(unsigned int n, char taille)
{
	int i, p;
	char *bin = NULL;
	
	bin = malloc((taille + 1) * sizeof(char));
	
	if(bin == NULL)
		return NULL;

	for(i = 0; i < taille; i++)
	{
		p = pow(2, taille-i-1);
	
		if(n/p == 1)
			*(bin + i) = '1';
		
		else if(n/p == 2)
			printf("attention taille");
			
		else
			*(bin + i) = '0';
			
		n = n % p;
	}
	
	*(bin + taille) = '\0';
	
	return bin;
}






int load_dic(Dic *dic)
{
	int r = 0;
	FILE *fd = NULL;



	init_dic(dic);

	fd = fopen("lib/instructions_32bits.dic", "r");
	r = load_ins_tab_from_file(&(dic->ins32), &(dic->sz32), fd);

	which_error(r);
	fclose(fd);

	fd = fopen("lib/instructions_16bits.dic", "r");
	r = load_ins_tab_from_file(&(dic->ins16), &(dic->sz16), fd);

	which_error(r);
	fclose(fd);

	return r;
}





/*
 * dic est un pointeur sur un tableau d'instruction
 * dic_sz est un pointeur sur la future taille de ce tableau
 */



int load_ins_tab_from_file(Instruction **dic, int *dic_sz, FILE *fd)
{
	char chaine[TAILLE_MAX] = "";
	char ch = 0;
	int sz = 0, i, l = 0;



	if(fd == NULL) return 9;

	sz++;

	while(!feof(fd)) // pour connaître la taille
	{
		ch = fgetc(fd);
		if(ch == '\n')
		{
			sz++;
		}
	}
	// printf("\nsz = %u\n", sz);

	if(fseek(fd, 0, SEEK_SET) != 0) // On se remet au début du fichier
		return 8;

	*dic = malloc(sz * sizeof(Instruction));

	if(*dic == NULL) {
		return 2;
	}

	*dic_sz = sz;

	// On suppose qu'il n'y a pas d'erreurs dans le fichier

	for (i = 0; i < sz; i++) // les premières instructions sont en 32 bits (il y en a NB_INS_32)
	{
		if (fgets(chaine, TAILLE_MAX, fd) == NULL)
			return 1;

		// printf("%s\n", chaine);

		l = load_ins(&((*dic)[i]), chaine); // &((*)) = prise de tête !


		if(l != 0)
			return l;

		// disp_ins((*dic)[i]);
	}

	return 0;
}


			
			
