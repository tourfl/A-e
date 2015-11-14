#ifndef COMMAND_H
#define COMMAND_H

#include "com/interpreteur.h"
#include "mem/memory_v2.h" // pour Memory et Registre
#include "com/dic.h"

// Macro

#define INTO -1

 /* Commandes */

// Ces fonctions font principalement de l'analyse syntaxique

int exitcmd();

int loadcmd(interpreteur inter, Memory *mem);
int dispcmd (interpreteur inter, Memory *mem);
int setcmd (interpreteur inter, Memory *mem);
int execute_cmd(interpreteur inter, Memory *mem, Dic *dic);
int assert(interpreteur inter, Memory *mem);
int disasm(interpreteur inter, Memory *mem, Dic *dic);
int run_cmd(interpreteur inter, Memory *mem/* autres paramètres à ajouter */);
int step_cmd(interpreteur inter, Memory *mem/* dic ?*/);

//int disasmcmd(interpreteur inter);

// Affichage

void print_section_raw_content(char* name, unsigned int start, byte* content, unsigned int taille);

int disp_some_mem(char *token, interpreteur inter, Memory *mem);

void disp_map(Segment map[NB_SEC]);
void disp_plage(unsigned int va, unsigned int va_2, Memory *mem); //Pour la fonction discmd
void disp_oct(unsigned int va, Memory *mem);

int disp_some_reg(char *token, interpreteur inter, Registre reg[NB_REG]);

void disp_all_regs(Registre reg[NB_REG]);
int disp_reg(char *name, Registre reg[NB_REG]);
// Set

//int set_reg (char* name, char* content, Registre reg[NB_REG]); //Pour la fonction setcmd
int set_mem (interpreteur inter,char* type,char* adresse, char* content, Segment map[]);	//Pour la fonction setcmd;

// Assert

int assert_reg(char *name, unsigned int val, Registre reg[NB_REG]); // OK
int assert_wrd(vaddr32 va_1, word val, Memory *mem);
int assert_bte(vaddr32 va, unsigned int val, Memory *mem);

// Disasm

int disasm_plage(vaddr32 va_1, vaddr32 va_2, Memory *mem, Dic *dic);
int disasm_ins(word mot, Instruction *ins);


#endif
