#include "dic/instruction.h"

#include <stdlib.h> // free
#include <string.h> // strtok
#include "inter/notify.h" // messages de contrôle
#include "types.h" // plgtab

// Pour les fonctions pointées

#include "dic/mov.h"



Instruction* init_ins() // pour éviter les bugs lors de la désallocation
{
	Instruction *ins = calloc(1, sizeof(Instruction));

	if(ins == NULL)
		return NULL;

	// Paramètres :

	ins->reg = init_plgtab();
	ins->imm = init_plgtab();
	ins->ext = init_plgtab();

	return ins;
}






void del_ins(Instruction *ins) // Les éléments alloués sont libérés
{
	free(ins->commande);
	free(ins->encoding);
	free(ins->name_in);
	free(ins->name_out);


	// Paramètres :

	del_plgtab(ins->reg);
	del_plgtab(ins->imm);
	del_plgtab(ins->ext);
}








void insclone(Instruction *dest, Instruction *src)
{
	dest->commande = strclone(src->commande);
	dest->encoding = strclone(src->encoding);
	dest->name_in = strclone(src->name_in);
	dest->name_out = strclone(src->name_out);
	dest->mask = src->mask;
	dest->opcode = src->opcode;

	dest->reg = plgtabclone(src->reg);
	dest->imm = plgtabclone(src->imm);
	dest->ext = plgtabclone(src->ext);

	dest->run = src->run;
}







int load_ins(Instruction *ins, char *chaine)
{
	int r=0;



	r = load_from_string(ins, chaine);

	if(r != 0)
		return r;

	ins->run = get_run_pft(ins->commande);

	// if(ins->run_pft == NULL)
	// 	return 13; // cf which_error in src/inter/notify.c


	return 0;

}








int load_from_string(Instruction *ins, char *chaine)
{
	char *token = NULL, *str = NULL, *saveptr = NULL;
	int i, p = 0;




	// V4


	for (i = 0, str = chaine; i < 6; ++i, str = NULL)
	{
		token = strtok_r(str, " ", &saveptr);

		// DISP_TOKEN(i, token);

		if(token == NULL) // Comprend le cas chaine == NULL
			break;

		switch (i)
		{
			case 0: {
				Str str_tab[2]; // tableau de 2 char*

				p = to_strtab(token, str_tab);

				ins->commande = str_tab[0]; // alloué dynamiquement
				ins->encoding = str_tab[1];

				break;
			}

			case 1: {
				Str str_tab[2]; // tableau de 2 char*

				p = to_strtab(token, str_tab);

				ins->name_in = str_tab[0];
				ins->name_out = str_tab[1]; // juste un \0 s'il n'est pas dans la définition de l'instruction
				break;
			}
			
			case 2: {
				word wrd_tab[2];

				p = to_wrdtab(token, wrd_tab);

				ins->mask = wrd_tab[0]; // On suppose qu'il n'y a pas d'erreur
				ins->opcode = wrd_tab[1];
				break;
			}
			
			case 3 :
			p = to_plgtab(token, ins->reg);
			break;
			
			case 4 :
			p = to_plgtab(token, ins->imm);
			break;
			
			case 5 :
			p = to_plgtab(token, ins->ext);
			break;
		}

		if(p != 0 && p != 3)
			return p;
	}

	return 0;
}






Run_pft get_run_pft(char* mnemo)
{
	Run_pft pft = NULL;

	if(strcmp(mnemo, "mov_imm") == 0)
	{
		pft = mov_imm;
	}

	return pft;
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
		instab[i].reg = init_plgtab();
		instab[i].imm = init_plgtab();
		instab[i].ext = init_plgtab();
	}


	return instab;
}






// Pour une instruction extraite du dictionnaire

void disp_ins(Instruction ins)
{
	printf("\n  command: %s\tencoding: %s\n", ins.commande, ins.encoding);
	// printf("  name_in: %s\tname_out: %s\n", ins.name_in, ins.name_out);
	// printf("  mask: %8x\topcode: %8x\n", ins.mask, ins.opcode);
	// printf("\n  reg ");
	// disp_plgtab(*(ins.reg));
	// printf("\n  imm ");
	// disp_plgtab(*(ins.imm));
	// printf("\n  ext ");
	// disp_plgtab(*(ins.ext));
}



