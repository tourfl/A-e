#include "com/dic.h"
#include <stdlib.h> // Pour free
#include <string.h> // Pour strtok
#include "com/notify.h"
#include <math.h> // pow






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

	dic = fopen("lib/instructions.dic", "r");

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

	fd = fopen("lib/instructions.dic", "r");

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






void init_ins(Instruction *ins) // pour éviter les bugs lors de la désallocation
{
	ins->commande = NULL;
	ins->encoding = NULL;
	ins->name_in_IT = NULL;
	ins->name_out_IT = NULL;
}






void del_ins(Instruction *ins) // Les éléments alloués sont libérés
{
	free(ins->commande);
	free(ins->encoding);
	free(ins->name_in_IT);
	free(ins->name_out_IT);
}







int load_ins(Instruction *ins, char *chaine) // On suppose qu'il n'y a pas d'erreur dans les instructions
{
	if(chaine == NULL)
		return 1;

	char *token = NULL;
	char *token2 = NULL;
	char *token3 = NULL;

	token = strtok(chaine, " ");
	token2 = strtok(NULL, " ");

	ins->mask = strtoul(strtok(NULL, " "), NULL, 16);
	ins->opcode = strtoul(strtok(NULL, " "), NULL, 16);

	// On réutilise strtok avec token et token2

	token3 = strtok(token, "/");
	ins->commande = malloc(strlen(token3) * sizeof(char));
	strcpy(ins->commande, token3);

	token3 = strtok(NULL, "/");
	ins->encoding = malloc(strlen(token3) * sizeof(char));
	strcpy(ins->encoding, token3);

	token3 = strtok(token2, "/");
	ins->name_in_IT = malloc(strlen(token3) * sizeof(char));
	strcpy(ins->name_in_IT, token3);

	token3 = strtok(NULL, "/");

	if(token3 != NULL)
	{
		ins->name_out_IT = malloc(strlen(token3) * sizeof(char));
		strcpy(ins->name_out_IT, token); // NULL s'il n'y a pas un 2ème /
	}

	// printf("\n %s \n", ins->commande);
	// printf("\n %s \n", ins->encoding);
	// printf("\n %s \n", ins->name_in_IT);
	// printf("\n %s \n", ins->name_out_IT);
	// printf("\n %08x \n", ins->mask);
	// printf("\n %08x \n", ins->opcode);

	return 0;
}





void disp_ins(Instruction ins)
{
	if(ins.name_in_IT != NULL)
		printf("\n%s\n", ins.name_in_IT);
}





Instruction get_ins (word in, Instruction ins[], int taille) // retourne l'instruction en question s'il y a match, NULL sinon;
{
	char* mask = NULL;
	char* op_code = NULL;
	char *cin = NULL;
	char* a = NULL;
	char* b = NULL;
	int i=0;
	Instruction ins_vide;
	init_ins(&ins_vide);

	printf("word: %8x\n", in);


	cin = int_to_bin(in, taille);
	printf("in: %s\n", cin);

	while (i<NB_INS_32) {

		mask = int_to_bin (ins[i].mask , taille);
		op_code =  int_to_bin (ins[i].opcode , taille);
		
		a = bin_x_bin (cin , mask , taille); //on compare l'entrée et le masque;
		b = bin_x_bin (op_code, mask, taille); //on compare l'op code et le masque;

		if(strcmp (a,b) == 0) {
		printf("in: %s\nmask: %s\topcode: %s\na: %s\tb: %s\n",cin, mask, op_code, a, b);
			here;
			free(mask);
			here_n(2);
			free(op_code);
			here_n(3);
			free(a);
			here_n(4);
			free(b);
			here_n(5);
			free(cin);
			here_n(6);
			return ins[i];
		}
		free(mask);
		free(op_code);
		free(a);
		free(b);
		i++;

	}

	free(cin);

	WARNING_MSG("unable to find instruction");

	return ins_vide;
}
			
			
