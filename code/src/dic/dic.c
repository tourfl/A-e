#include "common/dic.h"

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
	
	for(i = 0; i < taille; i++)
	{
		if(bin_1[i] == '0' || bin_2[i] == '0')
			bin_1[i] == '0';
			
		else bin_1[i] == '1';
	}
	
	return bin_1;
}

int comp_instr_16(unsigned int inst, char ligne[TAILLE_MAX])
{
	// on utilise une fonction qui sectionne la ligne avec un espace ou un slash comme délimiteur
	
	char * token = NULL;
	int i = 1;
	
	
	token = strtok (ligne," /"); // token 1 contient la commande
	
	while (token != NULL)
	{
		printf ("%s\n", token);
		token = strtok (NULL, " /");
		i++;
		
		if(i == 5) // token 5 contient le masque
		{
		}
		
		else if (i == 6) // token 6 contient l'opcode
		{
		}
			
  	}
	
}

int comp_inst_32(unsigned long inst, char ligne[TAILLE_MAX])
{
}



