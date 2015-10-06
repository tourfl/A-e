
#include "common/command.h"




/* Fonction appelée dans le main, qui sert à réorienter le programme vers les commandes */

int execute_cmd(interpreteur inter, Memory *mem) {
    DEBUG_MSG("input '%s'", inter->input);
    char cmdStr[MAX_STR];
    memset( cmdStr, '\0', MAX_STR );

    /* gestion des commandes vides, commentaires, etc*/
    if(strlen(inter->input) == 0
            || sscanf(inter->input, "%s", cmdStr) == 0
            || strlen(cmdStr) == 0
            || cmdStr[0] == '#') { /* ligne commence par # => commentaire*/
        return CMD_OK_RETURN_VALUE;
    }

    /*on identifie la commande avec un premier appel à get_next_token*/
    char * token = get_next_token(inter);

    if(strcmp(token, "exit") == 0) {
        return exitcmd(inter);
    }
    else if(strcmp(token, "load") == 0) {
        return loadcmd(inter, mem);
    }
    else if(strcmp(token, "disp") == 0) {
	return dispcmd(inter, mem);
    }
    else if(strcmp(token, "disasm") == 0) {
	//return disasmcmd(inter);
    }
    else if(strcmp(token, "set") == 0) {
	return setcmd(inter, mem);
    }

    WARNING_MSG("Unknown Command : '%s'\n", cmdStr);
    return CMD_UNKOWN_RETURN_VALUE;
}


int loadcmd(interpreteur inter, Memory *mem) {

	char * token=NULL;
    FILE *fo = NULL;

    char *va = "0x00001000";

	
	token = get_next_token(inter);

    // le token suivant est normalement le nom du fichier, on essaye donc de l'ouvrir

    if (token == NULL)
    {
        WARNING_MSG("please specifiy a file");
        return 1;
    }

    fo = fopen(token, "r" );
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

    // On récupère le contenu du fichier ELF puis on le charge en mémoire
    load_elf_in_mem(fo, *va, mem->map);

    return 0;
}


//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//



int dispcmd (interpreteur inter, Memory *mem) {
	
	
	char* token= NULL;
	token = get_next_token (inter);
	
	
	if (token == NULL ){
		WARNING_MSG("Spécifiez la mémoire à afficher\n");
		return 1;
	}
	
	else if (strcmp(token, "mem") == 0){
		if (strcmp(token, "map") == 0)
		{
			disp_mem(0x00000000, 0xffffffff, mem->map); // On affiche toute la mémoire
			return 0;
		}
		else if(is_hexa(token) == 0)
		{


			char *va_1 = token;

			char *va_2 = get_next_token(inter);

			if(is_hexa(va_2))
			{
				// On affiche la mémoire comprise entre va_1 et va_2
				return disp_mem (va_1, va_2, mem->map);
			}

			else
			{
				// Sinon on pourrait afficher toute la mémoire à partir de la seule adresse spécifiée
				WARNING_MSG("Seconde adresse mémoire invalide \n");
			}
		}
		else
		{
			WARNING_MSG("Spécifiez la zone mémoire à afficher\n");
			return 2;
		}
	}
	
	else if (strcmp(token, "reg") == 0) {
		
		char* token = NULL;
		token = get_next_token(inter);
		if(strcmp (token, "all")==0){
			int i = 0;
			for (i=0; i<15 ; i++) {
				printf ("%s : %s\n", mem->reg->r[i].name , mem->reg->r[i].content);
			}
			printf ("sp : %s\n lr : %s\n pc : %s\n apsr : %s\n",mem->reg->sp->content, mem->reg->lr->content, mem->reg->pc->content, mem->reg->apsr->content);
			return 0;
		}
		else {
			Registre *r;
			while (token != NULL){

				r = which_reg(token, mem->reg);
				
				if (r != NULL) {/*Vérification de la bonne saisie du nom du registre*/
					printf ("%s : %s\n", r->name , r->content);
					token = get_next_token(inter);
				}
				
				else {
					WARNING_MSG ("Erreur lors de la saisie des reg\n");
					return 1;
				}
			}
		}

		return 0;
	}
	
	else {
		WARNING_MSG("Mauvaise utilisation de la fonction disp\n");
		return 1;
	}
}

