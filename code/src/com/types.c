/* Fonctions relatives aux types */

#include "com/types.h"
#include <string.h> // Pour strtok notamment
#include "com/notify.h" // messages de contrôle
#include <stdlib.h> // sscanf notamment


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
    if (chaine==NULL  
	|| strlen(chaine) <= 2 
        || chaine[0]!='0'
        || chaine[1]!='x'
	)
        return 1;

    return 0;
}

int is_oct(char* chaine) { // Ne fait pas bien son taf a priori
    DEBUG_MSG("is_oct entered");

    int i;
    long l;
    char* p;
    if (chaine!=NULL && strlen(chaine)> 1 && chaine[0]=='0' && sscanf(chaine,"%x",&i)==1){
	 l = strtol (chaine, &p , 8);
	 if (l!=0 && strcmp (p , "") == 0) return 0;
	 else return 1;
}
    else return 1;
}

int is_dec (char* chaine){ // Fixed!
    DEBUG_MSG("is_dec entered");

    int i;
    long l;
    char* p;
    if (chaine!=NULL && chaine[0] != '0' && sscanf(chaine,"%x",&i)==1){
	 l = strtol (chaine, &p , 10);
	 if (l!=0 && strcmp (p , "") == 0) return 0;
	 else return 1;
}
    else return 1;
}

int is_figure (char *chaine){

    if(is_hexa(chaine) == 0
    || is_dec(chaine) == 0
    || is_oct(chaine) == 0) // La valeur n'est pas correcte
        return 0;

    WARNING_MSG("Value must be decimal, hexadecimal or octal");
    return 2;
}


