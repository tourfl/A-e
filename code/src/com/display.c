#include "com/command.h" // inclut memory
#include "com/types.h" // byte
#include "com/notify.h" // messages de contrôle, inclut stdlib (printf)
#include <string.h> // pour strcmp

// fonction affichant les octets d'un segment sur la sortie standard
// parametres :
//   name           : nom de la section dont le contenue est affichée
//   start          : adresse virtuelle d'implantation du contenu de la section dans la memoire 
//   content        : le contenu de la section à afficher
//   taille         : taille en octet de la section à afficher


void print_section_bytes(int start, byte *content)
{
  unsigned char octet =0;
  if (content!=NULL) {
    octet = *((unsigned char *) (content+start));
    printf("%02x ", octet);
  }
}

void print_section_raw_content(char* name, unsigned int start, byte* content, unsigned int taille) {
  int k;
  unsigned char octet =0;
  printf("\n section %s loaded at %x :\n",name,start); 
  if (content!=NULL && taille>0) {
    for(k=0; k<taille; k+=1) {
           // on affiche le contenu de la section qui devrait se retrouver 
           // en "memoire virtuelle" à l'adresse virtuelle start+k 
           // (*(content + k) dans la mémoire physique)
      if(k%16==0) printf("\n  0x%08x ", start + k);
      octet = *((unsigned char *) (content+k));
      printf("%02x ", octet);
    }
  }
  printf("\n");
}

void disp_map(Memory *mem)
{
  int i;
  for(i = 0; i < NB_SEC; i++)
  {
    if(mem->map[i].size != 0)
    {
      print_section_raw_content(mem->map[i].name, (unsigned int) mem->map[i].va, mem->map[i].content, mem->map[i].size);
    }
  }

  printf("\n");
}

void disp_plage (unsigned int va_1, unsigned int va_2, Memory *mem) // on suppose va_1 > va_2
{
  /* Les octets sont dans des segments sinon ils sont nuls 
  */

  unsigned int va = va_1;
  unsigned int va_start;
  unsigned int va_end;
  unsigned int size;
  int k = 0;
  int i;
  
  for (i = 0; i < NB_SEC; i++)
  {
    va_start = mem->map[i].va;
    size = mem->map[i].size;
    va_end = va_start + size;

    while (va < va_start)
    {
      if (k % 16 == 0)
        printf("\n  0x%08x ", va);

      printf("00 ");

      if(va == va_2)
      {
        printf("\n");
        return;
      }
      
      va++;
      k++;
    }

    while (va <= va_end)
    {

      if (k % 16 == 0)
        printf("\n  0x%08x ", va);

      print_section_bytes(va - va_start, mem->map[i].content);

      if (va == va_2)
      {
        printf("\n");
        return;
      }
      
      va++;
      k++;
    }
  }

  while (va <= va_2)
  {
    if (k % 16 == 0)
      printf("\n  0x%08x ", va);

    printf("00 ");

    va++;
    k++;
  }

  printf("\n");
}

void disp_oct(unsigned int va, Memory *mem)
{
  unsigned int va_start;
  unsigned int va_end;
  unsigned int size;
  unsigned char octet =0;

  printf("  0x%08x ", va);
  int i;

  for (i = 0; i < NB_SEC; i++)
  {
    va_start = mem->map[i].va;
    size = mem->map[i].size;
    va_end = va_start + size;

    if (va < va_start)
      break;

    if (va < va_end)
    {
      octet = *((unsigned char *) (mem->map[i].content + va - va_start));
      printf("%02x\n", octet);
      return;
    }

  }

  printf("00 \n");
}

void disp_reg(char *name, Registre reg[NB_REG])
{
  char n[4];

  if(name[0] == 'r')
  {
    int i;
    for (i = 0; i < NB_REG-4; ++i)
    {
        sprintf(n, "r%i", i); // On stocke "ri" dans n

        if(strcmp(name, n) == 0)
        {
          printf("  r%i : %i \n", i, reg[i]);
          return;
        }
    }

    WARNING_MSG ("No '%s' registre\n", name);
  }

  else if(strcmp(name, "sp") == 0)
  {
    printf("  sp : %i \n", reg[13]);
  }
  else if(strcmp(name, "lr") == 0)
  {
    printf("  lr : %i \n", reg[14]);
  }
  else if(strcmp(name, "pc") == 0)
  {
    printf("  pc : %i \n", reg[15]);
  }
  else if(strcmp(name, "apsr") == 0)
  {
    printf("  apsr : %i \n", reg[16]);
  }
  else WARNING_MSG ("No '%s' registre\n", name);
}

void disp_all_regs(Registre reg[NB_REG])
{
  int i;
  for (i = 0; i < NB_REG-4; ++i)
  {
    printf("  r%i : %i \n", i, reg[i]);
  }

  printf("  sp : %i \n", reg[13]);
  printf("  lr : %i \n", reg[14]);
  printf("  pc : %i \n", reg[15]);
  printf("  apsr : %i \n", reg[16]);
}
