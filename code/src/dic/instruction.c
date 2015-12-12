#include "dic/display_ins.h" // display, inclut instruction.h

#include <stdlib.h> // free
#include <string.h> // strtok
#include "inter/notify.h" // messages de contrôle
#include "types.h" // plgtab
#include "dic/preprocess.h" // preprocess_default
#include "inter/interpreteur.h" // string_standardise

// Pour les fonctions pointées

// #include "dic/mov.h"
#include "dic/ins.h"



Instruction* init_ins() // pour éviter les bugs lors de la désallocation
{
	Instruction *ins = calloc(1, sizeof(Instruction));

	if(ins == NULL)
		return NULL;

	ins->it_flag = OUT;

	return ins;
}






void del_ins(Instruction *ins)
{
	if(ins == NULL)
		return;

	free(ins->imm.plages);
	free(ins->reg.plages);
	free(ins->ext.plages);
}







void insclone(Instruction *dest, Instruction *src)
{
	strcpy(dest->commande, src->commande);
	dest->encoding = src->encoding;
	strcpy(dest->name_in, src->name_in);
	strcpy(dest->name_out, src->name_out);
	dest->mask = src->mask;
	dest->opcode = src->opcode;

	plgtabdup(&dest->reg, &src->reg);
	plgtabdup(&dest->imm, &src->imm);
	plgtabdup(&dest->ext, &src->ext);

	dest->it_flag = src->it_flag;


	dest->preprocess = src->preprocess;
	dest->run = src->run;
	dest->display_decoded = src->display_decoded;
}







int load_ins(Instruction *ins, char *chaine)
{
	int r=0;



	r = load_from_string(ins, chaine);

	if(r != 0)
		return r;

	init_pft(ins);

	if(ins->preprocess == NULL || ins->display_decoded == NULL)
		return 13;


	return 0;

}







int load_from_string(Instruction *ins, char *chaine)
{
	char token_s[STRLEN] = {0}, *token = NULL, *str = NULL, *saveptr = NULL;
	int i, r = 0;




	// V4


	for (i = 0, str = chaine; i < 6 && r == 0; ++i, str = NULL)
	{
		token = strtok_r(str, " \t", &saveptr);


		if(token == NULL) // Comprend le cas chaine == NULL
			break;





		// DISP_TOKEN(i, token);
		
		switch (i)
		{
			case 0: {

				string_standardise(token, token_s); // Pour utiliser sscanf avec des chaînes
				r = sscanf(token_s, "%s / T%u", ins->commande, &ins->encoding);

				r = (2) ? 0 : 2;
				break;
			}

			case 1: {
				
				string_standardise(token, token_s); // Pour utiliser sscanf avec des chaînes
				r = sscanf(token_s, "%s / %s", ins->name_in, ins->name_out);

				if(r == 1 || r == 2) r = 0;
				break;
			}
			
			case 2: {
				r = sscanf(token, "%x/%x", &ins->mask, &ins->opcode);

				r = (2) ? 0 : 2;
				break;
			}
			
			case 3 :
			r = to_plgtab(token, &ins->reg);
			break;
			
			case 4 :
			r = to_plgtab(token, &ins->imm);
			break;
			
			case 5 :
			r = to_plgtab(token, &ins->ext);
			break;
		}
	}

	return r;
}




// initialise les pointeurs de fonction de chaque instruction

