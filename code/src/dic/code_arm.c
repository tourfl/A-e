#include "dic/code_arm.h"

#include <string.h> // strcat
#include <stdlib.h> // strtoul




word ZeroExtend (Plgtab imm) {
	
	char imm_str[33] = {0};
	int i;



	for(i = 0; i < imm.size; i++)
		strcat(imm_str, int_to_bin(imm.plages[i].value, imm.plages[i].end - imm.plages[i].start));

	return strtoul(imm_str, NULL, 2);
}
