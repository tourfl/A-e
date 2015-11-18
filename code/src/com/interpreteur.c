/**
 * @file emulARM.c
 * @author François Cayre, Nicolas Castagné, François Portet
 * @brief Main pour le début du projet émulateur.
 *
 */

 #include "com/interpreteur.h"
 #include <string.h> // pour strtok notamment
 #include <stdlib.h> // Pour free
 #include "com/notify.h" // Pour les messages warning, debug, etc.
 #include <ctype.h> // Pour isblank()

/* la librairie readline */
#include <readline/readline.h>
#include <readline/history.h>

/**
 * allocation et init interpreteur
 * @return un pointeur vers une structure allouée dynamiquement
 */
interpreteur init_inter(void) {
    interpreteur inter = calloc(1,sizeof(*inter));
    if (inter ==NULL)
        ERROR_MSG("impossible d'allouer un nouvel interpreteur");
    return inter;
}




int reset_pos(interpreteur inter, int pos)
{
    int cur_pos = inter->pos;

    while(inter->pos != pos - 1) // sinon on dépasse la position
    {
        get_next_token(inter);

        if(inter->pos == cur_pos) // boucle infinie (!)
            return 13;
    }

    return 0;
}


char* get_next_token(interpreteur inter) {

    char *token = NULL, *str = NULL;
    char delim[] = " \t\n";

    if ( inter->pos == 0 ) {
        str = malloc(strlen(inter->input) * sizeof(char));

        if(str == NULL)
            return NULL;

        if(strcpy(str, inter->input) == NULL)
            return NULL;
    }

    token = strtok_r(str, delim, &(inter->from) );
    inter->pos++;

    if ( NULL == token ) {
        inter->pos = 0;
    }

    return token;
}




/*
* get_next_token + vérifie que c'est le dernier token
*
*/


 int get_last_token(interpreteur inter, char **out)
 {
    *out = get_next_token(inter);

    if(*out == NULL)
        return 1;

    else if(get_next_token(inter) != NULL)
        return 12;

    return 0;
 }


/*
*   out : paramètre de sortie
*   retourne 0 si cela a fonctionné
*
*/


int get_next_if_addr(interpreteur inter, uint *out) {

    char *token = get_next_token(inter);

    printf("%s\n", token);

    if(token == NULL)
        return 1;

    if(is_addr(token) != 0)
        return 11;

    *out = strtoul(token, NULL, 16);

    return 0;
 }


/*
*   out : paramètre de sortie, la valeur en hexa
*   retourne 0 si cela a fonctionné
*
*/


int get_next_if_hexa(interpreteur inter, uint *out) {

    char *token = get_next_token(inter);

    if(token == NULL)
        return 1;

    if(is_hexa(token) != 0)
        return 11;

    *out = strtoul(token, NULL, 16);

    return 0;
 }


/*
* get_next_if_addr + vérifie que c'est le dernier token
*
*/



int get_last_if_addr(interpreteur inter, uint *out)
{
    int r = 1;

    r = get_next_if_addr(inter, out);

    if(r == 1 || r == 11)
        return r;

    else if(get_next_token(inter) != NULL)
        return 12;

    return 0;
}


/*
* get_next_if_hexa + vérifie que c'est le dernier token
*
*/



int get_last_if_hexa(interpreteur inter, uint *out)
{
    int r = 1;

    r = get_next_if_hexa(inter, out);

    if(r == 1 || r == 11)
        return r;

    else if(get_next_token(inter) != NULL)
        return 12;

    return 0;
}




int get_next_if_fig(interpreteur inter, uint *out)
{
    char *token = get_next_token(inter);

    if(token == NULL)
        return 1;

    if(is_figure(token) != 0)
        return 11;

    *out = strtoul(token, NULL, 0);

    return 0;
}



/**
 * desallocation de l'interpreteur
 * @param inter le pointeur vers l'interpreteur à libérer
 */
void del_inter(interpreteur inter) {
    if (inter !=NULL)
        free(inter);
}







/**
 * @param in Input line (possibly very badly written).
 * @param out Line in a suitable form for further analysis.
 * @return nothing
 * @brief This function will prepare a line for further analysis.
 *
 * This function will prepare a line for further analysis and check for low-level syntax errors.
 * colon, brackets, commas are surrounded with blank; tabs are replaced with blanks.
 * negs '-' are attached to the following token (e.g., "toto -   56" -> "toto -56")  .
 */
void string_standardise( char* in, char* out ) {
    unsigned int i=0, j;

    for ( j= 0; i< strlen(in); i++ ) {

        /* insert blanks around special characters*/
        if (in[i]==':' || in[i]=='+' || in[i]=='~') {
            out[j++]=' ';
            out[j++]=in[i];
            out[j++]=' ';

        }

        /* remove blanks after negation*/
        else if (in[i]=='-') {
            out[j++]=' ';
            out[j++]=in[i];
            while (isblank((int) in[i+1])) i++;
        }

        /* insert one blank before comments */
        else if (in[i]=='#') {
            out[j++]=' ';
            out[j++]=in[i];
        }
        /* translate tabs into white spaces*/
        else if (isblank((int) in[i])) out[j++]=' ';
        else out[j++]=in[i];
    }
}







/**
 *
 * @brief extrait la prochaine ligne du flux fp.
 * Si fp ==stdin, utilise la librairie readline pour gestion d'historique de commande.
 *
 * @return 0 si succes.
 * @return un nombre non nul si aucune ligne lue
 */
int  acquire_line(FILE *fp, interpreteur inter) {
    char* chunk =NULL;

    memset(inter->input, '\0', MAX_STR );
    inter->pos =0;
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
    // standardisation de la ligne d'entrée (on met des espaces là ou il faut)
    string_standardise(chunk, inter->input);

    free( chunk ); // liberation de la mémoire allouée par la fonction readline() ou par calloc()

    DEBUG_MSG("Ligne acquise '%s'\n", inter->input); /* macro DEBUG_MSG : uniquement si compil en mode DEBUG_MSG */
    return 0;
}


/****************/
void usage_ERROR_MSG( char *command ) {
    fprintf( stderr, "Usage: %s [file.emul]\n   If no file is given, executes in Shell mode.", command );
}
