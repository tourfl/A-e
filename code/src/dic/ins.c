#include "dic/display_ins.h" // display, inclut instruction.h
#include <stdio.h>
#include <stdlib.h> // free
#include <string.h> // strtok
#include "inter/notify.h" // messages de contrôle
#include "types.h" // plgtab
#include "inter/interpreteur.h" // string_standardise
#include "emulator.h"
// Pour les fonctions pointées

#include "dic/ins.h"

  

/**********************************************************************************************************/
/************************************************MUL***************************************************/
/**********************************************************************************************************/



int mul (Instruction ins, Emulator* emul ) {

	int d,n,m;
	int setflags; 
	long result, operand1, operand2;
	d = ins.reg->plages[0].value;
	n = ins.reg->plages[1].value;

	if (ins.encoding == 1 ) {
		if (mul_T1 (ins, &d, &m, &setflags)) {
			return 1;
		}
	}

	else if (ins.encoding == 2) {
		if ( mul_T2 (ins, &d, &n, &m, &setflags)) {
			return 1;
		}
	}
	
	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}
	

	operand1 = emul->reg[n];
	operand2 = emul->reg[m];
	result = operand1 * operand2;
	result = result & 0xFFFFFFE;
	emul->reg[d] = emul->reg[d] & 0x00000001;
	emul->reg[d] = result + emul->reg[d];
	if (setflags) {
		emul->reg[16] = emul->reg[16] & 0x7FFFFFFF;
		if (result & (1u << 31) ){			
			emul->reg[16] = emul->reg[16] | (1u << 31) ; // APSR.N = 1;
		}

		//APSR.Z = IsZeroBit (result);
		
		emul->reg[16] = emul->reg[16] & 0xBFFFFFFF;
			
		if (IsZeroBit(result)) { 
			emul->reg[16] = emul->reg[16] | (1u << 30)  ; 
		}
	
		// APSR.V & APSR.C unchanged
	
	}
	return 0;
}


//---------------------------------------------------------------------------------------------------------//


int mul_T1 (Instruction ins,int* d,int* m, int* setflags){

	*m = *d;
	setflags = 0; //!InITBlock();
	return 0;
}


int mul_T2 (Instruction ins, int* d, int* n, int* m, int* setflags){

	*setflags = 0;
	if ( (*n==13 || *n==14 || *n==15) || (*d==13 || *d==14 || *d==15) || (*m==13 || *m==14 || *m==15) ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
}


//---------------------------------------------------------------------------------------------------------//



/**********************************************************************************************************/
/************************************************MOVT***************************************************/
/**********************************************************************************************************/

int movt (Instruction ins , Emulator* emul) {
	

	int d;
	int imm16 = 0;
	d = ins.reg->plages[0].value;
	imm16 = ins.imm->plages[0].value;
	
	if (ins.encoding == 1 ) {
		if (movt_T1 (ins, &d)) {
			return 1;
		}
	}

	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}
	// imm16 = ZeroExtend (imm16) ; // Pas besoin, il est déjà fait !
	emul->reg[d] = emul->reg[d] & 0x0000FFFF;
	emul->reg[d] = emul->reg[d] + imm16;
	// R[d]<0:15> reste inchangé
	return 0;
}

//---------------------------------------------------------------------------------------------------------//

int movt_T1 (Instruction ins, int* d){

	if (*d==13 || *d==14 || *d==15) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}	
	return 0;
}


//---------------------------------------------------------------------------------------------------------//


/**********************************************************************************************************/
/*******************************************ADD_IMM********************************************************/
/**********************************************************************************************************/

