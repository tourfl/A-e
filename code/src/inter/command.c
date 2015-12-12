
#include "inter/command.h" // inclut interpreteur, dic et mémoire

#include "inter/notify.h" // Pour les messages de contrôle, inclut stdlib et stdio
#include <string.h> // pour strcmp notamment
#include <math.h> // je ne sais plus pour quoi
#include "dic/dic.h" // Pour lire le dictionnaire pardi


int exitcmd() {
    INFO_MSG("Bye bye !");
    return CMD_EXIT_RETURN_VALUE;
}


/* Fonction appelée dans le main, qui sert à réorienter le programme vers les commandes */

int execute_cmd(Emulator *emul) {
    interpreteur inter = emul->inter;
    char cmdStr[MAX_STR];
    memset( cmdStr, '\0', MAX_STR );



    DEBUG_MSG("input '%s'", inter->input);

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
        return loadcmd(emul);
    }
    else if(strcmp(token, "disp") == 0) {
	return dispcmd(emul);
    }
    else if(strcmp(token, "set") == 0) {
	return setcmd(emul);
    }
    else if(strcmp(token, "assert") == 0)
    {
    	return assert(emul);
    }
    else if(strcmp(token, "disasm") == 0)
    {
        return disasm_cmd(emul);
    }
    else if(strcmp(token, "dic") == 0)
    {
    	return disp_dic(emul->dic);
    }
    else if(strcmp(token, "run") == 0)
    {
        return run_cmd(emul);
    }
    else if(strcmp(token, "step") == 0)
    {
        return step_cmd(emul);
    }
    else if(strcmp(token, "debug") == 0)
        return CMD_DEBUG_RETURN_VALUE;

    else if(strcmp(token, "resume") == 0)
        return CMD_RESUME_RETURN_VALUE;

    WARNING_MSG("Unknown Command : '%s'\n", cmdStr);
    return CMD_UNKOWN_RETURN_VALUE;
}

