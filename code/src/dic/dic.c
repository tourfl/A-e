#include "dic/dic.h" // inclut Strlist et word

#include <stdlib.h> // calloc
#include "inter/notify.h"
#include "dic/display_ins.h"
#include "elf/bits.h" // wrd_good_endianness
#include <string.h> // strcpy







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
		disp_not_decoded(*(dic->ins32 + i)); // instructions 32 bits
	}

	for (i = 0; i < dic->sz16; ++i) {
		disp_not_decoded(*(dic->ins16 + i)); // instructions 16 bits
	}

	return 0;
}






int load_dic(Dic *dic)
{
	int r=0;


	r = load_from_file(&(dic->ins32), &(dic->sz32), "lib/instructions_32bits.dic");

	if(r != 0)
		return r;

	// on range le dictionnaire (règle le problème d'opcodes ayant le même début)
	qsort(dic->ins32, dic->sz32, sizeof(Instruction), cmp_ins);


	r =  load_from_file(&(dic->ins16), &(dic->sz16), "lib/instructions_16bits.dic");

	qsort(dic->ins16, dic->sz16, sizeof(Instruction), cmp_ins);


	// On copie les mnémoniques
	strcpy(dic->str_cond[0], "EQ");
	strcpy(dic->str_cond[1], "NE");
	strcpy(dic->str_cond[2], "HS");
	strcpy(dic->str_cond[3], "LO");
	strcpy(dic->str_cond[4], "MI");
	strcpy(dic->str_cond[5], "PL");
	strcpy(dic->str_cond[6], "VS");
	strcpy(dic->str_cond[7], "VC");
	strcpy(dic->str_cond[8], "HI");
	strcpy(dic->str_cond[9], "LS");
	strcpy(dic->str_cond[10], "GE");
	strcpy(dic->str_cond[11], "LT");
	strcpy(dic->str_cond[12], "GT");
	strcpy(dic->str_cond[13], "LE");
	strcpy(dic->str_cond[14], "AL");


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

		// display(dic[i], NOT_DECODED);
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
// out est l'instruction désassemblée



/*

	valeurs de retour :
	1 : introuvable
	2 ou 4 : offset pour le prochain mot
	3 : problème lors du décodage

*/


int decode(word in, Instruction *out, Dic *dic)
{
	int r=1;



	//V2

	r = get_ins32(in, out, dic);

	if(r == 0)
	{
		out->fill_params(in, out);
		return 4;
	}		


	in = in >> 16;

	r = get_ins16(in, out, dic);

	if(r == 0)
	{
		out->fill_params(in, out);
		return 2;
	}

	return r;
}






int get_ins32(word in, Instruction *out, Dic *dic) {
	return get_ins(in, out, dic->ins32, dic->sz32);
}
int get_ins16(word in, Instruction *out, Dic *dic) {
	return get_ins(in, out, dic->ins16, dic->sz16);
}



			
			
