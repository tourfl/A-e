
#ifndef MEMORY_H
#define MEMORY_H

typedef struct mem_seg
{
	long va; // Toutes les adresses virtuelles seront précédées d'un va_
	int size;
	char *content;


} Mem_seg;

// Mem_map est un tableau de Mem_seg

typedef struct *mem_seg Mem_map;

int add_first_segment(char *name, long va, int size);

int add_segment(char* name, int size);

#endif