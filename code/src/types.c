/* Fonctions relatives aux types */

#include "types.h"

#include <string.h> // Pour strtok, strcat notamment
#include "inter/notify.h" // messages de contrôle
#include <stdlib.h> // sscanf notamment
#include <ctype.h> // pour isdigit
#include <math.h> // pow






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




Plage* init_plage()
{
    return calloc(1, sizeof(Plage));
}





void disp_plg(Plage p) {
    printf("start: %u\tend: %u\n", p.start, p.end);
}





Plgtab* init_plgtab()
{
    return calloc(1, sizeof(Plgtab));
}






 void del_plgtab(Plgtab *t) {
    free(t->plages);
    free(t);
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





Strlist* init_strlist()
{
    return calloc(1, sizeof(Strlist));
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





// reprise de to_strlist

int to_strtab(char *chaine, char **tab) {
    char *token = NULL, *str = NULL, *saveptr = NULL;
    int i;




    for (i = 0, str = chaine; i < 2; ++i, str = NULL)
    {
        token = strtok_r(str, "/", &saveptr);

        // DISP_TOKEN(i, token);

        if (token != NULL) {
            tab[i] = malloc(strlen(token) * sizeof(char));

            if(tab[i] == NULL)
                return 2;

            if(strcpy(tab[i], token) == NULL)
                return 5;
        }
        else {
            tab[i] = calloc(1, sizeof(char));

            if(tab[i] == NULL)
                return 2;
        }
    }

    return 0;
}






// Même principe que to_wrdtab

int to_wrdtab(char *chaine, word tab[]) {
    char *token = NULL, *str = NULL, *saveptr = NULL;
    int i;



    for (i = 0, str = chaine; i < 2; ++i, str = NULL)
    {
        token = strtok_r(str, "/", &saveptr);

        // DISP_TOKEN(i, token);

        if(token == NULL) // comprend le cas chaine == NULL
            tab[i] = 0; // 0 == '\0'

        else tab[i] = strtoul(token, NULL, 16);

        // printf("tab[%u] = %8x", i, tab[i]);
    }

    return 0;
}





int to_strlist(char *chaine, Strlist *l) // p doit être initialisée
{
    char *token = NULL, *str = NULL, *saveptr = NULL;
    int i;



    // V3



    // V2
    // Nécessite une fonction pour exploiter les valeurs de retour (non-écrite)
    for (i = 0, str = chaine; ; ++i, str = NULL)
    {
        token = strtok_r(str, "/", &saveptr);

        if(token == NULL) // comprend le cas chaine == NULL
            break;

        if(i == 0) {
            l->size = strtoul(token, NULL, 10);

            if(l->size >= 0) {
                l->content = malloc(l->size * sizeof(char *));

                if(l->content == NULL)
                    return 2;
            }

            else return 3;
        }
        else if (i <= l->size) {
            l->content[i-1] = calloc(strlen(token), sizeof(char));

            if(l->content[i-1] == NULL)
                return 2;

            if(strcpy(l->content[i-1], token) == NULL)
                return 5;
        }
    }

    // disp_strlist(*l);

    if(i != l->size + 1) // l->size
        return 4; // Problème dans le nombre de tokens


    return 0;
}

int to_plgtab(char *chaine, Plgtab *plgt)
{
    Strlist *strl = init_strlist();
    int i, r = 0;


    r = to_strlist(chaine, strl);

    // disp_strlist(strl);

    if(r != 0)
        return r;

    plgt->plages = malloc(strl->size * sizeof(Plage));

    if(plgt->plages == NULL)
        return 2;

    plgt->size = strl->size;

    for (i = 0; i < strl->size; ++i)
    {
        if(sscanf(strl->content[i], "%u:%u", &(plgt->plages[i].start), &(plgt->plages[i].end)) == 1)
            plgt->plages[i].end = plgt->plages[i].start;


        // printf("  \nplage %u:\tstart: %u\tend: %u\n", i, plgt->plages[i].start, plgt->plages[i].end);
    }

    del_strlist(strl);

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







int is_figure (char *chaine){

    if(is_addr(chaine) == 0
    || is_hexa(chaine) == 0
    || is_oct(chaine) == 0
    || is_dec(chaine) == 0)
        return 0;

    return 1;
}







char* int_to_bin(unsigned int n, char taille)
{
    int i, p;
    char *bin = NULL;
    
    bin = malloc((taille + 1) * sizeof(char));
    
    if(bin == NULL)
        return NULL;

    for(i = 0; i < taille; i++)
    {
        p = pow(2, taille-i-1);
    
        if(n/p == 1)
            *(bin + i) = '1';
        
        else if(n/p == 2)
            printf("attention taille");
            
        else
            *(bin + i) = '0';
            
        n = n % p;
    }
    
    *(bin + taille) = '\0';
    
    return bin;
}


