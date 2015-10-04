
#ifndef MEMORY_H
#define MEMORY_H

/*
 * Structures
 */

typedef struct mem_seg
{
	char *name;
	long va; // Toutes les adresses virtuelles seront précédées d'un va_
	//int size;
	//char *content;


} Mem_seg;

// Mem_map est un tableau d'adresse de Mem_seg pour que l'accès soit rapide

typedef struct **mem_seg Mem_map;

/*
 * Fonctions
 */

int init_mem_map(char mem_size);

/* Fonction qui crée le 1er segment, soit .rodata, soit .text */
int add_first_segment(char *name, long va, int size, Mem_map *map);


/* Fonction qui crée un segment, à partir de l'adresse va */
int add_segment(char* name, long *va, Mem_map *map);

/* Fonction qui met à jour l'adresse virtuelle disponible à partir de la taille et de l'adresse du segment précédent */
int update_va(long *va, int size);

#endif