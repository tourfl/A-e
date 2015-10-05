
#include "common/interpreteur.h"
#include "mem/memory.h"

int loadcmd(interpreteur inter) {

	char * token=NULL;
    FILE *fo = NULL;
    char *va = "0x00001000";
	
	token = get_next_token(inter);

    // le token suivant est normalement le nom du fichier, on essaye donc de l'ouvrir

    if (token == NULL || get_type(token) == HEXA)
    {
        WARNING_MSG("please specifiy a file");
        return 1;
    }

    fo = fopen(token, "rb" );
    if ( fo == NULL ) 
    {
        perror( "fopen" );
        WARNING_MSG("file doesn't exist");
        return 1;
    }

    token = get_next_token(inter);

    if(token != NULL && get_type(token) != HEXA)
    {
        WARNING_MSG("value %s is not a valid address for load command\n",token);
        return 1;
    }

    else if(get_type(token) == HEXA)
        va = token; // On affecte la valeur spécifiée

    // Pas de valeur spécifiée, la mémoire sera implantée à partir de l'adresse 0x00001000

    // On récupère le contenu du fichier ELF ici en utilisant le programme de lecture des fichiers ELF fourni
    load_elf(fo, va); // fonction que l'on trouve dans memory.c

    fclose(fo);

    return 0;
}



int dispcmd (interpreteur inter) {
	
	
	char* token= NULL;
	token = get_next_token (inter);
	
	
	if (token == NULL ){
		WARNING_MSG("Spécifiez la mémoire à afficher");
	}
	
	else if (strcmp(token, "mem") == 0){
		token = get_next_token (inter);
		if (strcmp(token, "map") == 0) {
			//AFFICHER LA CARTE MEMOIRE;
			// fonction disp_mem_map;
			return 0;
		}
		else if (is_hexa(token) == 1) {
			//Afficher la plage de memoire;
			// fonction disp_mem_plage;
			return 0;
		}
		else {
			WARNING_MSG("%s n'est pas un bon argument pour disp mem \n",token);
			return 1;
		}
	}
	
	else if (strcmp(token, "reg") == 0) {
		token = get_next_token (inter);
		//Afficher les valeur du/des registre(s) mentionné(s);
		// fonction disp_reg
	}
	
	else {
		WARNING_MSG("Mauvaise utilisation de la fonction disp\n");
		return 1;
	}
}