void init_pft(Instruction *ins)
{
	ins->display_decoded = disp_default;
	ins->preprocess = preprocess_default;


	if(strcmp(ins->commande, "mov_imm") == 0)
	{
		ins->run = mov_imm;
	}
	else if (strcmp(ins->commande, "mov_reg") == 0)
	{
		ins->run = mov_reg;
	}
	else if (strcmp(ins->commande, "movt") == 0)
	{
		ins->run = movt;
	}
	else if (strcmp(ins->commande, "mul") == 0)
	{
		ins->run = mul;
	}
	else if(strcmp(ins->commande, "b") == 0)
	{
		if(ins->encoding == 1 || ins->encoding == 2)
			ins->fill_params = fill_params_B;

		else
			ins->fill_params = fill_params_BL;
	}
	else if(strcmp(ins->commande, "bl") == 0)
	{
		ins->fill_params = fill_params_BL;
	}
	else if(strcmp(ins->commande, "add_sp") == 0)
	{
		ins->display_decoded = disp_sub_sp;
		ins->fill_params = fill_params_ldr; // f_p_add_sp = f_p_ldr
	}
	else if(strcmp(ins->commande, "add_reg") == 0)
	{
		ins->fill_params = fill_params_add_reg;
		ins->run = add_reg;
	}
	else if(strcmp(ins->commande, "add_imm") == 0)
	{
		ins->run = add_imm;
	}
	else if(strcmp(ins->commande, "sub_imm") == 0)
	{
		ins->run = sub_imm;
	}
	else if(strcmp(ins->commande, "cmp_imm") == 0)
	{
		ins->run = cmp_imm;
	}
	else if(strcmp(ins->commande, "sub_sp") == 0)
	{
		ins->display_decoded = disp_sub_sp;
		ins->fill_params = fill_params_sub_sp;
	}
	else if(strcmp(ins->commande, "ldr_litt") == 0)
	{
		ins->fill_params = fill_params_sub_sp;
		ins->run = ldr_litt;
	}


	else if(strcmp(ins->commande, "pop") == 0)
	{
		ins->display_decoded = disp_pop_push;
		ins->fill_params = fill_params_pop_push;
		ins->run = pop;
	}
	else if (strcmp(ins->commande, "push") == 0)
	{
		ins->display_decoded = disp_pop_push;
		ins->fill_params = fill_params_pop_push;
		ins->run = push;
	}
	else if(strcmp(ins->commande, "ldr_imm") == 0)
	{
		ins->fill_params = fill_params_ldr;
		ins->display_decoded = disp_ldr;
		ins->run = ldr_imm;
	}
	else if(strcmp(ins->commande, "str_imm") == 0)
	{
		ins->fill_params = fill_params_ldr;
		ins->display_decoded = disp_ldr;
		ins->run = str_imm;
	}

	else if(strcmp(ins->commande, "str_reg") == 0)
	{
		ins->fill_params = fill_params_ldr;
		ins->display_decoded = disp_ldr;
		ins->run = str_reg;
	}
	else if(strcmp(ins->commande, "it") == 0)
	{
		ins->display_decoded = disp_it;
	}
	else if (strcmp(ins->commande, "cmp_reg") == 0)
	{
		ins->run = cmp_reg;
	}	


}






int cmp_ins(const void *ins1, const void *ins2)
{
	Instruction *_ins1 = (Instruction *) ins1;
	Instruction *_ins2 = (Instruction *) ins2;

	return cmp_mask(_ins1->mask, _ins2->mask);
}







Instruction* init_instab(int sz)
{
	int i;
	Instruction *instab = calloc(sz, sizeof(*instab));

	if(instab == NULL)
		return NULL;

	for (i = 0; i < sz; ++i)
	{
		instab[i].it_flag = OUT;
	}


	return instab;
}








void del_instab(Instruction *tab, int size)
{
	int i;


	if(tab == NULL)
		return;

	for (i = 0; i < size; ++i)
	{
		del_ins(tab + i);
	}

	free(tab);
}










int get_ins(word in, Instruction *out, Instruction dic[], int sz_dic) // retourne l'instruction en question s'il y a match, NULL sinon;
{
	int i=0;




	if(in == 0)
	{
		return 1;
	}
	
	//V2

	while (i<sz_dic) {

		if((in & dic[i].mask) == (dic[i].opcode & dic[i].mask)) {
			insclone(out, &(dic[i]));

			return 0;
		}

		i++;
	}

	return 1;
}



