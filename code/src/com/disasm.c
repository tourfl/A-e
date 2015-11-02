/*
 *	disasm.c
 *	@date : 29/10/15
 */

 #include "com/command.h" // inclut inter
 #include "com/types.h" // is_figure, vaddr32
 #include "com/notify.h" // messages de contrôle
 #include <string.h> // strcmp
 #include <math.h> // pow

int disasm(interpreteur inter, Memory *mem, Dic *dic)
{
	char *va;
	vaddr32 virt_ad = 0;
	char *token;
	vaddr32 virt_ad_2 = 0;
	uint32_t size = 0;
	char usage[] = "Usage : disasm <plage>+\n";
	int p = 1;

	va = get_next_token(inter);

	if(va == NULL)
	{
		WARNING_MSG("Problem with tokens");
		printf("%s", usage);
		return 1;
	}

	while(va != NULL)
	{
		if(is_hexa(va) != 0)
		{
			WARNING_MSG("address must be hexadecimal");
			return 1;
		}

		virt_ad = strtoul(va, NULL, 16);
		token = get_next_token(inter);

		if (token != NULL && strcmp(token, ":") == 0)
		{
			token = get_next_token(inter);

			if(token != NULL && is_hexa(token) == 0)
			{
				virt_ad_2 = strtoul(token, NULL, 16);

				if(virt_ad_2 > virt_ad)
					p = disasm_plage(virt_ad, virt_ad_2, mem, dic);
			}
		}
		else if (token != NULL && strcmp(token, "+") == 0)
		{
			token = get_next_token(inter);

			if(token != NULL && is_figure(token) == 0)
			{
				size = strtoul(token, NULL, 0);

				if(size > 0 && size < (0xffffffff - virt_ad)) // On évite les dépassements de mémoire
					p = disasm_plage(virt_ad, virt_ad+size, mem, dic);
			}

		}
		else
		{
			WARNING_MSG("invalid or missing second value");
			return 1;
		}

		va = get_next_token(inter);
	}

	return p;  // renvoie la valeur de retour de la dernière plage lue
}








int disasm_plage(vaddr32 va_1, vaddr32 va_2, Memory *mem, Dic *dic) // On suppose va_1 < va_2
{
	byte *plage = NULL;
	int i = 0;
	word mot = 0;
	Instruction ins;

	// On récupère la plage d'octet
	
	printf("%08x", va_2);

	plage = get_plage(va_1, va_2, mem->map);
	

	// On charge le dictionnaire si ce n'est pas déjà fait

	if(dic->ins32[0].commande == NULL && load_dic(dic) != 0) // ie si le dictionnaire n'a pas été chargé et ne charge pas
		return 1;

	while (i < va_2 - va_1 - 1) // il reste au moins 2 octets à lire (une instruction sur 16 bits)
	{
		if(plage[i] == 0)
			i ++;

		else if(i > va_2 - va_2 - 3) // il reste moins d'un mot à lire
		{
			mot = plage[i]*pow(16, 2) + plage[i+1];
			ins = get_ins16(mot, dic);

			if(ins.commande != NULL) // C'est une instruction 16
				i += 2; // on augment de 2 puisque une instruction de 2 octets a été lue
		}
		else {
			int j;
			for (j = 0; j < 4; ++j)
				mot += plage[i + j] * pow(16, 2 * (3 - j)); // bug avec pow(16, 0) : Non

			ins = get_ins32(mot, dic);

			if(ins.commande != NULL) // C'est une instruction 32
				i += 4; // on augment de 4 puisque une instruction de 4 octets a été lue

			else {
				ins = get_ins16(mot, dic);

				if(ins.commande != NULL) // C'est une instruction 16
					i += 2; // on augmente de 2 puisque une instruction de 2 octets a été lue
			}
		}

		if(ins.commande == NULL)
		{
			i ++;
			WARNING_MSG("unable to find instruction");
		}

		else
			disp_ins(ins);
	}

	return 0;
}
