#include "dic/fill_paramsCU.h"

#include "dic/fill_params.h"
#include "dic/dic.h"
#include "dic/display_ins.h"
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
	int r1=0, r2=0;
	word mot = hwd_good_endianness(0x4e46); 
	// 0100 0110 0100 1110
	Plage p1 = {0, 2, 0};
	// 0000 0000 0000 0111 ; res = 6
	Plage p2 = {3, 6, 0};
	// 0000 0000 0111 1000 ; res = 1




	CU_ASSERT((r1 = parse_param(mot, p1)) == 6);
	CU_ASSERT((r2 = parse_param(mot, p2)) == 9);



	printf("\nr1 = %u \t r2 = %u \n", r1, r2);
}




void CU_parse_reglist()
{
	word m1 = hwd_good_endianness(0x01b5), m2 = wrd_good_endianness(0xbde80140);
	word m3 = 0xf9e700d9;
	Instruction *i1 = init_ins(), *i2 = init_ins();
	Instruction *i3 = init_ins();

	// plages i1 : 8 et 0 à 7
	// plages i2 : 15, 14 et 0 à 12



	if(get_ins16(m1, i1, dic) != 0)
		return;

	if(get_ins32(m2, i2, dic) != 0)
		return;

	CU_ASSERT(parse_reglist(m1, i1) == 2);
	CU_ASSERT(parse_reglist(m2, i2) == 2);

	if (decode(m3, i3, dic) != 1)
		display(*i3, DECODED);


	// valeurs prévues : i2->reg->plages[1].value = 15

	printf("here {r%u, r%u}", i2->reg->plages[0].value, i2->reg->plages[1].value);


}







