<<<<<<< HEAD

#ifndef COMMAND_H
#define COMMAND_H

#include "common/interpreteur.h"
#include "mem/memory.h"
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
int disp_mem (char *va_1, char *va_2, Map *map); //Pour la fonction discmd
int set_reg (interpreteur inter, char* r_name, char* r_content, Registres *reg); //Pour la fonction setcmd
int set_mem (interpreteur inter,char* type,char* adresse, char* content, Map *map);	//Pour la fonction setcmd;

#endif
=======
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#include "common/interpreteur.h"
#include "mem/memory.h"
#include "common/command.h"
#include "common/notify.h"




int dispcmd (interpreteur inter);
registre wich_reg (char*nom);
int is_reg (char* nom);
int disp_mem (interpreteur inter, char* token, char* adresse);
int setcmd (interpreteur inter);
int set_reg (interpreteur inter, char* reg, char* valeur);
int set_mem (interpreteur inter,char* type,char* adresse,char* valeur);
>>>>>>> 912151e6d0c99c6c1f8590fbe6138aa2d77c14ac
