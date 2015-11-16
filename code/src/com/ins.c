










typedef int (*exec) (mem, Instruction);

exec isn_code [NB_INS];






/*******************************************ADD_IMM********************************************************/


int add_imm (Instruction ins, Mem mem) {


	int n,d;
	int setflags, imm32, carry, overflow, result;
	
	n = ins.reg[0].value;
	d = ins.reg[1].value;
	
	if (strcmp (ins.encoding, 'T1') == 0 ) {
		add_imm_T1 (ins, mem, setflags, imm32);
	}

	else if (strcmp (ins.encoding, 'T2') == 0 ) {
		add_imm_T2 (ins, mem, setflags, imm32);
	}

	else if (strcmp (ins.encoding, 'T3') == 0 ) {
		add_imm_T3 (ins, mem, setflags, imm32, d, n);
	}

	else if (strcmp (ins.encoding, 'T4') == 0 ) {
		add_imm_T4 (ins, mem, setflags, imm32, d, n);
	}

	else 
		WARNING_MSG ("Cette encodage n'est pas dans le dictionnaire");


	(result, carry, overflow) = AddWithCarry (R[n] , imm32, '0');
	R[d] = result;
	if (setflag) {
		APSR.N = result<31>;
		APSR.Z = IsZeroBit(result);
		APSR.C = carry;
		APSR.V = overflow;
	}
}





//------------------------------------------------------------------------------------------------------//





int add_imm_T1(Instruction ins, Mem mem, int setflags, int imm32) {
		
	setflags = !InITBlock();
	imm32 = ZeroExtend( ins.imm , 32); 
	
}


int add_imm_T2(Instruction ins, Mem mem, int setflags , int imm32) {
	
	setflags = !InITBlock();
	imm32 = ZeroExtend( ins.imm , 32); 

}


int add_imm_T3(Instruction ins, Mem mem, int setflags , int imm32 , int n, int d) {
	

	if ( d == 15 && ins.ext.value == 1 ) {
		// SEE CMN (immmediate)
	} 

	if ( n == 13 ) {
		// SEE ADD (SP plus immediate)
	}

	setflags = 1; 
	imm32 = ins.imm.value;

	if (d==13 || (d==15 && ins.ext.value == 0) || n==15 ) {
		//UNPREDICTABLE
	}

}


int add_imm_T4(Instruction ins, Mem mem, int setflags , int imm32, int n, int d ) {

	if (n == 15) {
		// SEE ADR
	}


	if (n == 13) {
		// SEE ADD
	}

	setflags = 0;
	imm32 = ins.imm.value;
if ( d == 13 || d == 14 || d == 15) {
		//UNPREDICTABLE
	}
	
}


//---------------------------------------------------------------------------------------------------------//




/**********************************************************************************************************/
/*******************************************ADD_REG********************************************************/
/**********************************************************************************************************/

int add_reg (Instruction ins, Mem mem) {
	
	int n,d,m;
	int setflags, imm32, carry, overflow, result;
	int shift_n, shift_t;
	
	n = ins.reg[0].value;
	d = ins.reg[1].value;
	m = ins.reg[2].value;

	if (strcmp (ins.encoding, 'T1') == 0 ) {
		add_reg_T1 (ins, mem);
	}

	else if (strcmp (ins.encoding, 'T2') == 0 ) {
		add_reg_T2 (ins, mem);
	}

	else if (strcmp (ins.encoding, 'T3') == 0 ) {
		add_reg_T3 (ins, mem);
	}

	else 
		WARNING_MSG ("Cette encodage n'est pas dans le dictionnaire");


	shifted = Shift(R[m] , shift_t , shift_n , ASPR.C);
	(result, carry, overflow) = AddWithCarry (R[n] , shifted, '0');
	id (d==15) {
		ALUWritePC(result);
	}
	else {
		R[d]=result;
		if(setflags) {
			ASPR.N = result<31>;
			ASPR.Z = IsZeroBit(result);
			ASPR.C = carry;
			ASPR.V = overflow;
		}
	}
}