int add_imm (Instruction ins, Emulator* emul) {


	int n,d;
	int setflags, imm32, carry, overflow;
	long result;
	
	n = ins.reg->plages[0].value;
	d = ins.reg->plages[1].value;
	
	if (ins.encoding == 1 ) {
		if( add_imm_T1 (ins,  &setflags)) {
			return 1;
		}
	}

	else if (ins.encoding == 2 ) {
		if ( add_imm_T2 (ins,  &setflags)) {
			return 1;
		}
	}

	else if (ins.encoding == 3 ) {
		if ( add_imm_T3 (ins,  &setflags, &d, &n) ) {
			return 1;
		}
	}

	else if (ins.encoding == 4 ) {
		if ( add_imm_T4 (ins,  &setflags, &d, &n) ) {
			return 1;
		}
	}

	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}
	
		
	imm32 = ins.imm->plages[0].value;
	result = AddWithCarry (emul->reg[n] , imm32, &carry , &overflow);
	emul->reg[d] = result;
	if (setflags) {

		emul->reg[16] = emul->reg[16] & 0x7FFFFFFF;
		if (result & (1u << 31) ){
			emul->reg[16] = emul->reg[16] | (1u << 31) ; // APSR.N = 1;
		}

		//APSR.Z = IsZeroBit (result);
	
		emul->reg[16] = emul->reg[16] & 0xBFFFFFFF; 
		if (IsZeroBit(result)) { 
			emul->reg[16] = emul->reg[16] | (1u << 30); 
		}

		//APSR.C = carry;

		emul->reg[16] = emul->reg[16] & 0xDFFFFFFF;
		if (carry) { 
			 
			emul->reg[16] = emul->reg[16] | (1u << 29); 
		}

		//APSR.V = overflow;
		
		emul->reg[16] = emul->reg[16] & 0xEFFFFFFF;
		if (overflow) {  
			emul->reg[16] = emul->reg[16] | (1u << 28); 
		}
		
	}
	return 0;
}





//------------------------------------------------------------------------------------------------------//





int add_imm_T1(Instruction ins,  int* setflags) {
		
	*setflags =  0;//!InITBlock(); 
	return 0;
}


int add_imm_T2(Instruction ins,  int* setflags) {
	
	*setflags = 0;//!InITBlock(); 
	return 0;
}


int add_imm_T3(Instruction ins,  int* setflags ,  int* n, int* d) {
	

	if ( *d == 15 && ins.ext->plages[0].value == 1 ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	} 

	if ( *n == 13 ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}

	*setflags = 1; 
	

	if (*d==13 || (*d==15 && ins.ext->plages[0].value == 0) || *n==15 ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;

}


int add_imm_T4(Instruction ins,  int* setflags ,  int* n, int* d ) {

	if (*n == 15) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}


	if (*n == 13) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}

	*setflags = 0;
	
	if ( *d == 13 || *d == 14 || *d == 15) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
	
}


//---------------------------------------------------------------------------------------------------------//




/**********************************************************************************************************/
/************************************************CMP_IMM***************************************************/
/**********************************************************************************************************/

int cmp_imm (Instruction ins, Emulator* emul) {

	int imm32, n;
	int carry, overflow;
	long result;
	n = ins.reg->plages[0].value;
	imm32 =ins.imm->plages[0].value;
	
	if (ins.encoding == 1 ) {
		if ( cmp_imm_T1 (ins)) {
			return 1;
		}
	}
	else if (ins.encoding == 2 ) {
		if ( cmp_imm_T2 (ins, &n) ) {
			return 1;
		}
	}

	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}
	
	
	result = AddWithCarry(emul->reg[n] , ~(imm32) , &carry, &overflow);	
	
	emul->reg[16] = emul->reg[16] & 0x7FFFFFFF;
	if (result & (1u << 31) ){
		emul->reg[16] = emul->reg[16] | (1u << 31); // APSR.N = 1;
	}

	//APSR.Z = IsZeroBit (result);
	
	emul->reg[16] = emul->reg[16] & 0xBFFFFFFF;
	if (IsZeroBit(result)) {  
		emul->reg[16] = emul->reg[16] | (1u << 30); 
	}

	//APSR.C = carry;

	emul->reg[16] = emul->reg[16] & 0xDFFFFFFF;
	if (carry) {  
		emul->reg[16] = emul->reg[16] | (1u << 29); 
	}

	//APSR.V = overflow;

	emul->reg[16] = emul->reg[16] & 0xEFFFFFFF; 
	if (overflow) { 
		emul->reg[16] = emul->reg[16] | (1u << 28); 
	}

	return 0;
}



