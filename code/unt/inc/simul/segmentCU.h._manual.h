
#ifndef  SEGMENTCU_H
#define SEGMENTCU_H

#include "simul/segment.h" // NB_SEC



static Segment map_fix[NB_SEC]; // fixture mais avec un fichier réel





int CU_init_seg_suite();
int CU_del_seg_suite();


void CU_get_word();
void CU_get_byte();


#endif