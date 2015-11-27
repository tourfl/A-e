
#include "inter/command.h"

#include "inter/notify.h" // messages de contrôle
#include <string.h> // strcmp
#include "elf/bits.h" // wrd_good_endianness
#include "dic/mov.h"

/*
* cf notify.c pour les erreurs
*
*/


int run_cmd(Emulator *emul) 
{
	int r = 1, s = 0;
	char usage[] = "Usage: run {address}";
	vaddr32 va = emul->reg[15]; // valeur courante du PC



	// Analyse syntaxique
	r = get_last_if_addr(emul->inter, &va); // cf interpreteur.c

	if (r ==  11 || r == 12) {
		printf("%s\n", usage);
		return r;
	}

	s = step(INTO, emul);

	if(s != 2 && s != 4)
		return 1;

	while(s == 2 || s == 4) {
		s = step(INTO, emul);
	} 

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

	step(flag, emul);

	return 0;
}





// flag= INTO or 0

int step(int flag, Emulator *emul)
{
	int r = 0;
	word in = get_word(emul->reg[15], emul->map);
	Ins_disas *out = init_ins();




	if(flag == INTO)
	{
		r = find_and_decode(in, out, emul->dic); // retourne l'offset (2 : instruction 16 bits, 4 : 32 bits)

		if (r == 3)
			return r;

		else if (r == 2 || r == 4)
		{
			disp_insd(*out);

			
			if(strcmp(out->commande, "mov_imm") == 0)
			{
				mov_imm(*out, emul->reg);
			}
			// exec(*out);
		}

		emul->reg[15] += r;

		return r;
	}
	else
	{
		return 2;
	}
}