//---------------------------------------------------------------------------------------------------------//



int cmp_imm_T1 (Instruction ins){
		

	return 0;
	
}



int cmp_imm_T2 (Instruction ins, int* n){
		

	if (*n==15) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
	
}


//---------------------------------------------------------------------------------------------------------//


/**********************************************************************************************************/
/************************************************SUB_IMM***************************************************/
/**********************************************************************************************************/


int sub_imm (Instruction ins, Emulator* emul) {
	

	
	int n,d;
	int setflags, imm32, carry, overflow;
	long result;
	
	n = ins.reg->plages[0].value;
	d = ins.reg->plages[1].value;
	
	if (ins.encoding == 1 ) {
		if (sub_imm_T1 (ins,  &setflags)) {
			return 1;
		}
	}

	else if (ins.encoding == 2 ) {
		if (sub_imm_T2 (ins,  &setflags)) {
			return 1;
		}
	}

	else if (ins.encoding == 3 ) {
		if (sub_imm_T3 (ins,  &setflags, &d, &n)) {
			return 1;
		}
	}

	else if (ins.encoding == 4 ) {
		if (sub_imm_T4 (ins,  &setflags, &d, &n)) {
			return 1;
		}
	}

	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}

	imm32 = ins.imm->plages[0].value;
	result = AddWithCarry (emul->reg[n] , ~imm32, &carry, &overflow);
	emul->reg[d] = result;
	if (setflags) {

		emul->reg[16] = emul->reg[16] & 0x7FFFFFFF;
		if (result & (1u << 31) ){
			
			emul->reg[16] = emul->reg[16] | (1u << 31); // APSR.N = 1;
		}

		//APSR.Z = IsZeroBit (result);

		emul->reg[16] = emul->reg[16] & 0xBFFFFFFF;
		if (IsZeroBit(result)) {  
			emul->reg[16] = emul->reg[16] | (1u << 30); 
		}

		//APSR.C = carry;

		emul->reg[16] = emul->reg[16] & 0xDFFFFFFF;
		if (carry) {  
			emul->reg[16] = emul->reg[16] | (1u << 29); 
		}

		//APSR.V = overflow;

		emul->reg[16] = emul->reg[16] & 0xEFFFFFFF;
		if (overflow) { 			 
			emul->reg[16] = emul->reg[16] | (1u << 28);
		}

	}
	return 0;
}





//------------------------------------------------------------------------------------------------------//





int sub_imm_T1(Instruction ins,  int* setflags) {
		
	*setflags = 0 ; //!InITBlock();
	return 0;
	
}


int sub_imm_T2(Instruction ins,  int* setflags) {
	
	*setflags = 0; //!InITBlock(); 
	return 0;

}


int sub_imm_T3(Instruction ins,  int* setflags , int* n, int* d) {
	

	if ( *d == 15 && ins.ext->plages[0].value == 1 ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	} 

	if ( *n == 13 ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}

	if (ins.ext->plages[0].value == 1 ) {
		*setflags = 1;
	} 
	

	if (*d==13 || (*d==15 && ins.ext->plages[0].value == 0) || *n==15 ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;

}


int sub_imm_T4(Instruction ins,  int* setflags, int* n, int* d ) {

	if (*n == 15) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}

	if (*n == 13) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}

	*setflags = 0;
	
	
	if ( *d == 13 || *d == 14 || *d == 15) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------------------------------//




/**********************************************************************************************************/
/************************************************MOV_IMM***************************************************/
/**********************************************************************************************************/


