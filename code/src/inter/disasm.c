/*
 *	disasm.c
 *	@date : 29/10/15
 */

 #include "inter/command.h" // inclut inter
 #include "types.h" // is_figure, vaddr32
 #include "inter/notify.h" // messages de contrôle
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
	uint i = 0, p = 1, r, k = 0;
	word mot;

	// On récupère la plage d'octet si elle est correcte (taille > 1 bytes, va_2 > va_1)

	if(plg.end <= plg.start)
		return 1;

	plage = get_plage(plg, emul->map);

	disp_plage(plg, emul->map);

	while (i < plg.end - plg.start - 1) // il reste au moins 2 octets à lire (une instruction sur 16 bits)
	{
		Instruction *ins = NULL;


		ins = init_ins();

		if(ins == NULL)
			return 2;

		mot = plage[i]*pow(16, 6) + plage[i+1]*pow(16, 4);
		mot+= plage[i+2]*pow(16, 2) + plage[i+3];

		r = decode(mot, ins, emul->dic);
		i+= r;

		if(r == 1)
		{
			k++;
		}
		else
		{
			if(k > 0)
			{
				WARNING_MSG("%i unfound instruction(s)", k);
				k = 0;
			}

			p = 0; // signifie qu'au moins une instruction a été lue

			// printf("\n%08x: ", mot);

			disp_insd(*ins);

			/* ne surtout pas supprimer le contenu des instructions (ins, ins_d), sinon on le supprime du dic */
		}
	}

	printf("\n\n");

	return p;
}
