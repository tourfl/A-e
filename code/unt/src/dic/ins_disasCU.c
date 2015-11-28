#include "dic/ins_disasCU.h"

#include "dic/ins_disas.h"
#include "dic/dic.h"
#include "inter/command.h"
#include "dic/instruction.h"
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




void CU_parse_param()
{
	word mot = 0x0a46;
	Plage p1 = {0, 2, 0};
	Plage p2 = {3, 6, 0};



	mot = hwd_good_endianness(mot);

	CU_ASSERT(parse_param(mot, p1) == 2);
	CU_ASSERT(parse_param(mot, p2) == 1);
}







void CUfind_and_disasm()
{
	Ins_disas *ins = init_ins();




	CU_ASSERT(decode(mot, ins, dic) == 4);

	disp_insd(*ins);

	del_ins(ins);
}