int mov_imm (Instruction ins, Emulator* emul) {


	int d;
	int carry;
	long result;
	int setflags;
	int imm32;
	d = ins.reg->plages[0].value;
	imm32 = ins.imm->plages[0].value;

	if (ins.encoding == 1 ) {
		if ( mov_imm_T1 (ins, emul, &setflags, &imm32 , &carry) ) {
			return 1;
		}
	}

	else if (ins.encoding == 2 ) {
		if (mov_imm_T2 (ins, &setflags, &imm32, &carry, &d)) {
			return 1;
		}
	}

	else if (ins.encoding == 3 ) {
		if (mov_imm_T3 (ins, &setflags, &imm32, &d)) {
			return 1;
		}
	}

	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}

	
	result = imm32;
	emul->reg[d] = result;
	if (setflags) {
		emul->reg[16] = emul->reg[16] & 0x7FFFFFFF;
		if (result & (1u << 31) ){
			emul->reg[16] = emul->reg[16] | (1u << 31); // APSR.N = 1;
		}

		//APSR.Z = IsZeroBit (result);

		emul->reg[16] = emul->reg[16] & 0xBFFFFFFF;
		if (IsZeroBit(result)) {  
			emul->reg[16] = emul->reg[16] | (1u << 30);
		}

		//APSR.C = carry;

		emul->reg[16] = emul->reg[16] & 0xDFFFFFFF;
		if (carry) {  
			emul->reg[16] = emul->reg[16] | (1u << 29);
		}
		
		// APSR.V unchanged
	}
	return 0;
}
 


//---------------------------------------------------------------------------------------------------------//


int mov_imm_T1 (Instruction ins, Emulator* emul, int* setflags, int* imm32, int* carry){
	
	*setflags = 0; //!InITBlock() ;
	if (emul->reg[16] & (1u << 29) ) {
		*carry = 1;
	}
	else {
		*carry = 0;
	}
	return 0;
}


int mov_imm_T2 (Instruction ins, int* setflags, int* imm32, int* carry, int* d) {
	
	if (ins.ext->plages[0].value==1) {
		*setflags = 1;
	}
	
	//(imm32 , carry) = ThumbExpandImm_C (imm32 , 32);
	if (*d==13 || *d==14 || *d==15) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
}
	
int mov_imm_T3 (Instruction ins, int* setflags, int* imm32, int* d) {

	*setflags = 0;
	if (*d==13 || *d==14 || *d==15) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0; 
}

//---------------------------------------------------------------------------------------------------------//




/**********************************************************************************************************/
/************************************************MOV_REG***************************************************/
/**********************************************************************************************************/


int mov_reg (Instruction ins, Emulator* emul) {
	
	
	int d, m;
	int setflags = 0;
	long result;
	d = ins.reg->plages[0].value;
	m = ins.reg->plages[1].value;

	if (ins.encoding == 1 ) {
		if ( mov_reg_T1 (ins,  &setflags, &d)) {
			return 1;
		}
	}

	else if (ins.encoding == 2 ) {
		if (mov_reg_T2 (ins, &setflags)) {
			return 1;
		}
	}

	else if (ins.encoding == 3 ) {
		if (mov_reg_T3 (ins, &setflags, &d, &m)) {
			return 1;
		}
	}

	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}
	
	result = emul->reg[m];
	if (d==15) {
		BranchWritePC (result, emul); //setflags is always false here
	}
	else {
		emul->reg[d] = result;
		if (setflags) {

			emul->reg[16] = emul->reg[16] & 0x7FFFFFFF;
			if (result & (1u << 31) ){
				emul->reg[16] = emul->reg[16] | (1u << 31); // APSR.N = 1;
			}

			//APSR.Z = IsZeroBit (result);
	
			emul->reg[16] = emul->reg[16] & 0xBFFFFFFF;
			if (IsZeroBit(result)) {  
				emul->reg[16] = emul->reg[16] | (1u << 30); 
			}

			// APSR.C & APSR.V unchanged
		}
	}
	return 0;
}



//---------------------------------------------------------------------------------------------------------//


