#include "simul/dic.h" // inclut Strlist et word

#include <stdlib.h> // calloc
#include "inter/notify.h"







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
	int r = 0, sz = 0;
	FILE *fd = NULL;




	

	fd = fopen("lib/instructions_32bits.dic", "r");

	dic->sz32 = sz = get_nb_ins(fd);
	dic->ins32 = init_instab(sz);

	if(dic->ins32 == NULL)
		return 2;

	r = load_ins_tab_from_file(dic->ins32, dic->sz32, fd);

	fclose(fd);



	fd = fopen("lib/instructions_16bits.dic", "r");

	dic->sz16 = sz = get_nb_ins(fd);
	dic->ins16 = init_instab(sz);

	if(dic->ins16 == NULL)
		return 2;

	r = load_ins_tab_from_file(dic->ins16, dic->sz16, fd);

	fclose(fd);

	return r;
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





/*
 * dic est un pointeur sur un tableau d'instruction
 * dic_sz est un pointeur sur la future taille de ce tableau
 */



int load_ins_tab_from_file(Instruction *dic, int dic_sz, FILE *fd)
{
	char chaine[TAILLE_MAX] = "";
	int i, l = 0;




	if(fd == NULL) return 9;

	// On suppose qu'il n'y a pas d'erreurs dans le fichier

	for (i = 0; i < dic_sz; i++) // les premières instructions sont en 32 bits (il y en a NB_INS_32)
	{
		if (fgets(chaine, TAILLE_MAX, fd) == NULL)
			return 1;

		// printf("%s\n", chaine);

		l = load_ins(dic + i, chaine);


		if(l != 0)
			return l;

		// disp_ins(dic[i]);
	}

	return 0;
}


			
			
