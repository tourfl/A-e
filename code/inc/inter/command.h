#ifndef COMMAND_H
#define COMMAND_H

#include "emulator.h"
// Macro

#define INTO -1

 /* Commandes */

// Ces fonctions font principalement de l'analyse syntaxique

int exitcmd();


int execute_cmd(Emulator *emul);

int loadcmd(Emulator *emul);
int dispcmd (Emulator *emul);
int setcmd (Emulator *emul);
int assert(Emulator *emul);
int disasm(Emulator *emul);
int run_cmd(Emulator *emul);
int step_cmd(Emulator *emul);



// Chargement fichier

int load_elf_in_mem(FILE *fo, Segment map[NB_SEC], unsigned int va);


// Affichage

void print_section_raw_content(char* name, unsigned int start, byte* content, unsigned int taille);


void disp_map(Segment map[NB_SEC]);
void disp_plage(Plage plg, Segment map[NB_SEC]); //Pour la fonction discmd
void disp_oct(unsigned int va, Segment map[NB_SEC]);

int disp_some_reg(char *token, Emulator *emul);

void disp_all_regs(Registre reg[NB_REG]);
int disp_reg(char *name, Registre reg[NB_REG]);
// Set

//int set_reg (char* name, char* content, Registre reg[NB_REG]); //Pour la fonction setcmd
int set_mem (char* type,char* adresse, char* content, Emulator *emul);	//Pour la fonction setcmd;

// Assert

int assert_reg(char *name, unsigned int val, Registre reg[NB_REG]); // OK
int assert_wrd(vaddr32 va_1, word val, Segment map[NB_SEC]);
int assert_bte(vaddr32 va, unsigned int val, Segment map[NB_SEC]);

// Disasm

int disasm_plage(Plage p, Emulator *emul);

// Step

int step(int flag, Emulator *emul);


#endif
