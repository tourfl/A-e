#include "dic/code_armCU.h"

#include "dic/code_arm.h"
#include <CUnit/CUnit.h>

#include <stdlib.h>





int CU_init_arm()
{
	return 0;
}






int CU_del_arm()
{
	return 0;
}




void CU_ZeroExtend()
{
	
}





void CU_SignExtend()
{
	int r=0;
	Plgtab *imm = init_plgtab();
	Plage p = {0, 10, 2041};

	imm->plages = &p;
	imm->size = 1;


	CU_ASSERT((r = SignExtend(*imm)) == -6);

	printf("r = %i\n", r);

}
