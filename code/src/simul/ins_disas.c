#include "simul/ins_disas.h"
#include <string.h> // strcat






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






int get_ins32(word in, Instruction *out, Dic *dic) {
	return get_ins(in, out, dic->ins32, dic->sz32);
}
int get_ins16(word in, Instruction *out, Dic *dic) {
	return get_ins(in, out, dic->ins16, dic->sz16);
}





void cpy_ins(Ins_disas *dest, Instruction *src)
{
	dest->commande = src->commande;
	dest->encoding = src->encoding;
	dest->name_in = src->name_in;
	dest->name_out = src->name_out;
	dest->opcode = src->opcode;

	dest->reg = src->reg;
	dest->imm = src->imm;
	dest->ext = src->ext;
}