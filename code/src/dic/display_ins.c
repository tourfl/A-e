#include "dic/display_ins.h"

#include "inter/notify.h" // printf
#include "types.h" // Plgtab
#include <string.h> // strcat
#include "dic/code_arm.h"



void display(Instruction ins, int flag, Emulator *emul)
{
	if(flag == DECODED)
		ins.display_decoded(ins, emul); // les instructions ne s'affichent pas toujours de la même manière !

	else
		disp_not_decoded(ins);
}










// Pour une instruction extraite du dictionnaire

void disp_not_decoded(Instruction ins)
{
	printf("\n");
	disp_name(ins);
	printf("\n");
	// printf("  name_in: %s\tname_out: %s\n", ins.name_in, ins.name_out);
	// printf("  mask: %8x\topcode: %8x\n", ins.mask, ins.opcode);
	// printf("\n  reg ");
	// disp_plgtab(*(ins.reg));
	// printf("\n  imm ");
	// disp_plgtab(*(ins.imm));
	// printf("\n  ext ");
	// disp_plgtab(*(ins.ext));
}




void disp_name(Instruction ins)
{
	printf("%s", ins.name_in);
	// printf("(%s/T%u)", ins.commande, ins.encoding);
}






void disp_regs(Plgtab regs)
{
	int i;



	for(i = 0; i < regs.size; i++)
	{
		if(i != 0)
			printf(", ");

		disp_reg_name(regs.plages[i]);
	}
}





void disp_reg_name(Plage p)
{
	switch (p.value)
		{
			case 13: {
				printf("sp");
				break;
			}
			case 14: {
				printf("lr");
				break;
			}
			case 15: {
				printf("pc");
				break;
			}
			case 16: {
				printf("apsr");
				break;
			}
			default: {
				printf("r%u", p.value);			
				break;
			}
		}
}






void disp_imm(Plgtab imm)
{
	printf("#%i", imm.plages->value);
}




void disp_regs_and_imm(Instruction ins)
{
	if(ins.reg->size > 0)
		disp_regs(*(ins.reg));

	if(ins.reg->size > 0 && ins.imm->size > 0)
		printf(", ");

	if(ins.imm->size > 0)
		disp_imm(*(ins.imm));
}






// Pour une instruction désassemblée

void disp_default(Instruction ins, Emulator *emul)
{
	disp_name(ins);

	printf(" ");
	
	disp_regs_and_imm(ins);
}







void disp_sub_sp(Instruction ins, Emulator *emul)
{
	disp_name(ins);

	printf(" ");


	if(ins.reg->size > 0)
		disp_regs(*(ins.reg));
	else
		printf("sp");


	printf(", sp");

	if(ins.imm->size > 0)
	{
		printf(", ");
		disp_imm(*(ins.imm));
	}
}






void disp_pop_push(Instruction ins, Emulator *emul)
{

	disp_name(ins);
	printf(" ");

	printf("{");

	if(ins.reg->size > 0)
		disp_regs(*(ins.reg));

	printf("}");
}




void disp_ldr(Instruction ins, Emulator *emul)
{
	disp_name(ins);
	printf(" ");

	disp_reg_name(ins.reg->plages[0]);

	printf(", [");
	disp_reg_name(ins.reg->plages[1]);

	if(ins.imm->size > 0 && ins.imm->plages->value > 0)
	{
		printf(", ");
		disp_imm(*(ins.imm));
	}

	printf("]");

}




void disp_it(Instruction ins, Emulator *emul)
{
	char mask = (char) ins.ext->plages[0].value;
	int i, k, cond = ins.ext->plages[1].value;
	char cond0 = GET_BIT(cond, 0);
	char str_cond[15][3] = {{0}};

	strcpy(str_cond[0], "EQ");
	strcpy(str_cond[1], "NE");
	strcpy(str_cond[2], "HS");
	strcpy(str_cond[3], "LO");
	strcpy(str_cond[4], "MI");
	strcpy(str_cond[5], "PL");
	strcpy(str_cond[6], "VS");
	strcpy(str_cond[7], "VC");
	strcpy(str_cond[8], "HI");
	strcpy(str_cond[9], "LS");
	strcpy(str_cond[10], "GE");
	strcpy(str_cond[11], "LT");
	strcpy(str_cond[12], "GT");
	strcpy(str_cond[13], "LE");
	strcpy(str_cond[14], "AL");



	disp_name(ins);



	for (k = 0; k < 4; k++) // trouve le premier bit à 1
	{
		if(GET_BIT(mask, k) == (1 << k) )
			break;
	}



	for (i = 3; i > k; i--)
	{
		if( GET_BIT(mask, i) == (cond0 << i) )
			printf("T");
		else
			printf("E");
	}

	printf(" %s", str_cond[cond]);
}


