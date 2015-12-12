
#include "simul/it_state.h"
#include <string.h> // strcpy




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






void get_state(int *it_state, char state[3], char **str_cond)
{
	int ms = *it_state >> 8; // partie contenant masque et signature
	int te = (*it_state << 8) >> 8; // partie contenant les indexes des mnémos
	int st = ( (ms >> 8) & ms ) & 1; // 1 : Then ; 0 : Else
	int cond = te >> (st * 4);




	strcpy(state, str_cond[cond]);

	update_it_state(it_state);
}



void update_it_state(int *it_state)
{
	int ms = *it_state >> 8; // partie contenant masque et signature
	int te = (*it_state << 8) >> 8; // partie contenant les mnémos de then et else

	ms = ms >> 1; // cela met à jour !

	*it_state = (ms << 8) + te;
}




int InITBlock (int it_state) {
	// Est-on dans un bloc IT?

	return it_state >> 12;

}

int LastInITBlock (int it_state) {
	// Est-ce le dernier élément d'un bloc IT?

	if( (it_state >> 12) == 1)
		return 1;

	else
		return 0;
}
