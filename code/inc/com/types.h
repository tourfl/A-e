
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

 // Fonction qui permet de faire une allocation dynamique plus simplement

 void *init_m(void *addr, int size);
	
typedef uint32_t uint, word;
typedef uint8_t  byte;

typedef uint32_t vaddr32;
typedef uint64_t vaddr64;

typedef union {
    vaddr32 _32;
    vaddr64 _64;
} vaddr, addr;


#define STRLEN 256

#define NB_CHIFFRES_MAX_32o 11
#define NB_CHIFFRES_MAX_32d 10
#define NB_CHIFFRES_MAX_32h 8


typedef char *Str;

typedef char string[STRLEN+1];

#define init_string(s) memset( s, '\0', STRLEN+1 )

typedef struct {
	unsigned int start;
	unsigned int end;
	int value;

} Plage;

void init_plage(Plage *p);

void disp_plg(Plage p);

 typedef struct
 {
 	int size;
 	Plage *plages;

 } Plgtab;

 void init_plgtab(Plgtab *t);
 void del_plgtab(Plgtab *t);

 void disp_plgtab(Plgtab t);

 typedef struct
 {
 	int size;
 	char **content;

 } Strlist;

 void init_strlist(Strlist *l);
 void del_strlist(Strlist *l);

 void disp_strlist(Strlist l);

 int prepend_2slash(char **str); // ajoute "2/" au début de la chaîne


typedef enum { UNDEF, LSB, MSB } endianness;

/* Fonctions */

int get_type(char* chaine);
int is_addr(char* chaine);
int is_hexa(char* chaine);
int is_oct(char* chaine);
int is_dec (char* chaine);
int is_figure(char* chaine);

#ifdef __cplusplus
}
#endif

#endif /* _TYPES_H_ */
