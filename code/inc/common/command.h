#ifndef COMMAND_H
#define COMMAND_H

#include <string.h>
#include <math.h>

#include "common/interpreteur.h"
#include "mem/memory_v2.h"
#include "common/notify.h"
#include "common/types.h"

 /* Commandes */

int loadcmd(interpreteur inter, Memory *mem);
int dispcmd (interpreteur inter, Memory *mem);
int setcmd (interpreteur inter, Memory *mem);
int execute_cmd(interpreteur inter, Memory *mem);
//int disasmcmd(interpreteur inter);

int exitcmd(interpreteur inter);

Registre * which_reg (char *nom, Registres *reg);
//int is_reg (char* nom);
void disp_map(Memory *memory);
void disp_plage (unsigned long va_1, unsigned long va_2, Memory *mem); //Pour la fonction discmd
void disp_oct(unsigned long va, Memory *mem);

void disp_all_regs(Registres *mem);
void disp_reg(char *name, Registres *reg);
void display_reg_content(Registre reg);

int set_reg (interpreteur inter, char* r_name, char* r_content, Registres *reg); //Pour la fonction setcmd
int set_mem (interpreteur inter,char* type,char* adresse, char* content, Segment map[]);	//Pour la fonction setcmd;

#endif
