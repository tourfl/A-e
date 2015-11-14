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
		else if (strcmp(ponc, "+") == 0 && is_figure(v2) == 0) // Ce nombre peut-être en base 8, 10 ou 16
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
	int i = 0, j, p = 1, r;
	word mot;

	// On récupère la plage d'octet si elle est correcte (taille > 1 bytes, va_2 > va_1)

	if(va_2 <= va_1)
		return 1;

	plage = get_plage(va_1, va_2, mem->map);

	printf("\nplage : ");
	for (j = 0; j < va_2 - va_1 +1; ++j)
		printf("%2x ", plage[j]);
	printf("\n");

	while (i < va_2 - va_1 - 1) // il reste au moins 2 octets à lire (une instruction sur 16 bits)
	{
		Instruction ins;
		mot = 0;
		r = 1; // pas d'instruction lue dans ce tour de boucle

		mot = plage[i+1]*pow(16, 2) + plage[i]; // de BIG ENDIAN (segment) vers LITTLE ENDIAN (masque des instructions) (!)
		
		r = get_ins16(mot, &ins, dic);

		if (r == 0) // C'est une instruction 16 bits
			i += 2;

		else if(i <= va_2 - va_1 - 3) // il reste au moins un mot à lire
		{
			// printf("unfound word: %8x\t%s\n", mot, int_to_bin(mot, 16));

			mot = mot * pow(16, 4); // Little Endian ALIGNE (!)
			mot += plage[i+3]*pow(16, 2) + plage[i+2]; // On ajoute les 2 octets suivants de BIG ENDIAN (segment) vers LITTLE ENDIAN (masque des instructions) (!)

			r = get_ins32(mot, &ins, dic);

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
			p = &(ins->reg);
		}

		else if(i == 1) {
			p = &(ins->imm);
		}

		else if(i == 2) {
			p = &(ins->ext);
		}

		r = parse_params(mot, p);

		if(r != 0 && r != 3)
			return r;

	}

	return 0;
}
