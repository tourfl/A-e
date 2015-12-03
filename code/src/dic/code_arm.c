#include "dic/code_arm.h"

#include <string.h> // strcat
#include <stdlib.h> // strtoul
#include <stdio.h>




word ZeroExtend (Plgtab imm) {
	
	char imm_str[33] = {0};
	int i;



	for(i = 0; i < imm.size; i++)
		strcat(imm_str, int_to_bin(imm.plages[i].value, imm.plages[i].end - imm.plages[i].start));

	return strtoul(imm_str, NULL, 2);
}



int SignExtend (Plgtab imm) 
{
	uint u_imm=0, b=0;
	int prev_size=0, size=0;



	for (int i = imm.size -1; i >= 0; i--)
	{
		u_imm += imm.plages[i].value << prev_size;

		prev_size = imm.plages[i].end - imm.plages[i].start + 1;

		size += prev_size;
	}



	b = (1 << size) - 1;



	// printf("u_imm = %u\t b = %u\n", u_imm, b);

	if(u_imm > 1 << (size - 1))
		return -(u_imm ^ b);

	else
		return u_imm;
}
