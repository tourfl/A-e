#include "dic/ins_disasCU.h"
#include "dic/instructionCU.h"
#include "elf/bitsCU.h"
#include "simul/segmentCU.h"


#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include "dic/dic.h"




int main(int argc, char *argv[])
{
	CU_Suite *id_suite, *bits_suite, *seg_suite, *ins_suite;
	CU_Test *test;




	if(CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	id_suite = CU_add_suite("instruction disa suite", CU_init_ins_disa_suite, CU_del_ins_disa_suite);
	ins_suite = CU_add_suite("instruction suite", CU_init_ins_suite, CU_del_ins_suite);
	bits_suite = CU_add_suite("bits suite", NULL, NULL);
	seg_suite = CU_add_suite("segment suite", CU_init_seg_suite, CU_del_seg_suite);

	if(id_suite == NULL || ins_suite == NULL || bits_suite == NULL || seg_suite == NULL)
		return CU_get_error();


	test = CU_add_test(id_suite, "parse_param", CU_parse_param);
	test = CU_add_test(id_suite, "find_and_disasm", CUfind_and_disasm);

	test = CU_add_test(bits_suite, "hwd_good_endianness, CU", CU_hwd_good_endianness);
	test = CU_add_test(bits_suite, "wrd_good_endianness, CU", CU_wrd_good_endianness);
	test = CU_add_test(bits_suite, "masklen", CU_masklen);
	test = CU_add_test(bits_suite, "mask_from_0", CU_mask_from_0);

	test = CU_add_test(seg_suite, "get_word", CU_get_word);
	test = CU_add_test(seg_suite, "get byte", CU_get_byte);

	test = CU_add_test(ins_suite, "cmp_opcode_sz", CU_cmp_ins);

	if(test == NULL)
		return CU_get_error();

	
	CU_basic_set_mode(CU_BRM_VERBOSE);

	CU_basic_run_suite(bits_suite);
	// CU_console_run_tests();

	CU_cleanup_registry();

	return 0;
}
