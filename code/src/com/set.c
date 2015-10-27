/*
 * Fonction setcmd et fonctions liées
 */

#include "com/types.h"
#include "com/command.h" // inclut interpreteur et mémoire
#include "com/notify.h" // Pour les messages de contrôle, inclut stdlib et stdio
#include <string.h> // pour strcmp notamment



int setcmd (interpreteur inter, Memory *mem){

	char* what = NULL;
	char* name = NULL;
	vaddr32 va = 0; // name en unsigned int
	char *value = NULL;
	word val = 0; // value en unsigned int
	char usage[] = "Usage : set mem <type> <adresse> <valeur>\n\tset reg <registre> <valeur>\n";

	what = get_next_token (inter); // récupère mem ou reg

	if(what != NULL && strcmp(what, "mem") == 0)
		what = get_next_token(inter); // mem devient word ou byte

	name = get_next_token(inter); // registre ou adresse
	value = get_next_token(inter); // valeur

	/* On commence par tester la valeur car elle est commune au 3 commandes
	*/

	if( what == NULL
	|| name == NULL
	|| value == NULL 
	|| get_next_token(inter) != NULL) // pas assez ou trop de tokens
	{
		WARNING_MSG("Problem with tokens");
		printf("%s", usage);
		return 1;
	}

	if(is_figure(value) == 2) // La valeur n'est pas correcte
	{
		WARNING_MSG("Value must be decimal, hexadecimal or octodecimal");
		return 2;
	}

	val = strtoul(value, NULL, 0);

	if(strcmp(what, "reg") == 0)
	{
		return set_reg(name, val, mem->reg);
	}
	else if(is_figure(value) == 2)
	{
		WARNING_MSG("Address must be decimal, hexadecimal or octodecimal");
		return 2;
	}
	else if(strcmp(what, "word") == 0)
	{
		va = strtoul(name, NULL, 0);

		return set_word(va, val, mem);
	}
	else if(strcmp(what, "byte") == 0)
	{
		va = strtoul(name, NULL, 0);

		return set_byte(va, val, mem);
	}
	else {
		WARNING_MSG("Bad value for second or third token");
		printf("%s", usage);
		return 3;
	}
	
}