
#include "com/command.h"
#include "com/notify.h" // messages de contrôle
#include <string.h> // strcmp

/*
* cf notify.c pour les erreurs
*
*/


int run_cmd(interpreteur inter, Memory *mem/* autres paramètres à ajouter */) 
{
	int r = 0;
	char usage[] = "Usage: run {address}";
	vaddr32 va = mem->reg[15]; // valeur courante du PC



	r = get_last_if_hexa(inter, &va);

	if (r ==  11 || r == 12) {
		printf("%s\n", usage);
		return r;
	}

	//TODO

	return 0;
}







int step_cmd(interpreteur inter, Memory *mem/* dic ?*/) 
{
	int flag = 0, r = 1;
	char *token = NULL;
	char usage[] = "Usage: step\n\tstep into";

	r = get_last_token(inter, &token);


	if(r == 0) // step into
	{
		if(strcmp(token, "into") == 0)
			flag = INTO;

		else {
			printf("%s\n", usage);
			return 11;
		}
	}
	else if(r != 1) // ni step ni step into
	{
		printf("%s\n", usage);
		return r;
	}

	//TODO

	return 0;
}