int mov_reg_T1 (Instruction ins, int* setflags,int* d){


	// (*d) = ins.ext->ue:(*d.va.	
	if (ins.ext->plages[0].value) {
		*d = *d + 8;
	}
	*setflags = 0;
	if (*d==15 /*&& InITBlock() && !LastInITBlock()*/ ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
}

int mov_reg_T2 (Instruction ins, int* setflags){

	*setflags = 1;
	/*if (InITBlock() ){
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}*/
	return 0;
}

int mov_reg_T3 (Instruction ins, int* setflags,int* d, int* m){

	
	if (ins.ext->plages[0].value) {
		*setflags = 1;
	}
	
	if (*setflags && ( (*d==13 || *d==14 || *d==15 ) || (*m==13 || *m==14 || *m==15) )) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}	
	
	if (!(*setflags) && (*d==15 || *m== 15 || (*d==13 && *m==13) ) ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
}



//---------------------------------------------------------------------------------------------------------//



/**********************************************************************************************************/
/************************************************POP***************************************************/
/**********************************************************************************************************/



int pop (Instruction ins, Emulator* emul) {
	

	int t;
	int i;
	long registers;
	vaddr32 adress;


	if (ins.encoding == 1 ) {
		if (pop_T1 (ins, &registers)) {
			return 1;
		}
	}

	else if (ins.encoding == 2 ) {
		if (pop_T2 (ins, &registers)) {
			return 1;
		}
	}
	
	else if (ins.encoding == 3 ) {
		if (pop_T3 (ins, &registers, &t)) {
			return 1;
		}
	}
	
	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}

	adress = emul->reg[13];
	for (i = 0 ; i<=14 ; i++) {
		if (registers & (1u << i )) {
			emul->reg[i] = adress; //MemA(adress, 4);
			adress = adress +4;
		}
	}
	if (registers & (1u << 15 ) ) {
		BranchWritePC(adress, emul);//MemA(adress, 4));
	}
	emul->reg[13] = emul->reg[13] + 4*BitCount(registers);
	return 0;
}

//---------------------------------------------------------------------------------------------------------//



