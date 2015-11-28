#include "elf/bitsCU.h"

#include "elf/bits.h"

#include <CUnit/CUnit.h>
#include <stdlib.h>	






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

void CU_mask_from_0()
{
	CU_ASSERT(sum_2k(3) == 15);
	CU_ASSERT(mask_from_0(7) == 255);

	CU_ASSERT(mask_from_0(6) - mask_from_0(3) == strtoul("1110000", NULL, 2));
}
