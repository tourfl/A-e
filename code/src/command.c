#include "common/interpreteur.h"
#include "mem/memory.h"
#include "common/command.h"




//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//




int loadcmd(interpreteur inter) {

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

    // On récupère le contenu du fichier ELF ici en utilisant le programme de lecture des fichiers ELF fourni
    load_elf(fo, va); // fonction que l'on trouve dans memory.c

    return 0;
}


//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//



int dispcmd (interpreteur inter) {
	
	
	char* token= NULL;
	token = get_next_token (inter);
	
	
	if (token == NULL ){
		WARNING_MSG("Spécifiez la mémoire à afficher\n");
	}
	
	else if (strcmp(token, "mem") == 0){
		int a;
		char* adresse = NULL;
		token = get_next_token (inter);
		adresse = get_next_token (inter);
		adresse = get_next_token (inter);
		a = disp_mem (inter, token, adresse);
		return a;
	}
	
	else if (strcmp(token, "reg") == 0) {
		
		char* reg = NULL;
		reg = get_next_token(inter);
		if(strcmp (reg, "all")==0){
			int i = 0;
			for (i=0; i<=12 ; i++) {
				printf ("%s : %s\n", r[i].name , r[i].valeur);
			}
			printf ("sp : %s\n lr : %s\n pc : %s\n aspr : %s\n",sp.valeur,lr.valeur,pc.valeur,aspr.valeur);
			return 0;
		}
		else {
			registre r;
			while (reg != NULL){
				
				if (is_reg(reg) == 1 ) {/*Vérification de la bonne saisie du nom du registre*/
				r = which(reg);
				printf ("%s : %s\n", r.name , r.valeur);
				reg = get_next_token(inter);
				}
				
				else {
					WARNING_MSG ("Erreur lors de la saisie des registres\n");
					return 1;
				}
			}
			return 0;
		}
	}
	
	else {
		WARNING_MSG("Mauvaise utilisation de la fonction disp\n");
		return 1;
	}
}

registre wich_reg (char*nom) {
	registre r;
	strcpy(r.name, nom);
	int i;
	for (i=0; i<=12; i++){
		if (strcmp(r.name, r[i]) == 0 ){
			return r[i];
		}
	}
	if (strcmp(r.name, sp) == 1) {
		return sp;
	}
	else if (strcmp(r.name, lr) == 1){
		return lr;
	}
	else if (strcmp(r.name, pc) == 1) {
		return pc;
	}
	else if (strcmp(r.name, aspr) == 1) {
		return aspr;
	}

	else return 0;
	
}


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


//Pour la fonction discmd;
int set_mem (interpreteur inter, char* token, char* adresse) {
	
	if (strcmp(token, "map") == 0) {
		//Afficher la carte mémoire;
		return 0;
	}
	else if (is_hexa(token) == 1) {
			if (is_hexa(adresse) == 1) {
				//Afficher la plage de memoire des adresses "token" à "adresse";
				return 0;
			}
			else {
				WARNING_MSG ("Erreur dand la saisie des adresses\n");
				return 1;
			}
		}
	else if (token == NULL){
			WARNING_MSG("Spécifiez la mémoire à afficher\n");
			return 1;
		}
	else {
			WARNING_MSG("%s n'est pas un bon argument pour disp mem \n",token);
			return 1;
		}
}




//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//




int setcmd (interpreteur inter){
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
		a = set_mem (inter, type, adresse, valeur); //fonction qui copie "valeur" à "adresse";
		return (a);
	}
	
	else if (strcmp (token, "reg") == 0){
		char* valeur = NULL;
		char* reg = NULL;
		reg = get_netxt_token (inter);
		valeur = get_next_token(inter);
		a = set_reg(inter,reg,valeur); //fonction qui copie "valeur" dans le registre "registre"; 
		return (a); 
		
	}
	
	else {
		WARNING_MSG("Mauvaise utilisation de la fonction disp\n");
		return 1;
	}
	
}





//Pour la fonction setcmd
int set_reg (interpreteur inter, char* reg, char* valeur) {
	
	
	if(is_hexa(valeur) == 1 || is_dec(valeur) == 1 || is_oct(valeur) == 1) {
		
		if (is_reg(reg) == 1 ) { /*Vérification de la bonne saisie du nom du registre*/
			r=which_reg(reg);
			strcpy(r.name,valeur);
			return 0;
		}
		
		else {
			WARNING_MSG ("Mauvaise saisie du nom du registre\n");
			return 1;
		}
	}
	
	else {
		WARNING_MSG ("Maivais format de la valeur à écrire\n");
		return 1;
	}
	
	
}

//Pour la fonction setcmd;
int set_mem (interpreteur inter,char* type,char* adresse,char* valeur) {

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



