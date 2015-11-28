#include "dic/ins_disas.h"
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
			insclone(out, &(dic[i]));

			return 0;
		}

		i++;
	}

	return 10;
}











int fill_params(word in, Instruction *out)
{
	int i, r = 0;
	Plgtab *p = NULL; // cf types.h




	for(i = 0; i < 3; i++)
	{
		if(i == 0) {
			p = out->reg;
		}

		else if(i == 1) {
			p = out->imm;
		}

		else if(i == 2) {
			p = out->ext;
		}

		r = parse_params(in, p);

		if(r != 0 && r != 3)
			return r;

	}

	return 0;
}





int parse_params(word mot, Plgtab *tab)
{
    int i;


    // printf("\n word : %08x", mot);

    if(tab->size == 0)
        return 3;

    for(i = 0; i < tab->size; i++)
    {
    	Plage *p = &(tab->plages[i]);


        p->value = parse_param(mot, *p);
        // printf("\nstart: %u, end: %u, value[%u] = %u", p->start, p->end, i, p->value);
    }


    return 0;
}






int parse_param(word mot, Plage p)
{
	word mask = plg_to_mask(p);
	int r = (int) (mot & mask) >> p.start;



    // V2

	// disp_plg(p);
	// printf("\nword: %s\nmask: %s\t res: %u", int_to_bin(mot, 32), int_to_bin(mask, 32), r);



	return r;
}





// Pour une instruction désassemblée

void disp_insd(Ins_disas ins)
{
	int i;
	char imm[33] = {0};





	printf("\n");
	printf("%s", ins.name_in);
	printf("(%s/%s)", ins.commande, ins.encoding);

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

	printf("1ere valeur : %u\n", ins.imm->plages[0].value);

	for(i = 0; i < ins.imm->size; i++)
		strcat(imm, int_to_bin(ins.imm->plages[i].value, ins.imm->plages[i].end - ins.imm->plages[i].start));

	// printf("\nimm:%s\n", imm);

	if(ins.imm->size > 0)
	{
		printf(", #%lu (%08lx)", strtoul(imm, NULL, 2), strtoul(imm, NULL, 2));
	}
}
