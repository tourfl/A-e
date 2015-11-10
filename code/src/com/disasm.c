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
	int i = 0, j, p = 1;
	word mot = 0;

	// On récupère la plage d'octet si elle est correcte (taille > 1 bytes, va_2 > va_1)

	if(va_2 <= va_1)
		return 1;

	plage = get_plage(va_1, va_2, mem->map);

	printf("\nplage : ");
	for (j = 0; j < va_2 - va_1 +1; ++j)
		printf("%2x ", plage[j]);
	printf("\n");

	// On charge le dictionnaire si ce n'est pas déjà fait

	if(dic->ins32[0].commande.content == NULL && load_dic(dic) != 0) // ie si le dictionnaire n'a pas été chargé et ne charge pas
		return 1;

	while (i < va_2 - va_1 - 1) // il reste au moins 2 octets à lire (une instruction sur 16 bits)
	{
		Instruction ins;
		init_ins(&ins); // très important
		mot = 0;

		if(plage[i] == 0)
			i ++;

		// On essaye de désassembler une insruction sur 16 bits

		mot = plage[i+1]*pow(16, 2) + plage[i]; // de BIG ENDIAN (segment) vers LITTLE ENDIAN (masque des instructions) (!)
		ins = get_ins16(mot, dic);

		if(ins.commande.content != NULL) // C'est une instruction 16
			i += 2; // on augment de 2 puisque une instruction de 2 octets a été lue

		else if(i <= va_2 - va_1 - 3) // il reste au moins un mot à lire
		{
			mot = mot * pow(16, 4); // Little Endian ALIGNE (!)
			mot += plage[i+3]*pow(16, 2) + plage[i+2]; // On ajoute les 2 octets suivants de BIG ENDIAN (segment) vers LITTLE ENDIAN (masque des instructions) (!)

			ins = get_ins32(mot, dic);

			if(ins.commande.content != NULL) // C'est une instruction 32
				i += 4; // on augmente de 4 puisque une instruction de 4 octets a été lue

			else i++;
		}
		else i++; // S'il reste 3 octets

		if(ins.commande.content != NULL && strcmp(ins.commande.content[0], "") != 0)
		{
			Ins_disasmd ins_d;
			init_ins_d(&ins_d, &ins);

			p = 0; // signifie qu'au moins une instruction a été lue
			disasm_ins(mot, &ins, &ins_d);
			disp_ins(ins_d);
			/* ne surtout pas supprimer le contenu des instructions (ins, ins_d), sinon on le supprime du dic */
		}
		else
			WARNING_MSG("unable to find instruction");
	}

	return p;
}







int disasm_ins(word mot, Instruction *ins, Ins_disasmd *ins_d)
{
	int i, r = 0;
	Strlist *strl = NULL, *strl_d = NULL;



	for(i = 0; i < 3; i++)
	{
		if(i == 0) {
			strl = &(ins->reg);
			strl_d = &(ins_d->reg);
		}

		else if(i == 1) {
			strl = &(ins->imm);
			strl_d = &(ins_d->imm);
		}

		else if(i == 2) {
			strl = &(ins->ext);
			strl_d = &(ins_d->ext);
		}

		r = parse_params(mot, strl, strl_d);

		if(r != 0 && r != 3)
			return r;

	}

	return 0;
}
