/* Fonctions relatives aux types */

#include "types.h"

#include <string.h> // Pour strtok, strcat notamment
#include "inter/notify.h" // messages de contrôle
#include <stdlib.h> // sscanf notamment
#include <ctype.h> // pour isdigit
#include <math.h> // pow
#include "elf/bits.h" // mask_from_0






// On pourrait écrire init_c avec calloc

void *init_m(void *addr, int size)
{
    addr = malloc(size * sizeof(*addr));

    return addr;
}




enum {HEXA,OCT,DEC,UNKNOWN};

int get_type(char* chaine) {

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
    printf("\t%u->%u = %u", p.start, p.end, p.value);
}




// ex : p.start = 2, p.end = 6 retourne 0000 0000 0111 1100 = 124

word plg_to_mask(Plage p)
{
    word m = 0;



    SET_BITS(m, p.start, p.end);

    return m;
}





Plgtab* init_plgtab()
{
    return calloc(1, sizeof(Plgtab));
}






int set_plgtab(Plgtab *plgt, int size)
{
    plgt->size = size;
    

    if ( ( plgt->plages = calloc(size, sizeof(Plage)) ) == NULL)
        return 2;

    return 0;
}






 void del_plgtab(Plgtab *t) {

    if(t == NULL)
        return;

    free(t->plages);
    free(t);
 }







Plgtab* plgtabclone(Plgtab *src)
{
    Plgtab *dest = init_plgtab();
    int i;





    if(dest == NULL)
        return NULL;

    dest->size = src->size;
    dest->plages = calloc(dest->size, sizeof(Plage));

    if(dest->plages == NULL)
    {
        free(dest);
        return NULL;
    }

    for(i = 0; i < dest->size; i++)
    {
        dest->plages[i] = src->plages[i];
    }

    return dest;
}




void plgtabdup(Plgtab *dest, Plgtab *src)
{
    int i;


    dest->size = src->size;

    if ( ( dest->plages = calloc(dest->size, sizeof(Plage)) ) == NULL )
        return;

    for(i = 0; i < dest->size; i++)
    {
        dest->plages[i] = src->plages[i];
    }
}




 void disp_plgtab(Plgtab t) {
    int i;


    printf("(size=%u)\n", t.size);

    for (i = 0; i < t.size; ++i)
    {
        printf("  plage %u:\t", i);
        disp_plg(t.plages[i]);
        printf("\n");
    }
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






int to_plgtab(char *chaine, Plgtab *plgt)
{
    char *token, *str = NULL, *saveptr = NULL;
    int i, s = 0; // au moins un token, celui de la taille

    // 2 niveaux de parsing

    if(plgt == NULL)
        return 31;


    for (i = -1, str = chaine; i < s; ++i, str = NULL)
    {
        token = strtok_r(str, "/", &saveptr);


        // DISP_TOKEN(i, token);


        if(token == NULL) // Comprend le cas chaine == NULL
            break;

        if(i == -1)
        {
            sscanf(token, "%u", &s);
            set_plgtab(plgt, s);
        }

        else if(sscanf(token, "%u:%u", &plgt->plages[i].start, &plgt->plages[i].end) == 1)
        {
            plgt->plages[i].end = plgt->plages[i].start;
        }

        
    }



    return 0;
}






int is_addr(char* chaine) 
{

    if(strlen(chaine) < 3 || *chaine != '0' || *(chaine +1) != 'x')
        return 1;

    return is_hexa(chaine + 2);
}





int is_hexa(char* chaine) {

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


