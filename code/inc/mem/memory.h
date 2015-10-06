
#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

#include "elf/elf.h"
#include "common/notify.h"


/*
 * Structures
 */

typedef struct
{
	char *name;
	char *va; // Toutes les adresses virtuelles seront précédées d'un va_ et seront des chaînes de char
	int size;
	unsigned char *content;
} Segment;

/*
 *	Convention dans le programme pour les INSTANCES de Registres et Registre :
 *
 *	reg : ensemble des registres
 *
 *	r : UN registre
 *
 */

typedef struct {
	char *name;
	//char *va; // Un registre a-t-il une adresse virtuelle ?
	char* content;
} Registre;

typedef struct {
	Registre r[15];
	Registre *sp;
	Registre *lr;
	Registre *pc;
	Registre *apsr;
} Registres;

// Map est un tableau d'adresses de Segment pour que l'accès soit rapide

typedef Segment *Map;

typedef struct {
	Registres *reg;
	Map *map;
	int nb_seg;
} Memory;


/*
 * Fonctions
 */

Memory *init_mem();
Segment *init_seg();
Segment *init_seg_with(char *name, char *content);

int load_elf_in_mem(FILE *fo, char *va, Map *map);

/* Fonction qui crée le 1er segment, soit .rodata, soit .text */
int add_first_segment(char *name, char *va, int size, Map *map, char *content);


/* Fonction qui crée un segment, à partir de l'adresse va */
int add_segment(char* name, char map_cursor, char *va, int size, Map *map, char *content);

/* Fonction qui met à jour l'adresse virtuelle disponible à partir de la taille (en héxa) et de l'adresse du segment précédent */
int update_va(char **va, int size);

// Fonction qui affiche le map mémoire
int display_map();

#endif
