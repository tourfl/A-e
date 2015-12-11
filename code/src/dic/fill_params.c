#include "dic/fill_params.h"

#include <string.h> // strcat
#include <math.h> // pow
#include <stdlib.h> // calloc
#include "inter/notify.h" // printf notamment
#include "elf/bits.h" // to_good_endianness
#include "dic/code_arm.h" // ZeroExtend

#include "dic/display_ins.h"














int fill_params_default(word in, Instruction *out)
{
	int imm=0, r=0;

	

	if( (r = parse_all_params(in, out)) != 0)
		return r;





	imm = ZeroExtend(*(out->imm));

	if(out->imm->size > 0)
		fill_with_final_value(imm, out->imm);

	return 0;
}





int fill_with_final_value(int in, Plgtab *tab)
{
	del_plgtab(tab);

	if ( (tab = init_plgtab()) == NULL)
		return 2;

	tab->size = 1;

	if (( tab->plages = calloc(1, sizeof(Plage)) ) == NULL)
		return 2;

	tab->plages->value = in;

	return 0;
}





int fill_params_B(word in, Instruction *out)
{
	int imm=0, r=0;

	

	if( (r = parse_all_params(in, out)) != 0)
		return r;



	imm = SignExtend(*(out->imm));



	fill_with_final_value(imm << 1, out->imm);


	return 0;
}




int fill_params_BL(word in, Instruction *out)
{
	int imm=0, r=0;

	

	if( (r = parse_all_params(in, out)) != 0)
		return r;


	// Ji = not(S xor Ii)
	out->imm->plages[1].value = (~ (out->imm->plages[0].value ^ out->imm->plages[1].value) ) & 1;
	out->imm->plages[2].value = (~ (out->imm->plages[0].value ^ out->imm->plages[2].value) ) & 1;


	imm = SignExtend(*(out->imm));

	fill_with_final_value(imm << 1, out->imm);

	return 0;
}




int fill_params_pop_push(word in, Instruction *out)
{
	if(out->encoding == 3)
		return parse_reg(in, out);


	return parse_reglist(in, out);
}




int fill_params_sub_sp(word in, Instruction *out)
{
	int imm=0, r=0;



	if( (r = parse_all_params(in, out)) != 0)
		return r;


	imm = ZeroExtend(*(out->imm));

	if(out->encoding == 1)
		imm <<= 2;  // décalage de 2 bits, cf spécifications

	return fill_with_final_value(imm , out->imm);
}






int fill_params_ldr(word in, Instruction *out)
{
	int imm=0, r=0;



	if( (r = parse_all_params(in, out)) != 0)
		return r;

	imm = ZeroExtend( *(out->imm) );

	if(out->encoding == 1 || out->encoding == 2)
		imm <<= 2;

	return fill_with_final_value(imm, out->imm);
}





int fill_params_add_reg(word in, Instruction *out)
{
	int rdn=0;
	Plgtab *rdntab = plgtabclone(out->reg);



	if(out->encoding != 2)
		return fill_params_default(in , out);

	disp_plgtab(*out->reg);

	parse_params(in, out->reg);



	disp_plgtab(*out->reg);

	rdntab->size = 2; // il ne doit pas prendre en compte la dernière case

	rdn = ZeroExtend(*rdntab); // met la taille à 1 et concatène les valeurs binaires

	printf("\nrdn = %u", rdn);

	fill_with_final_value(rdn, rdntab);
	disp_plgtab(*rdntab);	

	rdntab->plages = realloc(rdntab->plages, 2 * sizeof(Plage));

	if(rdntab->plages == NULL)
		return 2;

	disp_plgtab(*rdntab);

	rdntab->size = 2;

	rdntab->plages[1] = out->reg->plages[2];

	free(out->reg);

	out->reg = rdntab;

	return 0;
}





int parse_all_params(word in, Instruction *out)
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

	// printf("reglist: %u\n", reglist);


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

			reglist -= 1 << i;
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


