#include "emulator.h"

#include "inter/notify.h" // Messages de contrôle







 Emulator* init_emul() {
 	Emulator *em = calloc(1, sizeof(*em)); // met à 0 ou NULL toutes les valeurs

 	if(em == NULL)
 	{
 		ERROR_MSG("Problem with calloc");
 		return NULL;
 	}

 	em->dic = init_dic();
  	em->inter = init_inter();

 	if(em->dic == NULL
 	|| em->inter == NULL)
 	 	return NULL;

    return em;
 }





void del(Emulator *em) {

	del_map(em->map);
	del_dic(em->dic);
	del_inter(em->inter);

	free(em);
}