Registre * which_reg (char *nom, Registres *reg) {
	int i;
	for (i=0; i<15; i++){
		if (strcmp(nom, reg->r[i].name) == 0 ){
			 return &(reg->r[i]);
		}
	}
	if (strcmp(nom, reg->sp->name) == 0) {
		return reg->sp;
	}
	else if (strcmp(nom, reg->lr->name) == 0){
		return reg->lr;
	}
	else if (strcmp(nom, reg->pc->name) == 0) {
		return reg->pc;
	}
	else if (strcmp(nom, reg->apsr->name) == 0) {
		return reg->apsr;
	}

	return NULL; // Si aucun des cas n'a été rencontré
	
}

/*

Cette fonction refait celle d'au dessus

int is_reg (char* nom) {
	registre r;
	strcpy(r.name, nom);
	int i;
	for (i=0; i<=12; i++){
		if (strcmp(r.name, r[i]) == 0 ){
			return 1;
		}
	}
	if (strcmp(r.name, sp) == 1 ||strcmp(r.name, lr) == 1 || strcmp(r.name, pc) == 1 || strcmp(r.name, aspr) == 1 ){
			return 1;
		}
	else return 0;
	
}
*/


//Pour la fonction discmd;
<<<<<<< HEAD
int disp_mem (char *va_1, char *va_2, Map *map) {
=======
int disp_mem (interpreteur inter, char* token, char* adresse) {
>>>>>>> 912151e6d0c99c6c1f8590fbe6138aa2d77c14ac
	
	if (is_hexa(va_1) == 1) {
			if (is_hexa(va_2) == 1) {
				//Afficher la plage de memoire des adresses "token" à "adresse";
				return 0;
			}
			else {
				WARNING_MSG("%n n'est pas un bon argument pour disp mem \n", va_2);
				return 1;
			}
		}
	else {
			WARNING_MSG("%n n'est pas un bon argument pour disp mem \n", va_1);
			return 1;
		}
}




//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//




int setcmd (interpreteur inter, Memory *mem){
	char* token;
	token = get_next_token (inter);
	int a;
	
	if (token == NULL ){
		WARNING_MSG("Spécifiez la mémoire à modifier");
		return 0;
	}
	
	else if (strcmp (token, "mem") == 0){
		char* type = NULL;
		char* valeur = NULL;
		char* adresse = NULL;
		type = get_next_token(inter);
		adresse = get_next_token(inter);
		valeur = get_next_token (inter);
		a = set_mem (inter, type, adresse, valeur, mem->map); //fonction qui copie "valeur" à "adresse";
		return (a);
	}
	
	else if (strcmp (token, "reg") == 0){
		char* valeur = NULL;
		char* reg = NULL;
		reg = get_next_token (inter);
		valeur = get_next_token(inter);
		a = set_reg(inter,reg,valeur, mem->reg); //fonction qui copie "valeur" dans le registre "reg"; 
		return (a); 
		
	}
	
	else {
		WARNING_MSG("Mauvaise utilisation de la fonction disp\n");
		return 1;
	}
	
}





//Pour la fonction setcmd
int set_reg (interpreteur inter, char* r_name, char* r_content, Registres *reg) {
	
	
	if(is_hexa(r_content) == 1 || is_dec(r_content) == 1 || is_oct(r_content) == 1) {
		
		Registre *r = which_reg(r_name, reg);

		if (r != NULL) { /*Vérification de la bonne saisie du nom du registre*/
			strcpy(r->content, r_content);
			return 0;
		}
		
		else {
			WARNING_MSG ("Mauvaise saisie du nom du registre\n");
			return 1;
		}
	}
	
	else {
		WARNING_MSG ("Mauvais format de la valeur à écrire\n");
		return 1;
	}
	
	
}

//Pour la fonction setcmd;
int set_mem (interpreteur inter,char* type,char* adresse,char* valeur, Map *map) {

	char *token = get_next_token(inter);

	if (token == NULL ){
		WARNING_MSG("Spécifiez la mémoire à modifier");
		}
		
		
	else if (strcmp (type, "word") == 0 || strcmp (type, "byte") == 0  ){
	
			
		if(is_hexa(valeur) == 1 || is_dec(valeur) == 1 || is_oct(valeur) == 1) {
			//Inscrire cette valeur à l'adresse "adresse";
			return 0;
		}
			
		else {
			WARNING_MSG ("Mauvais format de la valeur\n");
			return 1;
		}	
	}
		
		
	else {
		WARNING_MSG("Mauvaise utilisation de la fonction disp\n");
		return 1;
	}
}	



