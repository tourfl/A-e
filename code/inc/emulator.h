#ifndef EMULATOR_H
#define EMULATOR_H value


#include "simul/segment.h"
#include "simul/registre.h"
#include "simul/dic.h"
#include "inter/interpreteur.h"


typedef struct
{
	Segment map[NB_SEC];
	Registre reg[NB_REG];

	Dic *dic;

	interpreteur inter;

} Emulator;







Emulator* init_emul();
void del(Emulator *emul);


#endif