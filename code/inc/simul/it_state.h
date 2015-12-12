
#ifndef IT_STATE_H
#define IT_STATE_H


void set_it_state(int *it_state, int mask, int sig, int thn, int els);
void reset_it_state(int *it_state);
void update_it_state(int *it_state);

void get_state(int *it_state, char state[3], char **states_tab);

int InITBlock (int it_state);

int LastInITBlock (int it_state);


#endif
