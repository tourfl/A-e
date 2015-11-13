#include "com/notify.h"






/*
 * Fonction qui interprète les retours des fonctions
 *
 */


void which_error(int code)
{
	DEBUG_MSG("Entering function which_error");

	switch (code) {
		case 0:
			DEBUG_MSG("No Problem");
			break;

		case 1:
		{
			DEBUG_MSG("Chaine vide");
			break;
		}

		case 2:
		{
			DEBUG_MSG("Error with malloc");
			break;
		}

		case 3:
		{
			DEBUG_MSG("Error with size");
			break;
		}

		case 4:
		{
			DEBUG_MSG("Uncorrect size of parameters");
			break;
		}

		case 5:
		{
			DEBUG_MSG("Error with strcpy");
			break;
		}

		case 6:
		{
			DEBUG_MSG("Problem with strcat");
			break;
		}

		case 8:
		{
			DEBUG_MSG("Error with fseek");
			break;
		}

		case 9:
		{
			DEBUG_MSG("Unable to open file");
			break;
		}

		default:
		{
			DEBUG_MSG("Untracked error %u", code);
			break;
		}
	}


	DEBUG_MSG("Exiting function which_error");
}

