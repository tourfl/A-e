
#include "inter/command.h"
#include "emulator.h" // Emulator
#include <string.h> // strcmp
#include "inter/notify.h"



// Analyse syntaxique

int break_cmd(Emulator *emul)
{

	// Reprise de dispcmd (3 niveaux de conditions max)

	interpreteur inter = emul->inter;
	int r=0;
	char* what = get_next_token (inter);
	char usage[] = "Usage : break add <address>+\n\tbreak del <address>+\n\tbreak list";
	vaddr32 va = 0;


	
	if(what != NULL && strcmp(what, "add") == 0)
	{
		while( ( r = get_next_if_addr(emul->inter, &va) ) == 0)
		{
			add_breakpoint(&emul->breaklist, va);
		}

		if(r == 1) // last token == NULL
			return 0;

		// Si le retour en sortie de boucle n'est pas 1, on sort sans qu'il ne se soit rien passé
	}
	else if(what != NULL && strcmp(what, "del") == 0)
	{
		while( ( r = get_next_if_addr(emul->inter, &va) ) == 0)
		{
			del_breakpoint(&emul->breaklist, va);
		}

		if(r == 1) // last token == NULL
			return 0;

	}
	else if(what != NULL && strcmp(what, "list") == 0)
	{
		if(get_next_token(inter) == NULL)
		{
			disp_breaklist(emul->breaklist);
			return 0;
		}

		// Si le token suivant n'est pas nul, on sort sans qu'il ne se soit rien passé

	}



	printf("%s\n", usage);
	return 11;
}

