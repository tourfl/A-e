#include "simul/registre.h"
#include <stdio.h> // sprintf
#include <string.h> // strcmp
#include "inter/notify.h" // messages de contrôle





int get_reg(char *name, word *content, Registre reg[NB_REG])
{
    char n[4];
    int k = -1; // indice du reg trouvé

    if(name[0] == 'r')
    {
        int i;
        for (i = 0; i < NB_REG-4 && k<0; ++i)
        {
            sprintf(n, "r%i", i); // On stocke "ri" dans n

            if(strcmp(name, n) == 0)
                k=i;
        }
    }

    else if(strcmp(name, "sp") == 0)
        k=13;
    
    else if(strcmp(name, "lr") == 0)
        k=14;

    else if(strcmp(name, "pc") == 0)
        k=15;

    else if(strcmp(name, "apsr") == 0)
        k=16;

    if(k < 0)
        return 1;

    *content = reg[k];
    return 0;
}









int set_reg(char *name, word value, Registre reg[NB_REG])
{
	char n[4];
    int k = -1; // indice du reg trouvé

    if(name[0] == 'r')
    {
    	int i;
    	for (i = 0; i < NB_REG-4 && k<0; ++i)
    	{
            sprintf(n, "r%i", i); // On stocke "ri" dans n

            if(strcmp(name, n) == 0)
            	k=i;
        }
    }

    else if(strcmp(name, "sp") == 0)
    	k=13;
    
    else if(strcmp(name, "lr") == 0)
    	k=14;

    else if(strcmp(name, "pc") == 0)
    	k=15;

    else if(strcmp(name, "apsr") == 0)
    	k=16;

    if(k < 0) {
    	WARNING_MSG ("No '%s' registre\n", name);
    	return 1;
    }

    reg[k] = value;
    return 0;
}
