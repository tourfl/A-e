#ifndef EMULATOR_H
#define EMULATOR_H value


#include "simul/segment.h"
#include "simul/registre.h"
#include "dic/dic.h"
#include "inter/interpreteur.h"



// pour le problème d'inclusions circulaires
struct dic;


struct emulator
{
	Segment map[NB_SEC];
	Registre reg[NB_REG];

	struct dic *dic;

	interpreteur inter;

	int it_state;
};

typedef struct emulator Emulator;








Emulator* init_emul();
void del(Emulator *emul);


#endif