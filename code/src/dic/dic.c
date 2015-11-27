#include "dic/dic.h" // inclut Strlist et word

#include <stdlib.h> // calloc
#include "inter/notify.h"
#include "dic/ins_disas.h"
#include "elf/bits.h" // wrd_good_endianness







Dic* init_dic()
{
	return calloc(1, sizeof(Dic));
}





void del_dic(Dic *dic)
{
	int i;

	for (i = 0; i < dic->sz32; ++i) {
		del_ins(dic->ins32 + i); // instructions 32 bits
	}

	for (i = 0; i < dic->sz16; ++i) {
		del_ins(dic->ins16 + i); // instructions 16 bits
	}
}







int disp_dic(Dic *dic)
{
	int i;

	for (i = 0; i < dic->sz32; ++i) {
		disp_ins(*(dic->ins32 + i)); // instructions 32 bits
	}

	for (i = 0; i < dic->sz16; ++i) {
		disp_ins(*(dic->ins16 + i)); // instructions 16 bits
	}

	return 0;
}






int load_dic(Dic *dic)
{
	int r=0;


	r = load_from_file(&(dic->ins32), &(dic->sz32), "lib/instructions_32bits.dic");

	if(r != 0)
		return r;


	r =  load_from_file(&(dic->ins16), &(dic->sz16), "lib/instructions_16bits.dic");

	return r;
}





// p_instab : pointeur vers un tableau d'instruction (output)
// nb_ins : pointeur vers le nombre d'instructions (output)



int load_from_file(Instruction **p_instab, int *nb_ins, char* filename)
{

	int r = 0, sz = 0;
	FILE *fd = NULL;
	Instruction *instab;




	

	fd = fopen(filename, "r");

	if(fd == NULL)
	{
		return 9; // cf which_error de src/inter/notify.c
	}

	sz = get_nb_ins(fd);
	instab = init_instab(sz);

	if(instab == NULL)
		return 2;

	r = load_instab(instab, sz, fd);

	fclose(fd);

	*nb_ins = sz;
	*p_instab = instab;

	return r;
}





/*
 * instab est un tableau d'instruction
 * nb_ins est le nombre d'instructions de ce tableau
 */



int load_instab(Instruction *instab, int nb_ins, FILE *fd)
{
	char chaine[TAILLE_MAX] = "";
	int i, l = 0;




	if(fd == NULL) return 9;

	// On suppose qu'il n'y a pas d'erreurs dans le fichier

	for (i = 0; i < nb_ins; i++) // les premières instructions sont en 32 bits (il y en a NB_INS_32)
	{
		if (fgets(chaine, TAILLE_MAX, fd) == NULL)
			return 1;

		// printf("%s\n", chaine);

		l = load_ins(instab + i, chaine);


		if(l != 0)
			return l;

		// disp_ins(dic[i]);
	}

	return 0;
}






int get_nb_ins(FILE *fd)
{
	int sz = 0;
	char ch = 0;



	if(fd == NULL) return 9;

	sz++;

	while(!feof(fd)) // pour connaître la taille
	{
		ch = fgetc(fd);
		if(ch == '\n')
		{
			sz++;
		}
	}
	// printf("\nsz = %u\n", sz);

	if(fseek(fd, 0, SEEK_SET) != 0) // On se remet au début du fichier
		return 8;

	return sz;

}





// retourne l'offset de lecture de la plage d'octets


int find(word in32, Ins_disas *out, Dic *dic)
{
	int r = 1;
	word in16 = 0;




	in32 = wrd_good_endianness(in32);
	in16 = in32 >> 16;

	r = get_ins16(in16, out, dic);

	if (r == 0) // C'est une instruction 16 bits
		return 2;

	// printf("unfound word: %8x\t%s\n", mot, int_to_bin(mot, 16));

	r = get_ins32(in32, out, dic);

	if (r == 0) // C'est une instruction 32
		return 4; // on augmente de 4 puisque une instruction de 4 octets a été lue

	return 1;
}



/*

	valeurs de retour :
	1 : introuvable
	2 ou 4 : offset pour le prochain mot
	3 : problème lors du décodage

*/


int find_and_decode(word mot, Ins_disas *insd, Dic *dic)
{
	int r1, r2;

	r1 = find(mot, insd, dic);


	if(r1 == 1)
		return 1;


	r2 = decode(mot, insd);
	
	if (r2 == 0)
		return r1;
	else
		return 3;

}






int get_ins32(word in, Instruction *out, Dic *dic) {
	return get_ins(in, out, dic->ins32, dic->sz32);
}
int get_ins16(word in, Instruction *out, Dic *dic) {
	return get_ins(in, out, dic->ins16, dic->sz16);
}



			
			
