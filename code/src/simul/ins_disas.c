#include "simul/ins_disas.h"
#include <string.h> // strcat
#include <math.h> // pow
#include <stdlib.h> // calloc
#include "inter/notify.h" // printf notamment
#include "types.h" // intobin
#include "elf/bits.h" // to_good_endianness



/*
 * in : mot à décoder
 * out : instruction de sortie
 * dic : tableau d'instructions
 * sz_dic : nb d'instructions dans le tableau
 */


int get_ins(word in, Instruction *out, Instruction dic[], int sz_dic) // retourne l'instruction en question s'il y a match, NULL sinon;
{
	/*
	 * Problème : les fichiers .o sont codés en little endian aligné (cf 2.4)
	 * Les masques sont en big endian
	 *
	 * ce problème est résolu dans la fonction disasm_plage
	 */

	int i=0;

	if(in == 0)
	{
		return 1;
	}



	//V2

	while (i<sz_dic && dic[i].commande != NULL) {

		if((in & dic[i].mask) == (dic[i].opcode & dic[i].mask)) {
			cpy_ins(out, &(dic[i]));

			return 0;
		}

		i++;
	}

	return 10;
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





int parse_params(word mot, Plgtab *tab)
{
    int i;




    if(tab->size == 0)
        return 3;

    for(i = 0; i < tab->size; i++)
    {
        parse_param(mot, &(tab->plages[i])); // Valeur de retour non-utilisée
        // printf("\nvalue[%u] = %u", i, tab_d->plages[i].value);
    }


    return 0;
}






int parse_param(word mot, Plage *p) // On utilise un paramètre de sortie pour renvoyer des codes d'erreur en valeur de retour
{
    uint i;
    int t = 16;
    char *word_bin = NULL, *val_bin = NULL;

    if(mot > pow(2, 16))
        t = 32;


    word_bin = int_to_bin(mot, t);
    val_bin = calloc(p->end - p->start + 2, sizeof(char));

    to_good_endianness(&word_bin, t);
    // printf("\nAfter flip: %s \n", word_bin);

    if(p->start > p->end)
        return 3;

    for(i = p->start; i <= p->end; i++)
    {
        val_bin[p->end - i] = word_bin[i]; // A cause de l'endianness
    }

    p->value = strtoul(val_bin, NULL, 2);

    // printf("\nval_bin : %s et value : %u\n", val_bin, p->value);

    return 0;
}





// Pour une instruction désassemblée

void disp_insd(Ins_disas ins)
{
	int i;
	char imm[33] = {0};



	printf("\n%s", ins.name_in);
	// printf("(%s/%s)", ins.commande, ins.encoding);

	for(i = 0; i < ins.reg->size; i++)
	{
		if(i != 0)
			printf(",");

		switch (ins.reg->plages[i].value)
		{
			case 13: {
				printf(" sp");
				break;
			}
			case 14: {
				printf(" lr");
				break;
			}
			case 15: {
				printf(" pc");
				break;
			}
			case 16: {
				printf(" apsr");
				break;
			}
			default: {
				printf(" r%u", ins.reg->plages[i].value);				
				break;
			}
		}
	}

	for(i = 0; i < ins.imm->size; i++)
		strcat(imm, int_to_bin(ins.imm->plages[i].value, ins.imm->plages[i].end - ins.imm->plages[i].start));

	// printf("\nimm:%s\n", imm);

	if(ins.imm->size > 0)
	{
		printf(", #%lu (%8lx)", strtoul(imm, NULL, 2), strtoul(imm, NULL, 2));
	}
}
