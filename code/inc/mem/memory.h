
#ifndef MEMORY_H
#define MEMORY_H

typedef struct mem_seg
{
	long virtual_adress;
	int size;


} Mem_seg;

typedef struct mem_map
{
	Mem_seg *rodata;
	Mem_seg *text;
	Mem_seg *data;
	Mem_seg *bss;
	Mem_seg *lib;
	Mem_seg *stack;
} Mem_map;



#endif