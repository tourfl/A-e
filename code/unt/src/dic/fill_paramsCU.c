#include "dic/fill_paramsCU.h"

#include "dic/fill_params.h"
#include "dic/dic.h"
#include "inter/command.h"
#include "dic/instruction.h"
#include <string.h>

#include <CUnit/CUnit.h>





// Il semble que l'utilisation des variables globales soit inévitable avec CUnit

static Dic *dic;
static word mot;






int CU_init_fill_params_suite()
{
	dic = init_dic();
	mot = 0x4ff00801;

	return load_dic(dic);
}

int CU_del_fill_params_suite()
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




void CU_parse_reglist()
{
	word m1 = hwd_good_endianness(0x01b5), m2 = wrd_good_endianness(0xbde80140);
	Instruction *i1 = init_ins(), *i2 = init_ins();





	if(get_ins16(m1, i1, dic) != 0)
		return;

	if(get_ins32(m2, i2, dic) != 0)
		return;

	CU_ASSERT(parse_reglist(m1, i1) == 2);
	CU_ASSERT(parse_reglist(m2, i2) == 2);

	printf("here {r%u, r%u}", i2->reg->plages[0].value, i2->reg->plages[1].value);


}







