
#include "inter/command.h"
#include "inter/notify.h" // messages de contrôle
#include <string.h> // strcmp
#include <CUnit/CUnit.h>

/*
* cf notify.c pour les erreurs
*
*/


int run_cmd(Emulator *emul) 
{
	int r = 1;
	char usage[] = "Usage: run {address}";
	vaddr32 va = emul->reg[15]; // valeur courante du PC



	// Analyse syntaxique
	r = get_last_if_addr(emul->inter, &va); // cf interpreteur.c

	if (r ==  11 || r == 12) {
		printf("%s\n", usage);
		return r;
	}

	// while()

	return 0;
}







int step_cmd(Emulator *emul) 
{
	int flag = 0, r = 1;
	char *token = NULL;
	char usage[] = "Usage: step\n\tstep into";



	// Analyse syntaxique
	r = get_last_token(emul->inter, &token);

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

	// step(flag, emul);

	return 0;
}





// flag: INTO or not

int step(int flag, Emulator *emul)
{
	if(flag == INTO)
	{
		return 0;
	}
	else
	{
		return 2;
	}
}



// int TONAME(vaddr32 address, Emulator *emul)


