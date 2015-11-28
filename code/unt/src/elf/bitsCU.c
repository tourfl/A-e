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

void CU_masklen()
{
	word mot = 0xffffffff, m2 = 1;
	int l = masklen(mot), l2 = masklen(m2);

	CU_ASSERT(l == 32);
	CU_ASSERT(l2 = 1);

	printf("l = %u et l2 = %u", l, l2);

}
