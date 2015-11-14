#ifndef  INTERPRETEUR_H
#define INTERPRETEUR_H

#include <stdio.h>
#include "com/types.h" // uint

/* prompt du mode shell interactif */
#define PROMPT_STRING "ARMShell> "

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
#define CMD_DEBUG_RETURN_VALUE -3
#define CMD_RESUME_RETURN_VALUE -4


/* type de token (exemple) */
enum {HEXA,UNKNOWN};

/* mode d'interaction avec l'interpreteur (exemple)*/
typedef enum {INTERACTIF,SCRIPT,DEBUG_MODE} inter_mode;

/* structure passée en parametre qui contient la connaissance de l'état de
 * l'interpréteur
 */
typedef struct {
    inter_mode mode;
    char input[MAX_STR];
    char * from;
    char first_token;
} *interpreteur;

/*
 *	Fonctions
 */

 // Fournies

 interpreteur init_inter(void);
 void del_inter(interpreteur inter);

 char* get_next_token(interpreteur inter);
 int get_next_if_hexa(interpreteur inter, uint *out);


 void string_standardise( char* in, char* out );
 int  acquire_line(FILE *fp, interpreteur inter);
 void usage_ERROR_MSG( char *command );

 // Créées

 int get_type(char* chaine);

#endif
