#ifndef  REGISTRE_H
#define REGISTRE_H value

#include "types.h" // word

#define NB_REG 17
/*
 * de r0 à r15 + apsr (reg[16])
 * r13 : sp 	r14 : lr 	r15 : pc
 */



// un registre est un unsigned int
typedef word Registre;






int get_reg(char *name, word *content, Registre reg[NB_REG]); // Il faut une valeur de retour pour savoir si name appartient bien aux registres
int set_reg(char *name, word value, Registre reg[NB_REG]);

#endif