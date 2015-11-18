/* Fonctions relatives aux types */

#include "com/types.h"
#include <string.h> // Pour strtok, strcat notamment
#include "com/notify.h" // messages de contrôle
#include <stdlib.h> // sscanf notamment
#include <ctype.h> // pour isdigit
#include <CUnit/CUnit.h>





// On pourrait écrire init_c avec calloc

void *init_m(void *addr, int size)
{
    addr = malloc(size * sizeof(*addr));

    return addr;
}





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




void init_plage(Plage *p)
{
    p->start = 0;
    p->end = 0;
}





void disp_plg(Plage p) {
    printf("start: %u\tend: %u\n", p.start, p.end);
}





 void init_plgtab(Plgtab *t)
 {
    t->size = 0;
    t->plages = NULL;
 }






 void del_plgtab(Plgtab *t) {
    free(t->plages);
 }






 void disp_plgtab(Plgtab t) {
    int i;


    printf("(size=%u)\n", t.size);

    for (i = 0; i < t.size; ++i)
    {
        printf("  plage %u:\t", i);
        disp_plg(t.plages[i]);
    }
 }





void init_strlist(Strlist *l)
{
    l->size = 0;
    l->content = NULL;
}








void del_strlist(Strlist *l)
{
    int i;

    // printf("\nsize : %u", l->size);

    if(l->content == NULL)
        return;

    for (i = 0; i < l->size; ++i)
    {
        // printf("\n%s", l->content[i]);
        free(l->content[i]); // on libère ce qu'il y a dans les cases du tableau
        l->content[i] = NULL;
    }

    free(l->content); // on libère le tableau
    l->size = 0;
}






 void disp_strlist(Strlist l)
 {
    int i;


    for (i = 0; i < l.size; ++i)
    {
        printf("\nl.content[%u] : %s", i, l.content[i]);
    }
 }


 int prepend_2slash(char **str)
 {

    char *new_chaine =  NULL;





    if(str == NULL)
        return 1;

    new_chaine = calloc((strlen(*str) + 2), sizeof(char));

    if(new_chaine == NULL)
        return 2;

    new_chaine[0] = '2';
    new_chaine[1] = '/';

    if(strcat(new_chaine, *str) == NULL) {
        return 6;
    }

    *str = new_chaine;

    return 0;
 }






int is_addr(char* chaine) 
{
    DEBUG_MSG("is_addr entered");

    if(strlen(chaine) < 3 || *chaine != '0' || *(chaine +1) != 'x')
        return 1;

    return is_hexa(chaine + 2);
}





int is_hexa(char* chaine) {
    DEBUG_MSG("is_hexa entered");

    int i;

    // V3

    for(i = 0; i < NB_CHIFFRES_MAX_32h && *chaine != '\0'; i++) // Tant que le caractère n'est pas le caractère de fin
    {
        if(!isxdigit((int) *chaine)) // Si ce n'est pas un chiffre en base 16
            return 1;

        chaine ++; // On passe au caractère suivant
    }

    return 0;

}

int is_oct(char* chaine) {
    DEBUG_MSG("is_oct entered");

    int i;

    if(strlen(chaine) < 2 || *chaine != '0')
        return 1;

    chaine ++;

    for(i = 0; i < NB_CHIFFRES_MAX_32o && *chaine != '\0'; i++) // Tant que le caractère n'est pas le caractère de fin
    {
        if(!isdigit((int) *chaine) || *chaine == '8' || *chaine == '9') // Si ce n'est pas un chiffre en base 8
            return 1;

        chaine ++; // On passe au caractère suivant
    }

    return 0;
}

int is_dec(char* chaine){
    DEBUG_MSG("is_dec entered");
    // V3

    int i;

    for(i = 0; i < NB_CHIFFRES_MAX_32d && *chaine != '\0'; i++) // Tant que le caractère n'est pas le caractère de fin
    {
        if(!isdigit((int) *chaine)) // Si ce n'est pas un chiffre en base 10
            return 1;

        chaine ++; // On passe au caractère suivant
    }

    return 0;
}

void cu_is_dec(void)
{
    CU_ASSERT(is_dec("12") == 0);
}







int is_figure (char *chaine){

    if(is_addr(chaine) == 0
    || is_hexa(chaine) == 0
    || is_oct(chaine) == 0
    || is_dec(chaine) == 0)
        return 0;

    return 1;
}


