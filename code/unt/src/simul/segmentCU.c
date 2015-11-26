
#include "simul/segmentCU.h"
#include "inter/command.h"

#include <CUnit/CUnit.h>






int CU_init_seg_suite()
{
	int r;
	FILE *fo = fopen("pile.o", "r");



	if(fo == NULL)
	{
		ferror(fo);
	}


	r = load_elf_in_mem(fo, map_fix, 0x1000);

	if(r != 0)
		return r;

	fclose(fo);

	return 0;
}




int CU_del_seg_suite()
{
	del_map(map_fix);

	return 0;
}






void CU_get_word()
{
	CU_ASSERT(get_word(0x1000, map_fix) == 0x4ff00000);
}




void CU_get_byte()
{
	CU_ASSERT(get_byte(0x1004, map_fix) == 0x4f);
}
