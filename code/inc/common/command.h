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
int set_mem (interpreteur inter, char* token, char* adresse);
int setcmd (interpreteur inter);
int set_reg (interpreteur inter, char* reg, char* valeur);
int set_mem (interpreteur inter,char* type,char* adresse,char* valeur);
