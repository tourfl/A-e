
#include "common/command.h"

int loadcmd(interpreteur inter) {

	char * token=NULL;
    FILE *fo = NULL;
	
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

	switch(get_type(token)) {
        case HEXA:
            //sscanf(token,"%x", &hexValue);
	   /* charger le fichier Ã  l'adresse saisi */ 	//?????????? _loadcmd
            break;
        default :
            WARNING_MSG("value %s is not a valid argument of load command\n",token);
            return 1;
        }


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
			char* adresse =NULL;
			adresse = get_next_token(inter);
			adresse = get_next_token(inter);
			//Afficher la plage de memoire des adresses "token" à "adresse";
			// fonction disp_mem_plage;
			return 0;
		}
		else if (token == NULL){
			WARNING_MSG("Spécifiez la mémoire à afficher");
			return 1;
		}
		else {
			WARNING_MSG("%s n'est pas un bon argument pour disp mem \n",token);
			return 1;
		}
	}
	
	else if (strcmp(token, "reg") == 0) {
		
		char* registre = NULL;
		registre = get_next_token(inter);
		if(strcmp (registre, "all")==0){
			//Afficher le contenu de tous les registres;
			return 0;
		}
		else {
			while (registre != NULL){
				
				if (is_reg(registre) == 1 ) { /*Vérification de la bonne saisie du nom du registre*/
				//Afficher la valeur de "registre";
				// fonction disp_reg
				registre = get_next_token(inter);
				}
				
				else {
					WARNING_MSG ("Maivais format de la valeur à écrire\n");
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
		char* registre = NULL;
		registre = get_netxt_token (inter);
		valeur = get_next_token(inter);
		a = set_reg(inter,registre,valeur); //fonction qui copie "valeur" dans le registre "registre"; 
		return (a); 
		
	}
	
	else {
		WARNING_MSG("Mauvaise utilisation de la fonction disp\n");
		return 1;
	}
	
}



//Pour la fonction setcmd
int set_reg (interpreteur inter, char* registre, char* valeur) {
	
	
	if(is_hexa(valeur) == 1 || is_dec(valeur) == 1 || is_oct(valeur) == 1) {
		
		if (is_reg(registre) == 1 ) { /*Vérification de la bonne saisie du nom du registre*/
			//Ecrire valeur dans le registre;
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

//Pour la Fonction setcmd;
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