int pop_T1 (Instruction ins, long* registers) {

	//*registers = ins.ext->ue:'00.va.000':ins.reg->ue ;
	*registers = ins.reg->plages[0].value;
	if (ins.ext->plages[0].value) {
		*registers = *registers | (1u << 12);
	} 
	if (BitCount(*registers)<1){
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
}

int pop_T2 (Instruction ins, long* registers){

	//*registers = ins.ext->value:[0].s.ext->value:[1].s.reg->ue ;
	*registers = ins.reg->plages[0].value;
	if (ins.ext->plages[1].value) {
		*registers = *registers | (1u << 5);
	} 
	if (ins.ext->plages[0].value) {
		*registers = *registers | (1u << 6);
	} 
	if (BitCount (*registers)<2 || (ins.ext->plages[0].value == 1 && ins.ext->plages[1].value == 1 ) ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}

	if (*registers & (1u << 15) /* && InITBlock() && !LastInITBlock ()*/) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
}

int pop_T3 (Instruction ins , long* registers , int* t){

	*t = ins.reg->plages[0].value;
	*registers = 0;
	unsigned int d;
	int s;
	s = *t;
	d = (unsigned int) s;
	// *registers<t> = 1;
	*registers = *registers | (1u << d);
	if (*t==13 || *t==15 /*&& InITBlock() && !LastInITBlock()*/  ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
} 

//---------------------------------------------------------------------------------------------------------//						 



/**********************************************************************************************************/
/************************************************LDR_IMM***************************************************/
/**********************************************************************************************************/

int ldr_imm (Instruction ins, Emulator* emul) {
	

	int t,n, imm32;
	long offset_addr, address, data;
	int index, wback, add;
	imm32 = ins.imm->plages[0].value;
	
	if (ins.encoding == 1 ) {
		if (ldr_imm_T1 (ins, &imm32, &index, &add, &wback, &n, &t)) {
			return 1;
		}
	}

	else if (ins.encoding == 2 ) {
		if (ldr_imm_T2 (ins, &imm32, &index, &add, &wback, &n, &t)) {
			return 1;
		}
	}

	else if (ins.encoding == 3 ) {
		if ( ldr_imm_T3 (ins, &imm32, &index, &add, &wback, &n, &t)) {
			return 1;
		}
	}

	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}
	
	if (add) {
		offset_addr = emul->reg[n] + imm32;
	}
	else {
		offset_addr = emul->reg[n] - imm32;
	}
	
	if (index) {
		address = offset_addr;
	}
	else {
		address = emul->reg[n];
	} 
	
	data = address;
	
	if (wback) {
		emul->reg[n] = offset_addr;
	}
	
	if (t==15) {
		if ( (address & (1u << 0)) && (address & (1u << 1)) ) {
			return 1;
		}
		else {
			BranchWritePC (data, emul) ;
		}
	}
	else {
		emul->reg[t] = data;
	}  

	return 0;
}


//---------------------------------------------------------------------------------------------------------//						 


int ldr_imm_T1 (Instruction ins, int* imm32, int* index, int* add, int* wback, int* n, int* t) {
	
	*t = ins.reg->plages[0].value;
	*n = ins.reg->plages[1].value;
	*imm32 = ins.imm->plages[0].value << 2;
	*index = 1;
	*add = 1;
	*wback = 0;
	return 0;	
}

int ldr_imm_T2 (Instruction ins, int* imm32, int* index, int* add, int* wback, int* n, int* t) {

	*t = ins.reg->plages[0].value;
	*n = 13;	
	*imm32 = ins.imm->plages[0].value;
	*index = 1;
	*add = 1;
	*wback = 0;
	return 0;	
}

int ldr_imm_T3 (Instruction ins, int* imm32, int* index, int* add, int* wback, int* n, int* t) {

	*t = ins.reg->plages[0].value;
	*n = ins.reg->plages[1].value;
	if (*n==15) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	*imm32 = ins.imm->plages[0].value;
	*index = 1;
	*add = 1;
	*wback = 0;
	if (*t==15) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
}
	
//---------------------------------------------------------------------------------------------------------//						 





/**********************************************************************************************************/
/************************************************PUSH***************************************************/
/**********************************************************************************************************/


int push (Instruction ins, Emulator* emul) {
	
	int t;
	int i;
	long registers;
	long address;


	if (ins.encoding == 1 ) {
		if (push_T1 (ins, &registers)) {
			return 1;
		}
	}

	else if (ins.encoding == 2 ) {
		if (push_T2 (ins, &registers)) {
			return 1;
		}
	}
	
	else if (ins.encoding == 3 ) {
		if (push_T3 (ins, &registers, &t)) {
			return 1;
		}
	}
	
	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}
	
	address = emul->reg[13] - 4*BitCount(registers);
	for (i=0 ; i<=14; i++) {
		if (registers & (1u << i) ){
			address = emul->reg[i];
			address = address + 4;
		}
		
	}
	emul->reg[13] = emul->reg[13] - 4*BitCount(address) ;
	return 0;

}



//---------------------------------------------------------------------------------------------------------//



