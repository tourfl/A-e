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


void CU_change_endianness()
{
	byte tab[] = {0xaa, 0xf0, 0x45, 0x32};

	change_endianness(tab, 4, LITTLE_E, BIG_E);

	printf("tab: %2x %2x %2x %2x\n", tab[0], tab[1], tab[2], tab[3]);


}



void CU_change_word_endianness()
{
	word mot = 0xf0f40108;

	change_word_endianness(&mot, LITTLE_E_ALIGNED, LITTLE_E);

	printf("mot = %08x\n", mot);


}
