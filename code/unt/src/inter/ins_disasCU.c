#include "simul/ins_disasCU.h"

#include "simul/ins_disas.h"
#include "simul/dic.h"
#include "inter/command.h"
#include "simul/instruction.h"
#include <string.h>







CU_TestFunc CUfind()
{
	Ins_disas *ins = init_ins();
	Dic *dic = init_dic();
	word in32 = 0xF04F0108;
	word in16 = in32 >> 16;



	printf("\nin16: %04x; in32: %08x\n", in16, in32);
	load_dic(dic);


	CU_ASSERT(find(in32, ins, dic) == 4);

	disasm_ins(in32, ins);

	disp_insd(*ins);

	free(dic);

	return 0;
}







CU_TestFunc CUfind_and_disasm()
{
	Ins_disas *ins = init_ins();
	Dic *dic = init_dic();
	word in32 = 0xF04F0108;



	
	load_dic(dic);


	CU_ASSERT(find_and_disasm(in32, ins, dic) == 0);

	disp_insd(*ins);

	free(dic);

	return 0;
}
