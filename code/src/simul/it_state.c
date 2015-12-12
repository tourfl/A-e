
#include "simul/it_state.h"
#include <string.h> // strcpy
#include "inter/notify.h" // printf
#include "types.h" // int_to_bin
#include "elf/bits.h" // GET_BITS




/*
	- renvoie le mnémonique dans state ;
	- met à jour it_state (!) ;
*/



void set_it_state(int *it_state, int mask, int sig, int thn, int els)
{
	// it_state = mask   sign   then   else

	*it_state = mask << 12;
	*it_state += sig <<  8;
	*it_state += thn <<  4;
	*it_state += els;
}





void reset_it_state(int *it_state)
{
	*it_state = 0; // tout simplement !
}




// get and update state

void process_state(int *it_state, char name_in[], char states_tab[15][3])
{
	int ms = GET_BITS(*it_state, 8, 15) >> 8; // partie contenant masque et signature
	int te = GET_BITS(*it_state, 0, 7); // partie contenant les indexes des mnémos
	int st = ( (ms >> 4) & ms ) & 1; // 1 : Then ; 0 : Else
	int cond = GET_BITS(te, (st * 4), 3 + (st * 4)) >> (st * 4);


	// printf("\nit_state = 0b%s\n", int_to_bin(*it_state, 16));
	// printf("cond = %u\n", cond);

	name_in = strcat(name_in, states_tab[cond]);

	update_it_state(it_state);
}




void update_it_state(int *it_state)
{
	int ms = GET_BITS(*it_state, 8, 15) >> 8; // partie contenant masque et signature
	int te = GET_BITS(*it_state, 0, 7); // partie contenant les mnémos de then et else

	// printf("te = %s\n", int_to_bin(te, 8));
	// printf("ms = %s\n", int_to_bin(ms, 8));

	ms >>=  1; // cela met à jour !

	*it_state = (ms << 8) + te;
}




int InITBlock (int it_state) {
	// Est-on dans un bloc IT?

	return (it_state >> 12) & 1;

}

int LastInITBlock (int it_state) {
	// Est-ce le dernier élément d'un bloc IT?

	if( (it_state >> 12) == 1)
		return 1;

	else
		return 0;
}
