/*
 *	disasm.c
 *	@date : 29/10/15
 */

 #include "inter/command.h" // inclut inter
 #include "types.h" // is_figure, vaddr32
 #include "inter/notify.h" // messages de contrôle
 #include <string.h> // strcmp
 #include <math.h> // pow
 #include "dic/display_ins.h" // display

int disasm_cmd(Emulator *emul)
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
	uint i = 0, j, p = 14 /* #error code*/, k = 0;
	Decodage r=UNFOUND;
	word mot;

	// On récupère la plage d'octet si elle est correcte (taille > 1 bytes, va_2 > va_1)

	if(plg.end <= plg.start)
		return 1;

	plage = get_plage(plg, emul->map);

	// disp_plage(plg, emul->map);

	while (i < plg.end - plg.start - 1) // il reste au moins 2 octets à lire (une instruction sur 16 bits)
	{
		Instruction *ins = NULL;


		ins = init_ins();

		if(ins == NULL)
			return 2;

		for (j = 0; j < 4; ++j)
			mot = (mot << 8) + plage[i+j];

		i += ( r = disasm(mot, ins, emul->dic) );


		if(r == UNFOUND)
		{
			k++;
		}
		else if(r == PARSE_ERROR || r == PREPROCESS_ERROR)
			return r;
		else
		{
			if(k == 1)
			{
				printf("\033[00;31m");
				disp_oct(plg.start + i - r - 1, emul->map);
				printf("\033[0m");
				k = 0;
			}
			else if (k > 1)
			{
				printf("\033[00;31m");
				Plage pl = {plg.start + i - r - k, plg.start + i - r - 1, 0};
				disp_plage(pl, emul->map);
				printf("\033[0m");
				k = 0;
			}



			p = 0; // signifie qu'au moins une instruction a été lue

			printf("\n0x%08x\t\033[00;32m", plg.start + i - r);

			if(r == 4)
				printf("%08x\t", mot);
			else
				printf("%04x\t\t", mot >> 16);

			printf("\033[0m");

			display(*ins, DECODED, emul);

			/* ne surtout pas supprimer le contenu des instructions (ins, ins_d), sinon on le supprime du dic */
		}
	}
	if (k > 0)
	{
		printf("\033[00;31m");
		Plage pl = {plg.start + i - r - k + 1, plg.start + i - r, 0};
		disp_plage(pl, emul->map);
		printf("\033[0m");
		k = 0;
	}

	printf("\n\n");

	return p;
}
