
#include "dic/preprocess.h"
#include "dic/code_arm.h" // ZeroExtend_plgtab
#include <string.h> // strcmp
#include "inter/notify.h" // messages de contrôle










int preprocess_default(Instruction *out)
{
	ZeroExtend_plgtab(&out->imm);

	return 0;
}




int preprocess_B(Instruction *out)
{
	SignExtend_plgtab(&out->imm);

	return 0;
}




int preprocess_BL(Instruction *out)
{
	// Ji = not(S xor Ii)
	out->imm.plages[1].value = (~ (out->imm.plages[0].value ^ out->imm.plages[1].value) ) & 1;
	out->imm.plages[2].value = (~ (out->imm.plages[0].value ^ out->imm.plages[2].value) ) & 1;


	SignExtend_plgtab(&out->imm);

	return 0;
}




int preprocess_sub_sp(Instruction *out)
{
	ZeroExtend_plgtab(&out->imm);

	if(out->encoding == 1)
		out->imm.plages->value <<= 2;  // décalage de 2 bits, cf spécifications

	return 0;
}




int preprocess_ldr(Instruction *out)
{
	ZeroExtend_plgtab(&out->imm);

	if(out->encoding == 1 || out->encoding == 2)
		out->imm.plages->value <<= 2;

	return 0;
}



/*
	semblable à :
	- sub_reg_16
	- mul_16
	- mov_reg_16
*/

int preprocess_add_reg_16(Instruction *out)
{
	Plgtab rdntab;





	plgtabdup(&rdntab, &out->reg);

	rdntab.size = 2; // il ne doit pas prendre en compte la dernière case

	ZeroExtend_plgtab(&rdntab); // met la taille à 1 et concatène les valeurs binaires


	rdntab.plages = realloc(rdntab.plages, 2 * sizeof(Plage));

	if(rdntab.plages == NULL)
		return 2;

	rdntab.size = 2;

	rdntab.plages[1] = out->reg.plages[2];

	free(out->reg.plages);

	out->reg = rdntab;

	return 0;
}










int preprocess_pop_push(Instruction *out)
{
	if(out->encoding != 3)
		preprocess_reglist(out);

	return 0;
}








int preprocess_reglist(Instruction *out)
{
	int i, l=0, k=0;
	word reglist = 0;



	if(strcmp(out->commande, "pop") == 0)
	{
		reglist = preprocess_pop_reglist(out);
	}
	else if(strcmp(out->commande, "push") == 0)
	{
		reglist = preprocess_push_reglist(out);
	}

	// printf("reglist: %u\n", reglist);


	free(out->reg.plages);

	l = masklen(reglist); // nombre de bits à 1
	out->reg.size = l;

	out->reg.plages = calloc(l, sizeof(Plage));

	for (i = 16; i >= 0; i--)
	{
		if(reglist >> i == 1)
		{
			// printf("i=%u", i);
			out->reg.plages[l - 1 - k].value = i;

			reglist -= 1 << i;
			k++;
		}
	}


	return k;
}






int preprocess_pop_reglist(Instruction *out)
{
	int i=0;
	word reglist=0;





	reglist = out->reg.plages[i].value << 15;
	i++;

	if(out->encoding == 2)
	{
		reglist += out->reg.plages[i].value << 14;
		i++;
	}

	reglist += out->reg.plages[i].value;


	return reglist;
}







int preprocess_push_reglist(Instruction *out)
{
	word reglist=0;





	reglist = out->reg.plages[0].value << 14;

	reglist += out->reg.plages[1].value;

	// printf("reglist = %u", reglist);

	return reglist;
}


