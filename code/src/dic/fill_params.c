#include "dic/fill_params.h"

#include <string.h> // strcat
#include <math.h> // pow
#include <stdlib.h> // calloc
#include "inter/notify.h" // printf notamment
#include "elf/bits.h" // to_good_endianness

#include "dic/display_ins.h"














int fill_params_default(word in, Instruction *out)
{
	int r1, r2, r3;

	r1 = parse_reg(in, out);

	r2 = parse_imm(in, out);

	r3 = parse_ext(in, out);

	return r1 + r2 + r3;
}




int fill_params_pop_push(word in, Instruction *out)
{
	if(out->encoding == 3)
		return parse_reg(in, out);


	return parse_reglist(in, out);
}





int parse_params(word mot, Plgtab *tab)
{
    int i;


    // printf("\n word : %08x", mot);

    for(i = 0; i < tab->size; i++)
    {
    	Plage *p = &(tab->plages[i]);


        p->value = parse_param(mot, *p);
        // printf("\nstart: %u, end: %u, value[%u] = %u", p->start, p->end, i, p->value);
    }


    return 0; // ne sert pas à grand chose...
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



int parse_reg(word in, Instruction *out)
{
	return parse_params(in, out->reg);
}

int parse_imm(word in, Instruction *out)
{
	return parse_params(in, out->imm);
}

int parse_ext(word in, Instruction *out)
{
	return parse_params(in, out->ext);
}





int parse_reglist(word in, Instruction *out)
{
	int i, l=0, k=0;
	word reglist = 0;



	if(strcmp(out->commande, "pop") == 0)
	{
		reglist = parse_pop_reglist(in, out);
	}
	else if(strcmp(out->commande, "push") == 0)
	{
		reglist = parse_push_reglist(in, out);
	}


	free(out->reg->plages);

	l = masklen(reglist); // nombre de bits à 1
	out->reg->size = l;

	out->reg->plages = calloc(l, sizeof(Plage));

	for (i = 16; i >= 0; i--)
	{
		if(reglist >> i == 1)
		{
			// printf("i=%u", i);
			out->reg->plages[l - 1 - k].value = i;

			reglist -= pow(2, i);
			k++;
		}
	}


	return k;
}






int parse_pop_reglist(word in, Instruction *out)
{
	int i=0;
	word reglist=0;



	if(parse_reg(in, out) != 0)
		return 14;

	reglist = out->reg->plages[i].value << 15;
	i++;

	if(out->encoding == 2)
	{
		reglist += out->reg->plages[i].value << 14;
		i++;
	}

	reglist += out->reg->plages[i].value;


	return reglist;
}







int parse_push_reglist(word in, Instruction *out)
{
	word reglist=0;







	if(parse_reg(in, out) != 0)
		return 14;


	reglist = out->reg->plages[0].value << 14;

	reglist += out->reg->plages[1].value;

	// printf("reglist = %u", reglist);

	return reglist;
}


