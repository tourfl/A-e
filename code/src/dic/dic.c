#include "com/dic.h"
#include <stdlib.h> // Pour free
#include <string.h> // Pour strtok
#include "com/notify.h"
#include <math.h> // pow







int to_strlist_no_nb(char *chaine, Strlist *l)
{
	char *new_chaine =  NULL;






	new_chaine = malloc((strlen(chaine) + 2) * sizeof(char));

	if(chaine == NULL)
	{
		DEBUG_MSG("Chaine vide");
		return 1;
	}

	if(new_chaine == NULL)
	{
		DEBUG_MSG("Problem with malloc");
		return 5;
	}

	new_chaine[0] = '2';
	new_chaine[1] = '/';

	if(strcat(new_chaine, chaine) == NULL)
	{
		DEBUG_MSG("Problem with strcat");
		return 6;
	}

	// printf("\nnew_chaine : %s\n", new_chaine);

	return to_strlist(new_chaine, l);
}





int to_strlist(char *chaine, Strlist *l) // p doit être initialisée
{
	char *token = NULL;
	int i;





	if(chaine == NULL)
	{
		DEBUG_MSG("Chaine vide");
		return 1;
	}

	token = strtok(chaine, "/"); // Que se passe-t-il s'il n'y a pas de / ? On suppose qu'au moins 1 token est trouvé

	if(is_dec(token) != 0)
	{
		DEBUG_MSG("Bad value for parameter number");
		return 2;
	}

	if(strtoul(token, NULL, 10) <= 0)
	{
		DEBUG_MSG("0 paramètres");
		return 3; // En soit il n'y a pas d'erreur... 0 ?
	}

	l->size = strtoul(token, NULL, 10); // on le fait après la condition pour le cas où nb < 0
	l->content = malloc(l->size * sizeof(char *));

	for (i = 0; i < l->size && token != NULL; ++i)
	{
		token = strtok(NULL, "/");

		if(token == NULL)
		{
			DEBUG_MSG("less token than expected in parameters");
			return 4;
		}

		l->content[i] = malloc(strlen(token) * sizeof(char));

		if(l->content[i] == NULL)
		{
			DEBUG_MSG("Problem with malloc");
			return 5;
		}

		if(strcpy(l->content[i], token) == NULL)
		{
			DEBUG_MSG("Problem with strcpy");
			return 7;
		}
	}

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
	here_n(0);
	del_strlist(&(ins->imm));
	here_n(1);
	del_strlist(&(ins->ext));

	here_n(2);
}







int load_ins(Instruction *ins, char *chaine)
{
	/*
	 * 2 boucles pour que strtok puisse fonctionner correctement
	 *
	 */

	char *token = NULL;
	char **tab_tok = NULL; // Attention aux problèmes de désallocation
	int i;
	int j;






	if(chaine == NULL)
		return 1;

	tab_tok = malloc(6 * sizeof(char *)); // tableau de 6 strings

	// printf("\nchaine : %s\n", chaine);

	token = strtok(chaine, " ");


	for(i = 0; token != NULL && i < 6; i++) 
	{
		*(tab_tok + i) = calloc(strlen(token), sizeof(char));

		if(*(tab_tok + i) == NULL)
		{
			DEBUG_MSG("Problem with malloc");

			for(j = 0; j < i; j++)
				free(*(tab_tok + j));

			free(tab_tok);

			return 5;
		}

		if(strcpy(*(tab_tok + i), token) == NULL)
		{
			DEBUG_MSG("Problem with strcpy");

			for(j = 0; j < i; j++)
				free(*(tab_tok + j));

			free(tab_tok);

			return 7;
		}

		printf("\ntoken %u : %s\n", i, tab_tok[i]);

		token = strtok(NULL, " ");
	}

	if(i != 6)
	{
		DEBUG_MSG("%u", i);
		WARNING_MSG("Not the right number of tokens");
		return 2;
	}

	for (i = 0; i < 6; ++i)
	{
		printf("token : %s\n", tab_tok[i]);

		switch (i)
		{
			case 0:
			to_strlist_no_nb(tab_tok[i], &(ins->commande)); // (!) utilise strtok
			break;

			case 1:
			to_strlist(tab_tok[i], &(ins->names));
			break;
			
			case 2:
			to_strlist_no_nb(tab_tok[i], &(ins->opcode));
			break;
			
			case 3:
			to_strlist(tab_tok[i], &(ins->reg));
			break;
			
			case 4:
			to_strlist(tab_tok[i], &(ins->imm));
			break;
			
			case 5:
			to_strlist(tab_tok[i], &(ins->ext));
			break;
		}
	}

	return 0;

	for(j = 0; j < i; j++)
		free(*(tab_tok + j));

	free(tab_tok);
}





void disp_ins(Instruction ins)
{
	if(ins.names.size != 0)
		printf("\n%s\n", ins.names.content[0]);
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

			
			
