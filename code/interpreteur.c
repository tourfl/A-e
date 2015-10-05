/**
 * @file emulARM.c
 * @author FranÃ§ois Cayre, Nicolas CastagnÃ©, FranÃ§ois Portet
 * @brief Main pour le dÃ©but du projet Ã©mulateur.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* la librairie readline */
#include <readline/readline.h>
#include <readline/history.h>

/* macros de DEBUG_MSG fournies , etc */
#include "common/notify.h"


/* prompt du mode shell interactif */
#define PROMPT_STRING "ARMShell : > "

/* taille max pour nos chaines de char */
#define MAX_STR 1024


/*************************************************************\
Valeur de retour speciales pour la fonction
	int execute_cmd(interpreteur inter) ;
Toute autre valeur signifie qu'une erreur est survenue
 \*************************************************************/
#define CMD_OK_RETURN_VALUE 0
#define CMD_EXIT_RETURN_VALUE -1
#define CMD_UNKOWN_RETURN_VALUE -2



enum {HEXA,OCT,DEC};

/* mode d'interaction avec l'interpreteur (exemple)*/
typedef enum {INTERACTIF,SCRIPT,DEBUG_MODE} inter_mode;

/* structure passÃ©e en parametre qui contient la connaissance de l'Ã©tat de
 * l'interprÃ©teur
 */
typedef struct {
    inter_mode mode;
    char input[MAX_STR];
    char * from;
    char first_token;
} *interpreteur;



interpreteur init_inter(void) {
    interpreteur inter = calloc(1,sizeof(*inter));
    if (inter ==NULL)
        ERROR_MSG("impossible d'allouer un nouvel interpreteur");
    return inter;
}



int get_type(char* chaine) {
    if (is_hexa(chaine))
        return HEXA;
    else if (is_oct (chaine))
	return OCT;
    else if (is_dec (chaine))
	return DEC;	
    return UNKNOWN;
}




int is_hexa(char* chaine) {
    int i;
    long l;
    char* p;
    if (chaine!=NULL && strlen(chaine)>2&& chaine[0]=='0' && chaine[1]=='x' && sscanf(chaine,"%x",&i)==1){
	 l = strtol (chaine, &p , 16);
	 if (l!=0) return 1;
	 else return 0;
}
    else return 0;
}

int is_oct(char* chaine) {
    int i;
    long l;
    char* p;
    if (chaine!=NULL && strlen(chaine)> 1 && chaine[0]=='0' && sscanf(chaine,"%x",&i)==1){
	 l = strtol (chaine, &p , 8);
	 if (l!=0) return 1;
	 else return 0;
}
    else return 0;
}

int is_dec (char* chaine){
    int i;
    long l;
    char* p;
    if (chaine!=NULL && sscanf(chaine,"%x",&i)==1){
	 l = strtol (chaine, &p , 10);
	 if (l!=0) return 1;
	 else return 0;
}
    else return 0;
}   




char* get_next_token(interpreteur inter) {

    char       *token = NULL;
    char       *delim = " \t\n";

    if ( inter->first_token == 0 ) {
        token = strtok_r( inter->input, delim, &(inter->from) );
        inter->first_token = 1;
    }
    else {
        token = strtok_r( NULL, delim, &(inter->from) );
    }

    if ( NULL == token ) {
        inter->first_token = 0;
    }

    return token;
}






int execute_cmd(interpreteur inter) {
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

    /*on identifie la commande avec un premier appel Ã  get_next_token*/
    char * token = get_next_token(inter);

    if(strcmp(token, "exit") == 0) {
        return exitcmd(inter);
    }
    else if(strcmp(token, "load") == 0) {
        return loadcmd(inter);
    }
    else if(strcmp(token, "disp") == 0) {
	return dispcmd(inter);
    }
    else if(strcmp(token, "disasm") == 0) {
	return disasmcmd(inter);
    }
    else if(strcmp(token, "set") == 0) {
	return setcmd(inter);
    }

    WARNING_MSG("Unknown Command : '%s'\n", cmdStr);
    return CMD_UNKOWN_RETURN_VALUE;
}

















int  acquire_line(FILE *fp, interpreteur inter) {
    char* chunk =NULL;

    memset(inter->input, '\0', MAX_STR );
    inter->first_token =0;
    if (inter->mode==SCRIPT) {
        // mode fichier
        // acquisition d'une ligne dans le fichier
        chunk =calloc(MAX_STR, sizeof(*chunk));
        char * ret = fgets(chunk, MAX_STR, fp );
        if(ret == NULL) {
            free(chunk);
            return 1;
        }
        // si windows remplace le \r du '\r\n' (fin de ligne windows) par \0
        if(strlen(ret) >1 && ret[strlen(ret) -2] == '\r') {
            ret[strlen(ret)-2] = '\0';
        }
        // si unix remplace le \n par \0
        else if(strlen(ret) >0 && ret[strlen(ret) -1] == '\n') {
            ret[strlen(ret)-1] = '\0';
        }

    }
    else {
        /* mode shell interactif */
        /* on utilise la librarie libreadline pour disposer d'un historique */
        chunk = readline( PROMPT_STRING );
        if (chunk == NULL || strlen(chunk) == 0) {
            /* commande vide... */
            if (chunk != NULL) free(chunk);
            return 1;
        }
        /* ajout de la commande a l'historique, librairie readline */
        add_history( chunk );

    }
    // standardisation de la ligne d'entrÃ©e (on met des espaces lÃ  ou il faut)
    string_standardise(chunk, inter->input);

    free( chunk ); // liberation de la mÃ©moire allouÃ©e par la fonction readline() ou par calloc()

    DEBUG_MSG("Ligne acquise '%s'\n", inter->input); /* macro DEBUG_MSG : uniquement si compil en mode DEBUG_MSG */
    return 0;
}




int exitcmd(interpreteur inter) {
    INFO_MSG("Bye bye !");
    return CMD_EXIT_RETURN_VALUE;
}



