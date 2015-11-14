
#include "com/command.h"

/*
* cf notify.c pour les erreurs
*
*/


int run_cmd(interpreteur inter, Memory *mem/* autres paramètres à ajouter */) {

	int r = 0;
	vaddr32 va = mem->reg[15]; // valeur courante du PC


	r = get_next_if_hexa(inter, &va);

	if (r ==  11) {
		return 11;
	}

	else if(r == 0 && get_next_token(inter) != NULL) {
		return 12;
	}

	//TODO

	return 0;

}
