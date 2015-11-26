
/**
 * @file types.h
 * @author François Cayre <francois.cayre@grenoble-inp.fr>
 * @date Thu Oct 31 19:09:31 2013
 * @brief Types.h
 *
 * Types.
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h> // Pour memset ?


#define STRLEN 256

#define NB_CHIFFRES_MAX_32o 11
#define NB_CHIFFRES_MAX_32d 10
#define NB_CHIFFRES_MAX_32h 8









	
typedef uint32_t uint, word;
typedef uint8_t  byte;
typedef uint16_t Half_word;

typedef uint32_t vaddr32;
typedef uint64_t vaddr64;



typedef enum { UNDEF, LSB, MSB } endianness;





typedef union {
    vaddr32 _32;
    vaddr64 _64;
} vaddr, addr;





typedef struct {
	unsigned int start;
	unsigned int end;
	int value;

} Plage;



 typedef struct
 {
 	int size;
 	Plage *plages;

 } Plgtab;




 typedef struct
 {
 	int size;
 	char **content;

 } Strlist;

typedef char *Str;

typedef char string[STRLEN+1];











 // Fonction qui permet de faire une allocation dynamique plus simplement

 void *init_m(void *addr, int size);

#define init_string(s) memset( s, '\0', STRLEN+1 )




Plage* init_plage();

void disp_plg(Plage p);







 Plgtab* init_plgtab();
 void del_plgtab(Plgtab *t);

 Plgtab* plgtabclone(Plgtab *src);

 void disp_plgtab(Plgtab t);



char* strclone(char *src);

 Strlist* init_strlist();
 void del_strlist(Strlist *l);

 void disp_strlist(Strlist l);

 int prepend_2slash(char **str); // ajoute "2/" au début de la chaîne


 int to_strtab(char *chaine, char **tab);
 int to_wrdtab(char *chaine, word tab[]);

 int to_strlist(char *chaine, Strlist *liste); // fonction ultra sécurisée !

int to_plgtab(char *chaine, Plgtab *plgt);

/* Fonctions */

int get_type(char* chaine);
int is_addr(char* chaine);
int is_hexa(char* chaine);
int is_oct(char* chaine);
int is_dec (char* chaine);
int is_figure(char* chaine);



char* int_to_bin(unsigned int n, char taille); // peut servir pour faire des tests

#ifdef __cplusplus
}
#endif

#endif /* _TYPES_H_ */