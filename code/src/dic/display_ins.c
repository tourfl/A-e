#include "dic/display_ins.h"

#include "inter/notify.h" // printf
#include "types.h" // Plgtab
#include <string.h> // strcat
#include "dic/code_arm.h"



void display(Instruction ins, int flag)
{
	if(flag == DECODED)
		ins.display_decoded(ins); // les instructions ne s'affichent pas toujours de la même manière !

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
	printf("(%s/T%u)", ins.commande, ins.encoding);
	printf(" ");
}






void disp_regs(Plgtab regs)
{
	int i;



	for(i = 0; i < regs.size; i++)
	{
		if(i != 0)
			printf(", ");

		switch (regs.plages[i].value)
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
				printf("r%u", regs.plages[i].value);			
				break;
			}
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

void disp_default(Instruction ins)
{
	printf("\n");
	disp_name(ins);
	
	disp_regs_and_imm(ins);
}







void disp_sub_sp(Instruction ins)
{
	printf("\n");

	disp_name(ins);


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






void disp_pop_push(Instruction ins)
{

	printf("\n");

	disp_name(ins);

	printf("{");

	if(ins.reg->size > 0)
		disp_regs(*(ins.reg));

	printf("}");
}

