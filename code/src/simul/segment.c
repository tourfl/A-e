#include "simul/segment.h" // inclut la mémoire
#include "inter/notify.h" // messages de contrôle, inclut stdio, stdlib
#include <string.h> // pour strcpy notamment
#include <math.h> // pow






void del_map(Segment map[NB_SEC])
{
    int i;

    for (i = 0; i < NB_SEC; ++i)
    {
        del_seg(map + i);
    }
}









int realloc_seg(int size, int i, Segment map[NB_SEC])
{
    void *p = realloc(map[i].content, size * sizeof(byte));

	if(p == 0)
        return 1;

    else
    {
        map[i].content = p;
		map[i].size = size;

        if(i < NB_SEC && map[i].va + size >= map[i+1].va) // Si le segment empiète sur le suivant, on décale les adresses
        {

        	int j;

        	for (j = i+1; j < NB_SEC; j++)
        		map[j].va = (map[j-1].size/4096 + 1) * 4096;
        }

        return 0;
    }
}






void del_seg(Segment *seg)
{
      free(seg->content);
}







byte get_byte(vaddr32 va, Segment map[NB_SEC])
{
	vaddr32 va_start;
	vaddr32 va_end;
	vaddr32 size;
	int i;

	for (i = 0; i < NB_SEC; i++)
	{
		va_start = map[i].va;
		size = map[i].size;

		if (va < va_start)
			return 0;

		if(size > 0)
		{
            va_end = va_start + size - 1;  // avec 00 ff : size = 2, va_end = va_start + size - 1

            if (va <= va_end)
            	return *(map[i].content + va - va_start);
        }
    }

    return 0;
}









int set_byte(vaddr32 va, byte value, Segment map[NB_SEC])
{
	vaddr32 va_start;
	vaddr32 va_end;
	vaddr32 size;
	int i;

	for (i = 0; i < NB_SEC; i++)
	{
		va_start = map[i].va;
		size = map[i].size;

		if (va < va_start)
            return 1; // On ne fait rien si l'octet n'appartient pas au map

        if(size > 0)
        {
            va_end = va_start + size - 1;  // avec 00 ff : size = 2, va_end = va_start + size - 1

            if (va <= va_end)
            {
            	*(map[i].content + va - va_start) = value;
            	return 0;
            }
        }
    }

    return 1; // On ne fait rien si l'octet n'appartient pas au map
}

word get_word(vaddr32 va_1, Segment map[NB_SEC])
{

  /* Les octets sont dans des segments sinon ils sont nuls 
  */

	vaddr32 va = va_1;
	vaddr32 va_start;
	vaddr32 va_end;
	uint32_t size;
	word w = 0;
	int i;

	for (i = 0; i < NB_SEC; i++)
	{
		va_start = map[i].va;
		size = map[i].size;

		while (va < va_start)
		{
            if(va == va_1 + 3) // $(0x1)*16^3 + $(0x2)*16^2 + $(0x3)*16 + 0x4
            	return w;

            va++;
        }

        if(size > 0)
        {
            va_end = va_start + size - 1;  // avec 00 ff : size = 2, va_end = va_start + size - 1

            while (va <= va_end)
            {

            	w += *(map[i].content + va - va_start) * pow(16, 2*(3-va+va_1));

            	if (va == va_1 + 3)
            		return w;

            	va++;
            }
        }
    }

    return w;
}

int set_word(vaddr32 va_1, word value, Segment map[NB_SEC])
{

  /* Si va n'appartient à aucune section, rien n'est inscrit et la fonction renvoie 1
   * 
   * Cas particulier :
   * Le mot écrit est à la fin d'une section, nécessitant l'ajout d'octets à cette section, une nouvelle allocation est requise
   */

   vaddr32 va = va_1;
   vaddr32 va_start;
   vaddr32 va_end;
   uint32_t size;
   int p;
   int i;

   for (i = 0; i < NB_SEC; i++)
   {
		va_start = map[i].va;
		size = map[i].size;

   		if(va < va_start)
   			return 1;

   		if(size > 0)
   		{
        	va_end = va_start + size - 1;  // avec 00 ff : size = 2, va_end = va_start + size - 1

        	if(va <= va_end && va + 3 > va_end) // On réalloue la section
        	{
            	size = size + va + 3 - va_end; // On met à jour la taille
            	va_end = va_start + size;

            	if (realloc_seg(size, i, map) != 0)
            		return 2;
        	}

        	while (va <= va_1 + 3)
        	{
            	// big endian

        		p = pow(16, 2*(3-va+va_1));

        		*(map[i].content + va - va_start) = value / p;

        		value = value % p;

        		if(va == va_1 + 3)
        			return 0;

        		va++;
        	}
    	}
	}

	return 1;
}





byte *get_plage(Plage p, Segment map[NB_SEC]) // on suppose va_2 >= va_1
{
    if(p.end < p.end)
    {
        WARNING_MSG("va_2 < va_1");
        return NULL;
    }

    byte *plage = malloc((p.end - p.start + 1) * sizeof(byte));

    vaddr32 va = p.start;
    vaddr32 va_start;
    vaddr32 va_end;
    vaddr32 size;
    int i;

  /* Les octets appartiennent à des segments sinon ils sont nuls 
  */

    for (i = 0; i < NB_SEC; i++)
    {
        va_start = map[i].va;
        size = map[i].size;


        while (va < va_start)
        {
            plage[va - p.start] = 0;

            if (va == p.end)
                return plage;

            va++;
        }

        if(size > 0)
        {
            va_end = va_start + size - 1;  // avec 00 ff : size = 2, va_end = va_start + size - 1

            while (va <= va_end)
            {
                plage[va - p.start] = *(map[i].content + va - va_start);

                if (va == p.end)
                    return plage;

                va++;
            }
        }
    }

    while (va <= p.end)
    {
        plage[va - p.start] = 0;

        va++;
    }

    return plage;
}

