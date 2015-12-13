
#ifndef BREAKPOINTS_H
#define BREAKPOINTS_H

#include "types.h" // vaddr32


// unused

enum bp_state {  
	ADDRESS_OUT_OF_TEXT, BP_ALREADY_EXIST
};


struct breakpoint; // dans le doute pour pouvoir déclarer à l'intérieur de la structure

struct breakpoint {

	struct breakpoint *prev;
	struct breakpoint *next;

	vaddr32 va;
};

typedef struct breakpoint Breakpoint;




void del_breaklist(Breakpoint *breaklist);

void disp_breaklist( Breakpoint *breaklist );
int breakpoint_exist( Breakpoint *breaklist, vaddr32 va );

void add_breakpoint( Breakpoint **breaklist, vaddr32 va );
void del_breakpoint( Breakpoint **breaklist, vaddr32 va );


#endif