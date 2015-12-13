
#include "simul/breakpoints.h"
#include <stdlib.h> // free
#include "inter/notify.h" // printf




/*
	breaklist pointe sur le DERNIER élément de la pile
*/




void del_breaklist(Breakpoint *breaklist)
{
	if(breaklist == NULL)
		return;

	while ( (breaklist = breaklist->prev) != NULL)

	while ( (breaklist = breaklist->next) != NULL)
		free(breaklist->prev);

	free(breaklist);
}

void disp_breaklist( Breakpoint *breaklist )
{
	int i=0;




	if(breaklist == NULL)
	{
		printf("No breakpoint\n");
		return;
	}

	do
	{
		printf("#%u\t0x%08x\n", i, breaklist->va);
		i++;
	} while ( (breaklist = breaklist->prev) != NULL);
}



int breakpoint_exist( Breakpoint *breaklist, vaddr32 va )
{
	if(breaklist == NULL)
		return 0;


	do
	{
		if(breaklist->va == va)
			return 1;
	} while ( (breaklist = breaklist->prev) != NULL);

	return 0;
}


static Breakpoint * init_breakpoint( vaddr32 va ) // static pour ne pas que cette fonction soit utilisée autre part que dans ce fichier (encapsulation :) )
{
	Breakpoint *bp = calloc(1, sizeof(Breakpoint));



	bp->va = va;

	return bp;
}




void add_breakpoint( Breakpoint **p_breaklist, vaddr32 va )
{
	Breakpoint *current_bp = *p_breaklist; // le dernier élément au départ

	


	if(breakpoint_exist(*p_breaklist, va))
		return;

	// on ne gère pas le cas où le breakpoint est hors de la section text, ce bp n'aura juste pas d'impactes



	Breakpoint *bp = init_breakpoint(va);

	if (current_bp != NULL)
	{
		current_bp->next = bp;
		bp->prev = current_bp;
	}

	*p_breaklist = bp;
}



void del_breakpoint( Breakpoint **p_breaklist, vaddr32 va )
{
	Breakpoint *current_bp = *p_breaklist;




	if(current_bp == NULL)
		return;


	do
	{
		if(current_bp->va == va)
		{
			if(current_bp->next == NULL) // si c'est le dernier élément
			{
				if(current_bp->prev == NULL) // et que c'est en plus le seul élément
				{
					free(current_bp);
					*p_breaklist = NULL;
					return;
				}

				here;
				current_bp->prev->next = NULL;
				*p_breaklist = current_bp->prev;
				free(current_bp);

			}
			else if(current_bp->prev == NULL) // si c'est le premier élément
			{
				current_bp->next->prev = NULL;
				free(current_bp);
				return;
			}
			else
			{
				here;
				current_bp->prev->next = current_bp->next;
				current_bp->next->prev = current_bp->prev;
				free(current_bp);
			}


			
		}
	} while ( (current_bp = current_bp->prev) != NULL);
}
