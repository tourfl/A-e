#include "dic/parse_params.h"

#include <string.h> // strcat
#include <math.h> // pow
#include <stdlib.h> // calloc
#include "inter/notify.h" // printf notamment
#include "elf/bits.h" // to_good_endianness
#include "dic/code_arm.h" // ZeroExtend_plgtab

#include "dic/display_ins.h"









int parse(word in, Instruction *out)
{
	int r=0, r1, r2, r3;




	r1 = parse_reg(in, out);

	r2 = parse_imm(in, out);

	r3 = parse_ext(in, out);

	r = r1 + r2 + r3;
	
	return r;
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
	word r=0;




	if(p.start == p.end)
		r = GET_BIT(mot, p.start);

	else
		r = GET_BITS(mot, p.start, p.end + 1);

    // V2

	// disp_plg(p);
	// printf("\nword: %s\t res: %u", int_to_bin(mot, 32), r);



	return (int) (r >> p.start);
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


