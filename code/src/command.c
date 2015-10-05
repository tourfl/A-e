
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