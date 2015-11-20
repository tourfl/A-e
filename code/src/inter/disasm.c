/*
 *	disasm.c
 *	@date : 29/10/15
 */

 #include "inter/command.h" // inclut inter
 #include "types.h" // is_figure, vaddr32
 #include "inter/notify.h" // messages de contrôle
 #include "simul/ins_disas.h"
 #include <string.h> // strcmp
 #include <math.h> // pow

int disasm(Emulator *emul)
{
	char usage[] = "Usage : disasm <plage>+";
	int p = 1, r = 0;
	Plage plg;


	r = parse_plage(emul->inter, &plg);

	if(r == 1) // équivalent à un else pour le while
	{
		printf("%s\n", usage);
		return r;
	}

	while (r == 0)
	{
		// printf("start: 0x%08x;\tend: 0x%08x\n", plg.start, plg.end);

		p = disasm_plage(plg, emul);

		r = parse_plage(emul->inter, &plg);
	}

	if(r == 11) // équivalent à un else pour le while
	{
		printf("%s\n", usage);
		return r;
	}

	return p;  // renvoie la valeur de retour de la dernière plage lue
}








int disasm_plage(Plage plg, Emulator *emul) // On suppose va_1 < va_2
{
	byte *plage = NULL;
	uint i = 0, p = 1, r;
	word mot;

	// On récupère la plage d'octet si elle est correcte (taille > 1 bytes, va_2 > va_1)

	if(plg.end <= plg.start)
		return 1;

	plage = get_plage(plg, emul->map);

	disp_plage(plg, emul->map);

	while (i < plg.end - plg.start - 1) // il reste au moins 2 octets à lire (une instruction sur 16 bits)
	{
		Instruction ins;
		mot = 0;
		r = 1; // pas d'instruction lue dans ce tour de boucle

		mot = plage[i+1]*pow(16, 2) + plage[i]; // de BIG ENDIAN (segment) vers LITTLE ENDIAN (masque des instructions) (!)
		
		r = get_ins16(mot, &ins, emul->dic);

		if (r == 0) // C'est une instruction 16 bits
			i += 2;

		else if(i <= plg.end - plg.start - 3) // il reste au moins un mot à lire
		{
			// printf("unfound word: %8x\t%s\n", mot, int_to_bin(mot, 16));

			mot = mot * pow(16, 4); // Little Endian ALIGNE (!)
			mot += plage[i+3]*pow(16, 2) + plage[i+2]; // On ajoute les 2 octets suivants de BIG ENDIAN (segment) vers LITTLE ENDIAN (masque des instructions) (!)

			r = get_ins32(mot, &ins, emul->dic);

			if (r == 0) // C'est une instruction 32
				i += 4; // on augmente de 4 puisque une instruction de 4 octets a été lue

			else i++;
		}
		else i++; // S'il reste 3 octets
		

		if(r == 0)
		{
			p = 0; // signifie qu'au moins une instruction a été lue
			
			disasm_ins(mot, &ins);

			disp_insd(ins);

			/* ne surtout pas supprimer le contenu des instructions (ins, ins_d), sinon on le supprime du dic */
		}
		else if (r == 1)
		{
			WARNING_MSG("word is null");
		}
		else {
			printf("unfound word: %8x\t%s\n", mot, int_to_bin(mot, 32));
			WARNING_MSG("unable to find instruction");
		}
	}

	printf("\n\n");

	return p;
}







int disasm_ins(word mot, Instruction *ins)
{
	int i, r = 0;
	Plgtab *p = NULL; // cf types.h

	for(i = 0; i < 3; i++)
	{
		if(i == 0) {
			p = ins->reg;
		}

		else if(i == 1) {
			p = ins->imm;
		}

		else if(i == 2) {
			p = ins->ext;
		}

		r = parse_params(mot, p);

		if(r != 0 && r != 3)
			return r;

	}

	return 0;
}
