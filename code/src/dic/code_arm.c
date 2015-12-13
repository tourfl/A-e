#include "dic/code_arm.h"

#include <string.h> // strcat
#include <stdlib.h> // strtoul
#include <stdio.h>
#include "inter/notify.h" // messages de contrôle





// concatène les valeurs et place le résultat à l'index 0

void ZeroExtend_plgtab (Plgtab *imm) 
{	
	uint u=0;
	int prev_size=0;

	if(imm->size == 0)
		return;



	for (int i = imm->size -1; i >= 0; i--)
	{
		u += imm->plages[i].value << prev_size;

		prev_size = imm->plages[i].end - imm->plages[i].start + 1;
	}

	free(imm->plages);
	imm->size = 1;

	imm->plages = calloc(1, sizeof(Plage));

	imm->plages->value = u;
}


void SignExtend_plgtab (Plgtab *imm) 
{
	uint u=0, b=0;
	int prev_size=0, size=0;



	for (int i = imm->size -1; i >= 0; i--)
	{
		u += imm->plages[i].value << prev_size;

		prev_size = imm->plages[i].end - imm->plages[i].start + 1;

		size += prev_size;
	}



	b = (1 << size) - 1;



	free(imm->plages);
	imm->size = 1;

	imm->plages = calloc(1, sizeof(Plage));

	// printf("u = %u\t b = %u\n", u, b);

	if(u > 1 << (size - 1))
		imm->plages->value = -(u ^ b);

	else
		imm->plages->value = u;
}



void BranchWritePC (vaddr32 adresse, Emulator *emul) {
	
	//adresse = adresse & ~(1u << 0) ;
	if(adresse & (1u << 1) ) {
		emul->reg[15] = adresse  ;
	}
	
	else {
		emul->reg[15]= adresse - 2;
	}
	printf ("pc après branchement : %lx\n" , emul->reg[15]);
}


	
void BXWritePC (vaddr32 a, Emulator *emul) {

	BranchWritePC (a, emul);

}

long AddWithCarry (long registre , long imm32 , int* carry , int* overflow) {


	unsigned long unsigned_sum;
	long signed_sum;
	long result;
	unsigned long uregistre , uimm32;
	uregistre = (unsigned long) registre;
	uimm32 = (unsigned long ) imm32;
	/*if (emul->reg[16] & (1u << 29)) {
		unsigned_sum = UInt32 (registre) + UInt32 (imm32) + 1;
	    	signed_sum   = SInt32 (registre) + SInt32 (imm32) + 1;
	}
	else {*/
		unsigned_sum = uregistre + uimm32 ;
	    	signed_sum   = registre + imm32 ;
	//}		
    	result = signed_sum  ;
	if (result == unsigned_sum ) {
		*carry = 0; 
	}
	else {
		*carry = 1;
	}
    	if (result == signed_sum ) {
		*overflow = 0;
	}	
	else {
		*overflow = 1;
	}
	return result;	


}






int IsZeroBit (long result) {
	
	if (!result) {
		return 0;
	}
	else {
		return 1;
	}

}





int BitCount (long a) {
	
	int i;
	int j=0;
	for (i=0 ; i<=31 ; i++) {
		if (a & (1u << i) ) {
			j = j + 1;
		}
	}
	return j;
}


int condition (int cond, Emulator* emul) {	

	if (cond == 0) {
		if (emul->reg[16] & (1u << 30) ) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (cond == 1) {
		if (emul->reg[16] & (1u << 30) ) {
			return 0;
		}
		else {
			return 1;
		}
  	}




	else if (cond == 2) {
		if (emul->reg[16] & (1u << 29) ) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (cond == 3) {
		if (emul->reg[16] & (1u << 29) ) {
			return 0;
		}
		else {
			return 1;
		}
	}



	else if (cond == 4) {
		if (emul->reg[16] & (1u << 31) ) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (cond == 5) {
		if (emul->reg[16] & (1u << 31) ) {
			return 0;
		}
		else {
			return 1;
		}
	}



	else if (cond == 6) {
		if (emul->reg[16] & (1u << 28) ) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (cond == 7) {
		if (emul->reg[16] & (1u << 28) ) {
			return 0;
		}
		else {
			return 1;
		}
	}



	else if (cond == 8) {
		if ((emul->reg[16] & (1u << 29) ) && !(emul->reg[16] & (1u << 30) ) ) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (cond == 9) {
		if (!(emul->reg[16] & (1u << 29) ) || (emul->reg[16] & (1u << 30) ) ) {
			return 1;
		}
		else {
			return 0;
		}

	}




	else if (cond == 10) {
		if ((emul->reg[16] & (1u << 31) ) == (emul->reg[16] & (1u << 28) ) ) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (cond == 11) {
		if ((emul->reg[16] & (1u << 31) ) == (emul->reg[16] & (1u << 28) ) ) {
			return 0;
		}
		else {
			return 1;
		}
	}



	else if (cond == 12) {
		if (!emul->reg[16] && (emul->reg[16] & (1u << 31) ) == (emul->reg[16] & (1u << 28) ) ) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (cond == 13) {
		if (emul->reg[16] || (emul->reg[16] & (1u << 31) ) != (emul->reg[16] & (1u << 28) ) ) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else 
		return 0;
}
