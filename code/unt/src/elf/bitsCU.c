#include "elf/bitsCU.h"

#include "elf/bits.h"

#include <CUnit/CUnit.h>






void CU_hwd_good_endianness()
{
	Half_word in = 0xf0f4;


	CU_ASSERT(hwd_good_endianness(in) == 0xf4f0);
}



void CU_wrd_good_endianness()
{
	word in = 0xf0f40108;




	CU_ASSERT(wrd_good_endianness(in) == 0xf4f00801);
}
