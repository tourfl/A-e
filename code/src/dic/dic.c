#include "com/dic.h"
#include <stdlib.h> // Pour free
#include <string.h> // Pour strtok
#include "com/notify.h"






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








Instruction get_ins32(word in, Dic *dic) { return get_ins(in, dic->ins32); }
Instruction get_ins16(word in, Dic *dic) { return get_ins(in, dic->ins16); }






void init_ins(Instruction *ins) // pour éviter les bugs lors de la désallocation
{
	ins->commande = NULL;
	ins->encoding = NULL;
	ins->name_in_IT = NULL;
	ins->name_out_IT = NULL;
}






void del_ins(Instruction *ins) // Les éléments alloués sont libérés
{
	if(ins->commande != NULL)
		free(ins->commande);

	if(ins->encoding != NULL)
		free(ins->encoding);

	if(ins->name_in_IT != NULL)
		free(ins->name_in_IT);

	if(ins->name_out_IT != NULL)
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







Instruction get_ins(word in, Instruction ins[])
{
	return ins[0];
}
