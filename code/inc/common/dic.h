
#ifndef  DIC_H
#define DIC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TAILLE_MAX 1000

int disp_dic();

int comp_instr_16(unsigned int inst, char ligne[TAILLE_MAX]); // parsing ok

int comp_instr_32(unsigned long inst, char ligne[TAILLE_MAX]);

char* int_to_bin(unsigned int n, char taille); // retourne un tableau de char[taille]

char* bin_x_bin(char bin_1[], char bin_2[], int taille);

#endif