//---------------------------------------------------------------------------------------------------------//




int add_reg_T1(Instruction ins, Mem mem, int setflags, int shift_n, int shift_t) {
	setflags = !InITBlock();
	shift_n = 0;
	shift_t = SRType_LSL;
}


int add_reg_T2(Instruction ins, Mem mem, int setflags,int shift_n, int shift_t, int n ,int d ,int m) {

	if ( || m == 13) {
		// SEE ADD
	}
	
	setflags = 0;
	shift_n = 0;
	shift_t = SRType_LSL;
	
	if ( d==15 && InITBlock() && !LastInITBlock() ) {
		// UNPREDICTABLE
	}
	
	if (d==15 && m==15) {
		// UNPREDICTABLE
	}
	  
}


int add_reg_T3(Instruction ins, Mem mem, int setflags,int shift_n, int shift_t, int n , int d, int m) {

	if ( d==15 && ins.ext[1].value == 1) {
		// SEE CMN (register)
	}
	
	if ( n==13 ) {
		// SEE ADD (SP plus register)
	}
	
	setflags = 1;
	(shift_t, shift_n) = DecodeImmShift (ins.ext[0].value , ins.imm.value);
	
	if (d==13 || (d==15 && ins.ext[0].value == 0) || n==15 || m==13 || m==14 || m==15) {
		// UNPREDICTABLE
	}

}

//---------------------------------------------------------------------------------------------------------//



/**********************************************************************************************************/
/*******************************************B**************************************************************/
/**********************************************************************************************************/

int b (Instruction ins, Mem mem) {


	if (strcmp (ins.encoding, 'T1') == 0 ) {
		b_T1 (ins, mem);
	}

	else if (strcmp (ins.encoding, 'T2') == 0 ) {
		b_T2 (ins, mem);
	}

	else if (strcmp (ins.encoding, 'T3') == 0 ) {
		b_T3 (ins, mem);
	}

	else if (strcmp (ins.encoding, 'T4') == 0 ) {
		b_T4 (ins, mem);
	}

	else 
		WARNING_MSG ("Cette encodage n'est pas dans le dictionnaire");


	BranchWritePC(PC+imm32);
}

//---------------------------------------------------------------------------------------------------------//

int b_T1(Instruction ins, Mem mem) {
}


int b_T2(Instruction ins, Mem mem) {
}


int b_T3(Instruction ins, Mem mem) {
}


int b_T4(Instruction ins, Mem mem) {
}

//---------------------------------------------------------------------------------------------------------//




/**********************************************************************************************************/
/*******************************************BL*************************************************************/
/**********************************************************************************************************/


int bl (Instruction ins, Mem mem) {

	if (strcmp (ins.encoding, 'T1') == 0 ) {
		bl_T1 (ins, mem);
	}

	else 
		WARNING_MSG ("Cette encodage n'est pas dans le dictionnaire");


	next_instr_addr = PC;
	LR = next_instr_addr<31:1> : '1';
	BranchWritePC (PC+imm32);

}


//---------------------------------------------------------------------------------------------------------//

int bl_T1(Instruction ins, Mem mem) {
}

//---------------------------------------------------------------------------------------------------------//





/**********************************************************************************************************/
/************************************************BX********************************************************/
/**********************************************************************************************************/



int bx (Instruction ins, Mem mem) {

	if (strcmp (ins.encoding, 'T1') == 0 ) {
		bx_T1 (ins, mem);
	}

	else 
		WARNING_MSG ("Cette encodage n'est pas dans le dictionnaire");


	EncodingSpecificOperations();
	BXWritePC (R[m]);

}

int bx_T1(Instruction ins, Mem mem) {
}


/**********************************************************************************************************/
/************************************************BX********************************************************/
/**********************************************************************************************************/





























