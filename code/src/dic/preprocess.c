
#include "dic/preprocess.h"
#include "dic/code_arm.h" // ZeroExtend_plgtab
#include <string.h> // strcmp
#include "inter/notify.h" // messages de contrôle
#include "simul/it_state.h" // InITBlock










int preprocess_default(Instruction *out, Emulator *emul)
{
	ZeroExtend_plgtab(&out->imm);

	if(InITBlock(emul->it_state))
	{
		process_state(&emul->it_state, out->name_in, emul->dic->states_tab);
		out->it_flag = IN;
	}

	return 0;
}




int preprocess_B(Instruction *out, Emulator *emul)
{
	SignExtend_plgtab(&out->imm);

	if(InITBlock(emul->it_state))
	{
		process_state(&emul->it_state, out->name_in, emul->dic->states_tab);
		out->it_flag = IN;
	}

	return 0;
}




int preprocess_BL(Instruction *out, Emulator *emul)
{
	// Ji = not(S xor Ii)
	out->imm.plages[1].value = (~ (out->imm.plages[0].value ^ out->imm.plages[1].value) ) & 1;
	out->imm.plages[2].value = (~ (out->imm.plages[0].value ^ out->imm.plages[2].value) ) & 1;


	SignExtend_plgtab(&out->imm);

	if(InITBlock(emul->it_state))
	{
		process_state(&emul->it_state, out->name_in, emul->dic->states_tab);
		out->it_flag = IN;
	}

	return 0;
}




int preprocess_sub_sp(Instruction *out, Emulator *emul)
{
	ZeroExtend_plgtab(&out->imm);

	if(InITBlock(emul->it_state))
	{
		process_state(&emul->it_state, out->name_in, emul->dic->states_tab);
		out->it_flag = IN;
	}

	if(out->encoding == 1)
		out->imm.plages->value <<= 2;  // décalage de 2 bits, cf spécifications

	return 0;
}




int preprocess_ldr(Instruction *out, Emulator *emul)
{
	ZeroExtend_plgtab(&out->imm);

	if(InITBlock(emul->it_state))
	{
		process_state(&emul->it_state, out->name_in, emul->dic->states_tab);
		out->it_flag = IN;
	}

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

int preprocess_add_reg_16(Instruction *out, Emulator *emul)
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

	if(InITBlock(emul->it_state))
	{
		process_state(&emul->it_state, out->name_in, emul->dic->states_tab);
		out->it_flag = IN;
	}

	return 0;
}










int preprocess_pop_push(Instruction *out, Emulator *emul)
{
	if(out->encoding != 3)
		preprocess_reglist(out);

	if(InITBlock(emul->it_state))
	{
		process_state(&emul->it_state, out->name_in, emul->dic->states_tab);
		out->it_flag = IN;
	}

	return 0;
}







int preprocess_it(Instruction *out, Emulator *emul)
{
	char msk = (char) out->ext.plages[0].value;
	int mask=0, sign=0, thn=out->ext.plages[1].value, els=0;
	int i, k;
	char cond0 = GET_BIT(thn, 0);


	for (k = 0; k < 4; k++) // trouve le premier bit à 1
	{
		if(GET_BIT(msk, k) == (1 << k) )
			break;
	}



	for (i = 3; i > k; i--)
	{
		if( GET_BIT(msk, i) == (cond0 << i) )
		{
			strcat(out->name_out, "T");
			sign += (sign << 1) + 1;
		}
		else
		{
			strcat(out->name_out, "E");
			sign <<= 1;
		}
	}

	strcat(out->name_out, " ");
	strcat(out->name_out, emul->dic->states_tab[thn]);


	mask = (1 << (4 - k)) - 1; // mask au format de la fonction process_state
	sign += (sign << 1) + 1; // la première condition est forcément Then

	// printf("k = %u\tmask = %s\n", k, int_to_bin(mask, 4));

	if( (thn & 1) == 0 )
		els = thn + 1;
	else
		els = thn - 1;

	set_it_state(&emul->it_state, mask, sign, thn, els);

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


