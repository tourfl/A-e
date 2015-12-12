#include "dic/mov.h"


#include "dic/code_arm.h"
#include <string.h> // strcmp
#include <stdlib.h> // strtoul
#include <stdio.h> // sprintf

/**********************************************************************************************************/
/************************************************MOV_IMM***************************************************/
/**********************************************************************************************************/


int mov_imm_2 (Instruction ins, Emulator *emul) {


	int carry, setflags, i;
	word imm32;
	char reg_label[4];



	
	ZeroExtend_plgtab(&ins.imm);

	// if (strcmp (ins.encoding, 'T1') == 0 ) {
	// 	if ( mov_imm_T1 (ins, reg, &setflags, &imm32 , &carry) ) {
	// 		return 1;
	// 	}
	// }

	// else if (strcmp (ins.encoding, 'T2') == 0 ) {
	// 	if (mov_imm_T2 (ins, reg, &seflags, &imm32, &carry, &d)) {
	// 		return 1;
	// 	}
	// }

	// else if (strcmp (ins.encoding, 'T3') == 0 ) {
	// 	if (mov_imm_T3 (ins, reg, &setflags, &imm32, &d)) {
	// 		return 1;
	// 	}
	// }

	// else {
	// 	WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
	// 	return 1;
	// }

	// if (setflags) {
	// 	if (result & (1u << 31) ){
	// 		APSR.N = 1;
	// 	}
	// 	else {
	// 		APSR.N = 0;
	// 	}
	// 	APSR.Z = IsZeroBit(result);
	// 	APSR.C = carry;	
	// 	// APSR.V unchanged
	// }
	return 0;
}
 


//---------------------------------------------------------------------------------------------------------//


// int mov_imm_T1 (Instruction ins, Registre reg[NB_REG], int* setflags, int* imm32, int* carry){
	
// 	*setflags = !InITBlock() ;
// 	*imm32 = ZeroExtend (*imm32);
// 	*carry = ASPR.C;
// 	return 0;
// }


// int mov_imm_T2 (Instruction ins, Registre reg[NB_REG], int* setflags, int* imm32, int* carry, int* d) {
	
// 	if (ins.ext.value==1) {
// 		*setflags = 1;
// 	}
	
// 	(*imm32 , *carry) = ThumbExpandImm_C (*imm32 , 32);
// 	if (*d==13 || *d==14 || *d==15) {
// 		WARNING_MSG ("Accès non autorisé");
// 		return 1;
// 	}
// 	return 0;
// }
	
// int mov_imm_T3 (Instruction ins, Registre reg[NB_REG], int* setflags, int* imm32, int* d) {

// 	*setflags = 0;
// 	*imm32 = ZeroExtend (*imm32, 32);
// 	if (*d==13 || *d==14 || *d==15) {
// 		WARNING_MSG ("Accès non autorisé");
// 		return 1;
// 	}
// 	return 0; 
// }
