#ifndef CODE_ARM_H
#define CODE_ARM_H

#include "types.h" // Plgtab
#include "emulator.h"






void ZeroExtend_plgtab(Plgtab *imm);
void SignExtend_plgtab(Plgtab *imm);

long ZeroExtend (int x);
int BitCount (long a);
unsigned long AddWithCarry (unsigned long registre , unsigned long imm32 , int* carry , int* overflow);
long UInt32 (long x);
int UInt16 (int x);
long SInt32 (long x);
int SInt16 (int x );
void BXWritePC (vaddr32 a, Emulator* emul);
void BranchWritePC (vaddr32 adresse, Emulator* emul);
int IsZeroBit(long x);


#endif





