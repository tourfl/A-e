#include "simul/ins_disasCU.h"
#include "elf/bitsCU.h"
#include "simul/segmentCU.h"


#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include "simul/dic.h"




int main(int argc, char *argv[])
{
	CU_Suite *id_suite, *bits_suite, *seg_suite;
	CU_Test *test;




	if(CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	id_suite = CU_add_suite("instruction suite", CU_init_ins_disa_suite, CU_del_ins_disa_suite);
	bits_suite = CU_add_suite("bits suite", NULL, NULL);
	seg_suite = CU_add_suite("segment suite", CU_init_seg_suite, CU_del_seg_suite);

	if(id_suite == NULL || bits_suite == NULL || seg_suite == NULL)
		return CU_get_error();

	test = CU_add_test(id_suite, "find", CUfind);
	test = CU_add_test(id_suite, "find_and_disasm", CUfind_and_disasm);

	test = CU_add_test(bits_suite, "hwd_good_endianness, CU", CU_hwd_good_endianness);
	test = CU_add_test(bits_suite, "wrd_good_endianness, CU", CU_wrd_good_endianness);

	test = CU_add_test(seg_suite, "get_word", CU_get_word);
	test = CU_add_test(seg_suite, "get byte", CU_get_byte);

	if(test == NULL)
		return CU_get_error();

	
	CU_basic_set_mode(CU_BRM_VERBOSE);

	CU_basic_run_suite(id_suite);
	// CU_console_run_tests();

	CU_cleanup_registry();

	return 0;
}
