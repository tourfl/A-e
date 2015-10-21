#include "common/command.h"

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
      printf("%02x ",	octet);
    }
  }
  printf("\n");
}

void disp_map(Memory *mem)
{
  unsigned int next_segment_start = START_MEM; // compteur pour designer le début de la prochaine section

int i;
  for(i = 0; i < NB_SECTIONS; i++)
  {
    if(mem->map[i].size != 0)
    {
      print_section_raw_content(mem->map[i].name, (unsigned int) mem->map[i].va, mem->map[i].content, mem->map[i].size);
      next_segment_start+= (mem->map[i].size/4096 + 1) * 4096; // on arrondit au 1k suppérieur
    }
  }

  printf("\n");
}

void disp_plage (unsigned long va_1, unsigned long va_2, Memory *mem) // on suppose va_1 > va_2
{
  /* Les octets sont dans des segments sinon ils sont nuls 
  */

  unsigned long va = va_1;
  unsigned long va_start;
  unsigned long va_end;
  unsigned long size;
  char k = 0;
char i;
  for (i = 0; i < NB_SECTIONS; i++)
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

void disp_oct(unsigned long va, Memory *mem)
{
  unsigned long va_start;
  unsigned long va_end;
  unsigned long size;
  unsigned char octet =0;

  printf("  0x%08x ", va);
char i;

  for (i = 0; i < NB_SECTIONS; i++)
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

void disp_reg(char *name, Registres *reg)
{
  char n[4];

  if(name[0] == 'r')
  {
char i;
    for (i = 0; i < REG_NB; ++i)
    {
        sprintf(n, "r%i", i); // On stocke "ri" dans n

        if(strcmp(name, n) == 0)
        {
          printf("  r%i : ", i);
          display_reg_content(reg->r[i]);
          return;
        }
    }

    WARNING_MSG ("No '%s' registre\n", name);
  }

  else if(strcmp(name, "sp") == 0)
  {
    printf("  sp : ");
    display_reg_content(reg->sp);
  }
  else if(strcmp(name, "lr") == 0)
  {
    printf("  lr : ");
    display_reg_content(reg->lr);
  }
  else if(strcmp(name, "pc") == 0)
  {
    printf("  pc : ");
    display_reg_content(reg->pc);
  }
  else if(strcmp(name, "apsr") == 0)
  {
    printf("  apsr : ");
    display_reg_content(reg->apsr);
  }
  else WARNING_MSG ("No '%s' registre\n", name);
}

void disp_all_regs(Registres *reg)
{
int i;
  for (i = 0; i < REG_NB; ++i)
  {
    printf("  r%i : ", i);
    display_reg_content(reg->r[i]);
  }

  printf("  sp : ");
  display_reg_content(reg->sp);
  printf("  lr : ");
  display_reg_content(reg->lr);
  printf("  pc : ");
  display_reg_content(reg->pc);
  printf("  apsr : ");
  display_reg_content(reg->apsr);
}

void display_reg_content(Registre reg)
{
  if(strlen(reg) == 0 || strcmp(reg, "\0") == 0)
    printf("vide \n");

  else
    printf("%s \n", reg);
}

