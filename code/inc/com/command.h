#ifndef COMMAND_H
#define COMMAND_H

#include "com/interpreteur.h"
#include "mem/memory_v2.h" // pour Memory et Registre

 /* Commandes */

int exitcmd(interpreteur inter);

int loadcmd(interpreteur inter, Memory *mem);
int dispcmd (interpreteur inter, Memory *mem);
int setcmd (interpreteur inter, Memory *mem);
int execute_cmd(interpreteur inter, Memory *mem);
//int disasmcmd(interpreteur inter);

// Affichage

void disp_map(Memory *memory);
void disp_plage(unsigned int va_1, unsigned int va_2, Memory *mem); //Pour la fonction discmd
void disp_oct(unsigned int va, Memory *mem);

void disp_all_regs(Registre reg[NB_REG]);
void disp_reg(char *name, Registre reg[NB_REG]);
// Set

int set_reg (char* name, char* content, Registre reg[NB_REG]); //Pour la fonction setcmd
int set_mem (interpreteur inter,char* type,char* adresse, char* content, Segment map[]);	//Pour la fonction setcmd;


#endif
