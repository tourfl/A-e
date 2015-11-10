#include "com/dic.h"
#include <stdlib.h> // Pour free
#include <string.h> // Pour strtok
#include "com/bits.h" // flip_endianness
#include "com/notify.h"
#include <math.h> // pow







int to_strlist_no_nb(char *chaine, Strlist *l)
{
	char *new_chaine =  NULL;






	if(chaine == NULL)
		return 1;

	new_chaine = calloc((strlen(chaine) + 2), sizeof(char));

	if(new_chaine == NULL)
		return 5;

	new_chaine[0] = '2';
	new_chaine[1] = '/';

	if(strcat(new_chaine, chaine) == NULL)
	{
		DEBUG_MSG("Problem with strcat");
		return 6;
	}

	return to_strlist(new_chaine, l);
}





int to_strlist(char *chaine, Strlist *l) // p doit être initialisée
{
	char *token = NULL, *str = NULL, *saveptr = NULL;
	int i;





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

	if(i != l->size + 1) // l->size
		return 4; // Problème dans le nombre de tokens


	return 0;
}




void init_ins(Instruction *ins) // pour éviter les bugs lors de la désallocation
{
	init_strlist(&(ins->commande));
	init_strlist(&(ins->names));  
	init_strlist(&(ins->opcode)); 

	// Paramètres :

	init_strlist(&(ins->reg));
	init_strlist(&(ins->imm));
	init_strlist(&(ins->ext));
}






void del_ins(Instruction *ins) // Les éléments alloués sont libérés
{
	del_strlist(&(ins->commande));
	del_strlist(&(ins->names));  
	del_strlist(&(ins->opcode)); 


	// Paramètres :

	del_strlist(&(ins->reg));
	del_strlist(&(ins->imm));
	del_strlist(&(ins->ext));
}







int load_ins(Instruction *ins, char *chaine)
{
	char *token = NULL, *str = NULL, *saveptr = NULL;
	int i, p = 0;


	// V3

	if(chaine == NULL)
		return 1;

	for (i = 0, str = chaine; ; ++i, str = NULL)
	{
		token = strtok_r(str, " ", &saveptr);

		if(token == NULL || i == 6)
			break;

		switch (i)
		{
			case 0:
			p = to_strlist_no_nb(token, &(ins->commande)); // il faut que le contenu de token soit copié (!)
			break;

			case 1:
			p = to_strlist(token, &(ins->names));
			break;
			
			case 2:
			p = to_strlist_no_nb(token, &(ins->opcode));
			break;
			
			case 3:
			p = to_strlist(token, &(ins->reg));
			break;
			
			case 4:
			p = to_strlist(token, &(ins->imm));
			break;
			
			case 5:
			p = to_strlist(token, &(ins->ext));
			break;
		}

		if(p != 0 && p != 3)
			return p;

	}

	return 0;
}





void disp_ins(Instruction ins)
{
	int i;
	char imm[33] = {0};



	if(ins.names.size != 0)
		printf("\n%s ", ins.names.content[0]);

	for(i = 0; i < ins.reg.size; i++)
	{
		if(i != 0)
			printf(",");

		// printf("\n %s\n", ins.reg.content[i]);

		printf(" r%lu", strtoul(ins.reg.content[i], NULL, 2));
	}

	for(i = 0; i < ins.imm.size; i++)
		strcat(imm, ins.imm.content[i]);

	if(ins.imm.size >= 0)
	{
		// printf("\n %s\n", imm);
		printf(", #%lu", strtoul(imm, NULL, 2));
	}
	printf("\n");
}





Instruction get_ins (word in, Instruction ins[], int taille) // retourne l'instruction en question s'il y a match, NULL sinon;
{
	/*
	 * Problème : les fichiers .o sont codés en little endian aligné (cf 2.4)
	 * Les masques sont en big endian
	 *
	 * ce problème est résolu dans la fonction disasm_plage
	 */

	char* mask = NULL;
	char* op_code = NULL;
	char *cin = NULL;
	char* a = NULL;
	char* b = NULL;
	int i=0;
	Instruction ins_vide;
	init_ins(&ins_vide);





	// printf("word: %8x\n", in);


	cin = int_to_bin(in, taille);
	// printf("\n in : %s \n", cin);

	while (i<NB_INS_32 && ins[i].commande.content != NULL) {

		mask = int_to_bin (strtoul(ins[i].opcode.content[0], NULL, 16) , taille);
		op_code =  int_to_bin (strtoul(ins[i].opcode.content[1], NULL, 16) , taille);
		
		a = bin_x_bin (cin , mask , taille); //on compare l'entrée et le masque;
		b = bin_x_bin (op_code, mask, taille); //on compare l'op code et le masque;

		if(strcmp (a,b) == 0) {
			// printf("in: %s\tn°%u\nmask: %s\topcode: %s\na: %s\tb: %s\n",cin, i, mask, op_code, a, b);
			free(mask);
			free(op_code);
			free(a);
			free(b);
			free(cin);
			return ins[i];
		}
		free(mask);
		free(op_code);
		free(a);
		free(b);
		i++;

	}

	free(cin);

	return ins_vide;
}





