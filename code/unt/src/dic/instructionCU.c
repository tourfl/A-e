#include "dic/instructionCU.h"

#include "dic/instruction.h"

#include <CUnit/CUnit.h>




static Dic *dic;


int CU_init_ins_suite()
{
	dic = init_dic();

	return load_dic(dic);
}

int CU_del_ins_suite()
{
	del_dic(dic);

	return 0;
}





void CU_cmp_ins()
{

	CU_ASSERT(cmp_ins(dic->ins16, dic->ins16) == 0);
	CU_ASSERT(cmp_ins(dic->ins16, dic->ins16+18) < 0);
	CU_ASSERT(cmp_ins(dic->ins16+18, dic->ins16) > 0);
}
