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
 * valeur de retour : 1 si rien n'est trouvé
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

	return 1;
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
