
#include "com/command.h" // inclut interpreteur et mémoire
#include "com/notify.h" // Pour les messages de contrôle, inclut stdlib et stdio
#include <string.h> // pour strcmp notamment
#include <math.h> // je ne sais plus pour quoi
#include "com/dic.h" // Pour lire le dictionnaire pardi


int exitcmd(interpreteur inter) {
    INFO_MSG("Bye bye !");
    return CMD_EXIT_RETURN_VALUE;
}


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
    else if(strcmp(token, "dic") == 0)
    {
    	return disp_dic();
    }

    WARNING_MSG("Unknown Command : '%s'\n", cmdStr);
    return CMD_UNKOWN_RETURN_VALUE;
}

int loadcmd(interpreteur inter, Memory *mem) {

	DEBUG_MSG("Load Command entered");

	char * token=NULL;
    FILE *fo = NULL;

    unsigned int va = START_MEM;

	
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

    DEBUG_MSG("file %s opened", token);

    token = get_next_token(inter);

    if(token == NULL)
    	DEBUG_MSG("No address specified");

    else if(is_hexa(token) == 0)
    {
    	DEBUG_MSG("Address specified : %s", token);
        va = strtoul(token, NULL, 0);

        if(va % 4096 != 0) // on arrondit au ko supérieur
        	va = (va/4096 + 1) * 4096;
    }
    else
    {
        WARNING_MSG("value %s is not a valid address for load command\n", token);
        return 1;
    }

    // On récupère le contenu du fichier ELF puis on le charge en mémoire
    load_elf_in_mem(fo, mem, va);

    fclose(fo);

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
	
	else if (strcmp(token, "mem") == 0) {

		token = get_next_token(inter);

		if (strcmp(token, "map") == 0)
		{
			disp_map(mem); // On affiche toute la mémoire
			return 0;
		}

		int j = 3; // compte les token
		int p = 0; // pour les plages
		unsigned int va = 0;

		while(token != NULL)
		{

			if(is_hexa(token) == 0)
			{
				if(p != 0)
				{
					unsigned int va_2 = strtoul(token, NULL, 0);
					if(va < va_2) disp_plage(va, va_2, mem);
					else disp_plage(va_2, va, mem);
					p = 0;
					va = 0;
				}
				else 
				{
					if(va != 0) disp_oct(va, mem);
					
					va = strtoul(token, NULL, 0);
				}
			}

			else if(strcmp(token, ":") == 0) p++;

			else 
			{
				WARNING_MSG("invalid token %i (%s)", j, token);
				return 1;
			}


			token = get_next_token(inter);
			j++;
		}

		if(va != 0) disp_oct(va, mem);
	}
	
	else if (strcmp(token, "reg") == 0) {

		token = get_next_token(inter);

		if(token == NULL)
		{
			WARNING_MSG("Please give a registre to display");
			return 1;
		}

		else if(strcmp (token, "all")==0){
			
			disp_all_regs(mem->reg);
		}

		else {
			 while (token != NULL) {
				disp_reg(token, mem->reg);
				token = get_next_token(inter);
			}
		}

		return 0;
	}
	
	else {
		WARNING_MSG("Mauvaise utilisation de la fonction disp\n");
		return 1;
	}

	return 0;
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
	
	/*else if (strcmp (token, "mem") == 0){
		char* type = NULL;
		char* valeur = NULL;
		char* adresse = NULL;
		type = get_next_token(inter);
		adresse = get_next_token(inter);
		valeur = get_next_token (inter);
		a = set_mem (inter, type, adresse, valeur, mem->map); //fonction qui copie "valeur" à l'adrese "adresse";
		return (a);
	}*/
	
	else if (strcmp (token, "reg") == 0){
		char* valeur = NULL;
		char* reg = NULL;
		reg = get_next_token(inter);
		valeur = get_next_token(inter);

		a = set_reg(reg, valeur, mem->reg); //fonction qui copie "valeur" dans le registre "reg"; 
		return (a); 
		
	}
	
	else {
		WARNING_MSG("Mauvaise utilisation de la fonction disp\n");
		return 1;
	}
	
}


//Pour la fonction setcmd
int set_reg (char* name, char* content, Registre reg[NB_REG]) {
	
	
	if(is_hexa(content) == 0 || is_dec(content) == 1 || is_oct(content) == 1) {int i;
    	char n[4]; // la taille peut aller jusqu'à 4 : r15'\0'
    	unsigned int c = strtoul(content, NULL, 0);

    	for (i=0; i<NB_REG-1; i++){

    		sprintf(n, "r%i", i);

    		if (strcmp(name, n) == 0 ){
    			reg[i] = c;
    			return 0;
    		}

    	}
    	if (strcmp(name, "sp") == 0) {
    		reg[13] = c;
    		return 0;
    	}
    	else if (strcmp(name, "lr") == 0){
    		reg[14] = c;
    		return 0;
    	}
    	else if (strcmp(name, "pc") == 0) {
    		reg[15] = c;
    		return 0;
    	}
    	else if (strcmp(name, "aspr") == 0) {
    		reg[16] = c;
    		return 0;
    	}

    	else
    	{
    		WARNING_MSG("%s n'est pas un registre", name);
    		return 3;
    	}


    }

    else {
    	WARNING_MSG ("Mauvais format de la valeur à écrire\n");
    	return 2;
    }


}

//Pour la fonction setcmd;
/*int set_mem (interpreteur inter,char* type,char* adresse,char* valeur, Segment map[]) {

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
}	*/


//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------//



//*int assertcmd (interpreteur inter, Memory mem) {



