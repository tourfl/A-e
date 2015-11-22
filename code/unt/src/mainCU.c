#include "simul/ins_disasCU.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include "simul/dic.h"







int main(int argc, char *argv[])
{
	CU_Suite *suite;
	CU_Test *test;




	if(CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	suite = CU_add_suite("test suite", NULL, NULL);

	if(suite == NULL)
		return CU_get_error();

	test = CU_add_test(suite, "find", CUfind);
	test = CU_add_test(suite, "find_and_disasm", CUfind_and_disasm);

	if(test == NULL)
		return CU_get_error();

	
	CU_basic_set_mode(CU_BRM_VERBOSE);

	CU_basic_run_suite(suite);
	// CU_console_run_tests();

	CU_cleanup_registry();

	return 0;
}
