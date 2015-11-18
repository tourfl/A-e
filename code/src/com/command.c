
#include "com/command.h" // inclut interpreteur, dic et mémoire
#include "com/notify.h" // Pour les messages de contrôle, inclut stdlib et stdio
#include <string.h> // pour strcmp notamment
#include <math.h> // je ne sais plus pour quoi
#include "com/dic.h" // Pour lire le dictionnaire pardi


int exitcmd() {
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
        return exitcmd();
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
    	return disp_dic(dic);
    }
    else if(strcmp(token, "run") == 0)
    {
        return run_cmd(inter, mem);
    }
    else if(strcmp(token, "step") == 0)
    {
        return step_cmd(inter, mem);
    }
    else if(strcmp(token, "debug") == 0)
        return CMD_DEBUG_RETURN_VALUE;

    else if(strcmp(token, "resume") == 0)
        return CMD_RESUME_RETURN_VALUE;

    WARNING_MSG("Unknown Command : '%s'\n", cmdStr);
    return CMD_UNKOWN_RETURN_VALUE;
}



/*
* valeur de retour : 0 succès, 1 : un argment nul, 11 mauvaises valeurs, 2 une seule adresse
* 
*
*/

int parse_plage(interpreteur inter, Plage *p)
{
    int r = 0;
    vaddr32 va = 0;
    char *ponc = NULL;



    

    r = get_next_if_addr(inter, &(p->start));

    if(r == 1 || r == 11)
        return r;

    ponc = get_next_token(inter);

    if(ponc == NULL)
    {
        return 1;
    }
    else if(is_addr(ponc) == 0)
    {
        p->end = p->start;

        reset_pos(inter, inter->pos - 1);

        return 2;
    }
    if (strcmp(ponc, ":") == 0)
    {
        r = get_next_if_addr(inter, &va);

        if (r == 1 || r == 11)
            return r;

        if(p->start > va)
        {
            p->end = p->start;
            p->start = va;
        }

        else
        {
            p->end = va;
        }
    }
    else if (strcmp(ponc, "+") == 0)
    {
        r = get_next_if_fig(inter, &va);

        if (r == 1 || r == 11)
            return r;

        if(va < (0xffffffff - p->start)) // On évite les dépassements de mémoire
        {
            p->end = p->start + va;
        }

        else {
            return 11;
        }
    }
    else return 11;

    return 0;
}

