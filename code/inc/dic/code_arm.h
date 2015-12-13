#ifndef CODE_ARM_H
#define CODE_ARM_H

#include "types.h" // Plgtab
#include "emulator.h"






void ZeroExtend_plgtab(Plgtab *imm);
void SignExtend_plgtab(Plgtab *imm);

long ZeroExtend (int x);
int BitCount (long a);
long AddWithCarry (long registre , long imm32 , int* carry , int* overflow);
int condition (int cond, Emulator* emul);
void BXWritePC (vaddr32 a, Emulator* emul);
void BranchWritePC (vaddr32 adresse, Emulator* emul);
int IsZeroBit(long x);


#endif





