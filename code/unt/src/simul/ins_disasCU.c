#include "simul/ins_disasCU.h"

#include "simul/ins_disas.h"
#include "simul/dic.h"
#include "inter/command.h"
#include "simul/instruction.h"
#include <string.h>

#include <CUnit/CUnit.h>





// Il semble que l'utilisation des variables globales soit inévitable avec CUnit

static Dic *dic;
static word mot;






int CU_init_ins_disa_suite()
{
	dic = init_dic();
	mot = 0x4ff00801;

	return load_dic(dic);
}

int CU_del_ins_disa_suite()
{
	del_dic(dic);

	return 0;
}





void CUfind()
{
	Ins_disas *ins = init_ins();
	word in16 = mot >> 16;



	printf("\nin16: %04x; in32: %08x\n", in16, mot);


	CU_ASSERT(find(mot, ins, dic) == 4);

	decode(mot, ins);

	disp_insd(*ins);

	del_ins(ins);
}







void CUfind_and_disasm()
{
	Ins_disas *ins = init_ins();




	CU_ASSERT(find_and_decode(mot, ins, dic) == 4);

	disp_insd(*ins);

	del_ins(ins);
}
