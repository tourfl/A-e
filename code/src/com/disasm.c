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
	char *va = NULL;
	char *ponc = NULL;
	char *v2 = NULL;
	vaddr32 virt_ad = 0;
	vaddr32 virt_ad_2 = 0;
	uint32_t size = 0;
	char usage[] = "Usage : disasm <plage>+\n";
	int p = 1;

	va = get_next_token(inter);
	ponc = get_next_token(inter);
	v2 = get_next_token(inter);

	// printf("va : %s \n ponc : %s \n v2 : %s", va, ponc, v2);

	if(va == NULL || ponc == NULL || v2 == NULL)
	{
		WARNING_MSG("Problem with tokens");
		printf("%s", usage);	
	}

	while(va != NULL && ponc != NULL && v2 != NULL) // C'est un peu le else de la condition précédente
	{
		if(is_hexa(va) != 0)
		{
			WARNING_MSG("first address must be hexadecimal");
			break;
		}

		virt_ad = strtoul(va, NULL, 16);

		if (strcmp(ponc, ":") == 0 && is_hexa(v2) == 0) // Une adresse doit être hexadécimal
		{
			virt_ad_2 = strtoul(v2, NULL, 16);

			if(virt_ad_2 < virt_ad)
				p = disasm_plage(virt_ad_2, virt_ad, mem, dic);

			else
				p = disasm_plage(virt_ad, virt_ad_2, mem, dic);
		}
		else if (strcmp(ponc, "+") == 0 && is_dec(v2) == 0) // Ce nombre peut-être en base 8, 10 ou 16
		{
			size = strtoul(v2, NULL, 0);


			if(size < (0xffffffff - virt_ad)) // On évite les dépassements de mémoire
					virt_ad_2 = virt_ad + size;

			else {
				WARNING_MSG("second value is too big");
				break;
			}

			p = disasm_plage(virt_ad, virt_ad_2, mem, dic);
		}
		else {
			WARNING_MSG("invalid second value");
			break;
		}


		va = get_next_token(inter);
		ponc = get_next_token(inter);
		v2 = get_next_token(inter);	
	}

	return p;  // renvoie la valeur de retour de la dernière plage lue
}








int disasm_plage(vaddr32 va_1, vaddr32 va_2, Memory *mem, Dic *dic) // On suppose va_1 < va_2
{
	byte *plage = NULL;
	int i = 0;
	word mot = 0;
	Instruction ins;
	init_ins(&ins); // très important
	int p = 1;

	// On récupère la plage d'octet si elle est correcte (taille > 1 bytes, va_2 > va_1)

	if(va_2 <= va_1)
		return 1;

	plage = get_plage(va_1, va_2, mem->map);

	// On charge le dictionnaire si ce n'est pas déjà fait

	if(dic->ins32[0].commande == NULL && load_dic(dic) != 0) // ie si le dictionnaire n'a pas été chargé et ne charge pas
		return 1;

	while (i < va_2 - va_1) // il reste au moins 2 octets à lire (une instruction sur 16 bits)
	{
		if(plage[i] == 0)
			i ++;

		else if(i > va_2 - va_2 - 3) // il reste moins d'un mot à lire
		{
			mot = plage[i]*pow(16, 2) + plage[i+1];
			ins = get_ins16(mot, dic);

			if(ins.commande != NULL) // C'est une instruction 16
			{
				i += 2; // on augment de 2 puisque une instruction de 2 octets a été lue
				p = 0; // signifie qu'au moins une instruction a été lue
			}
			else i++;
		}
		else {
			int j;
			for (j = 0; j < 4; ++j)
				mot += plage[i + j] * pow(16, 2 * (3 - j)); // bug avec pow(16, 0) : Non

			ins = get_ins32(mot, dic);

			if(ins.commande != NULL) // C'est une instruction 32
			{
				i += 4; // on augment de 4 puisque une instruction de 4 octets a été lue
				p = 0;
			}

			else {
				ins = get_ins16(mot, dic);

				if(ins.commande != NULL) // C'est une instruction 16
				{
					i += 2; // on augmente de 2 puisque une instruction de 2 octets a été lue
					p = 0;
				}

				else i++;
			}
		}

		if(ins.commande == NULL)
			WARNING_MSG("unable to find instruction");

		else
			disp_ins(ins);
	}

	return p;
}
