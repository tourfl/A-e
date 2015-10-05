
#include "common/interpreteur.h"

int loadcmd(interpreteur inter) {

	char * token=NULL;
    FILE *fo = NULL;
    char caractereActuel = 0;
	
	token = get_next_token(inter);

    // le token suivant est normalement le nom du fichier, on essaye donc de l'ouvrir

    if (token == NULL || get_type(token) == HEXA)
    {
        WARNING_MSG("please specifiy a file");
        return 1;
    }

    fo = fopen(token, "rb" );
    if ( fo == NULL ) 
    {
        perror( "fopen" );
        WARNING_MSG("file doesn't exist");
        return 1;
    }

    do
        {
            caractereActuel = fgetc(fo); // On lit le caractère
            printf("%c", caractereActuel); // On l'affiche
        } while (caractereActuel != EOF); // On continue tant que fgetc n'a pas retourné EOF (fin de fichier)

    token = get_next_token(inter);

	switch(get_type(token)) {
        case HEXA:
            //sscanf(token,"%x", &hexValue);
	   /* charger le fichier Ã  l'adresse saisi */ 	//?????????? _loadcmd
            break;
        default :
            WARNING_MSG("value %s is not a valid argument of load command\n",token);
            return 1;
        }

    fclose(fo);
}