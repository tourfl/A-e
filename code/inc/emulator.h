#ifndef EMULATOR_H
#define EMULATOR_H value


#include "simul/segment.h"
#include "simul/registre.h"
#include "simul/dic.h"
#include "inter/interpreteur.h"



// pour le problème d'inclusions circulaires
struct dic;
typedef struct dic Dic;



struct emulator
{
	Segment map[NB_SEC];
	Registre reg[NB_REG];

	Dic *dic;

	interpreteur inter;

};

typedef struct emulator Emulator;








Emulator* init_emul();
void del(Emulator *emul);


#endif