#include "dic/fill_paramsCU.h"
#include "dic/instructionCU.h"
#include "elf/bitsCU.h"
#include "simul/segmentCU.h"
#include "dic/code_armCU.h"


#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include "dic/dic.h"




int main(int argc, char *argv[])
{
	CU_Suite *fill_suite, *bits_suite, *seg_suite, *ins_suite;


	/* initialisation */


	CU_basic_set_mode(CU_BRM_VERBOSE);

	if(CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();




	fill_suite = CU_add_suite("fill params suite", CU_init_fill_params_suite, CU_del_fill_params_suite);
	ins_suite = CU_add_suite("instruction suite", CU_init_ins_suite, CU_del_ins_suite);
	bits_suite = CU_add_suite("bits suite", NULL, NULL);
	seg_suite = CU_add_suite("segment suite", CU_init_seg_suite, CU_del_seg_suite);
	CU_Suite *arm_suite = CU_add_suite("code arm", NULL, NULL);









	CU_add_test(fill_suite, "parse_param", CU_parse_param);
	CU_add_test(fill_suite, "CU_parse_reglist", CU_parse_reglist);

	CU_add_test(bits_suite, "hwd_good_endianness, CU", CU_hwd_good_endianness);
	CU_add_test(bits_suite, "wrd_good_endianness, CU", CU_wrd_good_endianness);

	CU_add_test(seg_suite, "get_word", CU_get_word);
	CU_add_test(seg_suite, "get byte", CU_get_byte);

	CU_add_test(ins_suite, "cmp_opcode_sz", CU_cmp_ins);

	CU_add_test(arm_suite, "Sign Extend", CU_SignExtend);




	/* Exécution */

	CU_basic_run_suite(arm_suite);
	// CU_console_run_tests();





	CU_cleanup_registry();

	return 0;
}