int push_T1 (Instruction ins, long* registers) {

	*registers = ins.imm->plages[0].value;
	if (ins.ext->plages[0].value) {
		*registers = *registers | (1u << 16) ;
	} 	
	if (BitCount(*registers)<1){
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
}

int push_T2 (Instruction ins, long* registers){

	*registers = ins.imm->plages[0].value;
	if (ins.ext->plages[0].value) {
		*registers = *registers | (1u << 14) ;
	} 	
	if (ins.ext->plages[1].value) {
		*registers = *registers | (1u << 15) ;
	} 	

	if (BitCount (*registers)<2 || (ins.ext->plages[1].value && ins.ext->plages[0].value) ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	if (*registers & (1u << 15)) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
		 
	return 0;
}

int push_T3 (Instruction ins , long* registers , int* t){

	*registers = 0;
	int s = *t;
	int d;
	d = (unsigned int) s;
	*registers = *registers | (1u < d ) ;
	if (*t==13 || *t==14 || *t==15 ) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
} 
	
//---------------------------------------------------------------------------------------------------------//						 


	
/**********************************************************************************************************/
/************************************************LDR_LIT***************************************************/
/**********************************************************************************************************/
	
int ldr_lit (Instruction ins, Emulator* emul) {
	

	int t;
	long imm32, base, address, data;
	int add;
	t = ins.reg->plages[0].value;
	
	imm32 = ins.imm->plages[0].value;
	
	if ( ins.encoding == 1 ) {
		if ( ldr_litt_T1 (ins, &imm32, &add) ) {
			return 1;
		}
	}

	else if ( ins.encoding == 2 ) {
		if (ldr_litt_T2 (ins, &imm32, &t, &add)) {
			return 1;
		}
	}

	
	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}

	base = emul->reg[15];
	if (add) {
		address = base + imm32;
	}
	else {
		address = base - imm32;
	}
	data = address;
	if (t==15) {
		if ((address & (1u << 0)) && (address & (1u << 1))) {
			WARNING_MSG ("Erreur");
			return 1;
		}
		else {
			BranchWritePC (data , emul);
		}
	}
	else {
		emul->reg[t] = data;
	} 			
	
	return 0;
}	



//---------------------------------------------------------------------------------------------------------//

int ldr_litt_T1 (Instruction ins,long* imm32, int* add) {

	*imm32	= *imm32 << 2;
	*add = 1;
	return 0;

}

int ldr_litt_T2 (Instruction ins,long* imm32, int* t, int* add) {
	 
	if (ins.ext->plages[0].value == 1) {
		*add = 1;
	}
	
	if (*t==15 /*&& InITBlock() && !LastInITBlock()*/) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;

}
	

//---------------------------------------------------------------------------------------------------------//



/**********************************************************************************************************/
/************************************************STR_IMM***************************************************/
/**********************************************************************************************************/


int str_imm (Instruction ins, Emulator* emul) {
	

	int t,n;
	long imm32;
	long offset_addr, address;
	int index, add, wback;
	t = ins.reg->plages[0].value;
	
	imm32 = ins.imm->plages[0].value;
	
	if ( ins.encoding == 1 ) {
		if (str_imm_T1 (ins, &imm32, &index, &add, &wback, &n )) {
			return 1;
		}
	}

	else if ( ins.encoding == 2 ) {
		if ( str_imm_T2 (ins, &imm32, &index, &add, &wback, &n )) {
			return 1;
		}
	}

	else if ( ins.encoding == 3 ) {
		if (str_imm_T3 (ins, &index, &add, &wback, &n, &t )) {
			return 1;
		}
	}

	else {
		WARNING_MSG ("Cet encodage n'est pas dans le dictionnaire");
		return 1;
	}

	if (add) {
		offset_addr = emul->reg[n] + imm32;
	}
	else {
		offset_addr = emul->reg[n] - imm32;
	}
	
	if (index) {
		address = offset_addr;
	}
	else {
		address = emul->reg[n];
	}

	address = emul->reg[t];

	if (wback) {
		emul->reg[n] = offset_addr;
	}

	return 0;
}



//---------------------------------------------------------------------------------------------------------//	

int str_imm_T1 (Instruction ins, long* imm32, int* index, int* add, int* wback, int* n ){

	*n = ins.reg->plages[1].value;
	*imm32 = *imm32 << 2;
	*index = 1;
	*add = 1;
	*wback = 0;
	return 0;

}

int str_imm_T2 (Instruction ins, long* imm32, int* index, int* add, int* wback, int* n ){

	*n = 13;
	*imm32 = *imm32 << 2;
	*index = 1;
	*add = 1;
	*wback = 0;
	return 0;

}

int str_imm_T3 (Instruction ins, int* index, int* add, int* wback, int* n, int* t ){

	*n = ins.reg->plages[1].value;
	if (*n==15) {
		WARNING_MSG ("Non défini");
		return 1;
	}

	*index = 1;
	*add = 1;
	*wback = 0;
	return 0;
	if (*t==15) {
		WARNING_MSG ("Accès non autorisé");
		return 1;
	}
	return 0;
	
}


//---------------------------------------------------------------------------------------------------------//	






