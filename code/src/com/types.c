/* Fonctions relatives aux types */

#include "com/types.h"
#include <string.h> // Pour strtok notamment
#include "com/notify.h" // messages de contrôle
#include <stdlib.h> // sscanf notamment
#include <ctype.h> // pour isdigit


enum {HEXA,OCT,DEC,UNKNOWN};

int get_type(char* chaine) {

    DEBUG_MSG("get_type entered");

    if (is_hexa(chaine))
        return HEXA;
    else if (is_oct (chaine))
	return OCT;
    else if (is_dec (chaine))
	return DEC;	
    else return UNKNOWN;
}



int is_hexa(char* chaine) {
    DEBUG_MSG("is_hexa entered");

    // V3

    if(strlen(chaine) < 3 || *chaine != '0' || *(chaine +1) != 'x')
        return 1;

    chaine += 2; // Offset de 2 pour enlever 0x

    while(*chaine != '\0') // Tant que le caractère n'est pas le caractère de fin
    {
        if(!isxdigit((int) *chaine)) // Si ce n'est pas un chiffre en base 16
            return 1;

        chaine ++; // On passe au caractère suivant
    }

    return 0;

}

int is_oct(char* chaine) {
    DEBUG_MSG("is_oct entered");

    if(strlen(chaine) < 2 || *chaine != '0')
        return 1;

    chaine ++;

    while(*chaine != '\0') // Tant que le caractère n'est pas le caractère de fin
    {
        if(!isdigit((int) *chaine) || *chaine == '8' || *chaine == '9') // Si ce n'est pas un chiffre en base 8
            return 1;

        chaine ++; // On passe au caractère suivant
    }

    return 0;
}

int is_dec (char* chaine){ // Fixed!
    DEBUG_MSG("is_dec entered");

    // V3

    while(*chaine != '\0') // Tant que le caractère n'est pas le caractère de fin
    {
        if(!isdigit((int) *chaine)) // Si ce n'est pas un chiffre en base 10
            return 1;

        chaine ++; // On passe au caractère suivant
    }

    return 0;
}

int is_figure (char *chaine){

    if(is_hexa(chaine) == 0
    || is_oct(chaine) == 0
    || is_dec(chaine) == 0)
        return 0;

    return 1;
}