void init_ins_d(Ins_disasmd *ins_d, Instruction *ins)
{
	init_ins(ins_d);

	ins_d->commande = ins->commande;
	ins_d->names = ins->names;
	ins_d->opcode = ins->opcode;
}





int parse_params(word mot, Strlist *strl, Strlist *strl_d)
{
	int i;




	if(strl->size == 0)
		return 3;

	strl_d->content = calloc(strl->size, sizeof(char *));

	if(strl_d == NULL)
		return 1;

	strl_d->size = strl->size;

	for(i = 0; i < strl->size; i++)
	{
		parse_param(mot, strl->content[i], &(strl_d->content[i])); // Valeur de retour non-utilisée
		// printf("\ncontent[%u] = %s", i, strl_d->content[i]);
	}

	return 0;
}




int parse_param(word mot, char *plage, char **value_d)
{
	int i, r, start = 0, end = 0;
	char *word_bin = int_to_bin(mot, 32); // Problème avec la taille...

	// printf("\n %s \n", word_bin);
	flip_endianness(&word_bin);
	// printf("\nAfter flip: %s \n", word_bin);

	r = get_bounds(plage, &start, &end);
	// printf("\nr : %u et [%u:%u]\n", r, start, end);

	if(r != 2 && r != 1)
		return 2;

	if(start > end)
		return 3;

	*value_d = calloc(end - start + 2, sizeof(char));

	if(value_d == NULL)
		return 1;

	for(i = start; i <= end; i++)
	{
		// printf("end - i = %u\n", end - i);
		(*value_d)[end - i] = word_bin[i]; // A cause de l'endianness
	}

	// printf("\nvalue_d : %s\n", *value_d);

	return 0;
}






int get_bounds(char* plage, int *start, int *end)
{
	int r = sscanf(plage, "%u:%u", start, end);

	if(*end == 0)
		*end = *start;

	return r;
}






void init_dic(Dic *dic)
{

	int i;

	for (i = 0; i < NB_INS_32 + NB_INS_16; ++i)
	{
		if(i < NB_INS_32)
			init_ins(dic->ins32 + i); // instructions 32 bits

		else 
			init_ins(dic->ins16 + i - NB_INS_32); // instructions 16 bits
	}
}




void del_dic(Dic *dic)
{
	int i;

	for (i = 0; i < NB_INS_32 + NB_INS_16; ++i)
	{
		if(i < NB_INS_32)
			del_ins(dic->ins32 + i); // instructions 32 bits

		else 
			del_ins(dic->ins16 + i - NB_INS_32); // instructions 16 bits
	}
}







int disp_dic()
{
	FILE *dic = NULL;
	char chaine[TAILLE_MAX] = "";

	dic = fopen("lib/test.dic", "r");

	if(dic == NULL) return 1;

	while (fgets(chaine, TAILLE_MAX, dic) != NULL)
	{
		printf("%s", chaine);
	}

	fclose(dic);

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







char* bin_x_bin(char bin_1[], char bin_2[], int taille)
{
	int i;
	char *bin = NULL;

	bin = calloc(taille+1, sizeof(char)); // avec calloc, chaine remplie de '\0'
	
	for(i = 0; i < taille; i++)
	{
		if(bin_1[i] == '1' && bin_2[i] == '1')
			bin[i] = '1';
			
		else bin[i] = '0';
	}
	
	return bin;
}



int load_dic(Dic *dic)
{
	FILE *fd = NULL;
	char chaine[TAILLE_MAX] = "";
	int j = 0;

	fd = fopen("lib/test.dic", "r");

	if(fd == NULL) return 1;

	// On suppose qu'il n'y a pas d'erreurs dans le fichier instructions.dic

	while (fgets(chaine, TAILLE_MAX, fd) != NULL && j < NB_INS_32 + NB_INS_16) // les premières instructions sont en 32 bits (il y en a NB_INS_32)
	{
		if(j < NB_INS_32 && load_ins(dic->ins32 + j, chaine) != 0) // instructions 32 bits
			return 1; // l'instruction n'a pas correctement été chargée

		else if(j >= NB_INS_32 && load_ins(dic->ins16 + j - NB_INS_32, chaine) != 0) // instructions 16 bits
			return 1;

		j++;
	}

	fclose(fd);

	return 0;
}








Instruction get_ins32(word in, Dic *dic) { return get_ins(in, dic->ins32, 32); }
Instruction get_ins16(word in, Dic *dic) { return get_ins(in, dic->ins16, 16); }

			
			
