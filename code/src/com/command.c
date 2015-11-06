
#include "com/command.h" // inclut interpreteur, dic et mémoire
#include "com/notify.h" // Pour les messages de contrôle, inclut stdlib et stdio
#include <string.h> // pour strcmp notamment
#include <math.h> // je ne sais plus pour quoi
#include "com/dic.h" // Pour lire le dictionnaire pardi


int exitcmd(interpreteur inter) {
    INFO_MSG("Bye bye !");
    return CMD_EXIT_RETURN_VALUE;
}


/* Fonction appelée dans le main, qui sert à réorienter le programme vers les commandes */

int execute_cmd(interpreteur inter, Memory *mem, Dic *dic) {
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
    else if(strcmp(token, "set") == 0) {
	return setcmd(inter, mem);
    }
    else if(strcmp(token, "assert") == 0)
    {
    	return assert(inter, mem);
    }
    else if(strcmp(token, "disasm") == 0)
    {
        return disasm(inter, mem, dic);
    }
    else if(strcmp(token, "dic") == 0)
    {
    	return load_dic(dic);
    }
    else if(strcmp(token, "debug") == 0)
        return CMD_DEBUG_RETURN_VALUE;

    else if(strcmp(token, "resume") == 0)
        return CMD_RESUME_RETURN_VALUE;

    WARNING_MSG("Unknown Command : '%s'\n", cmdStr);
    return CMD_UNKOWN_RETURN_VALUE;
}

int loadcmd(interpreteur inter, Memory *mem) {

	char * name=NULL;
	char *virt_add=NULL;
	vaddr32 va = START_MEM; // virt_add en unsigned int
    FILE *fo = NULL;
    char usage[] = "Usage : load <nom_du_fichier> {<adresse>}\n";

	name = get_next_token(inter);
	virt_add = get_next_token(inter);

	if(name == NULL)
	{
		WARNING_MSG("Problem with tokens");
		printf("%s", usage);
		return 1;
	}

    fo = fopen(name, "r" );

    if ( fo == NULL ) 
    {
        perror( "fopen" );
        WARNING_MSG("file doesn't exist");
        return 2;
    }

    if(virt_add != NULL && is_hexa(virt_add) == 0)
    	va = strtoul(virt_add, NULL, 0);

    else WARNING_MSG("No or bad address specified");

    if(va % 4096 != 0) // on arrondit au ko supérieur
        va = (va/4096 + 1) * 4096;

    // On récupère le contenu du fichier ELF puis on le charge en mémoire
    load_elf_in_mem(fo, mem->map, va);

    fclose(fo);

    return 0;
}

