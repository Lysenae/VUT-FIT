// ppm.c
// Riesenie IJC-DU1, príklad a), 19.3.2016
// Autor:     Daniel Klimaj, FIT
// Prelozene: gcc 4.8.4
// Praca s formatom .ppm

#include "ppm.h"

struct ppm *ppm_read(const char *filename)
{
  struct ppm *pic;
  FILE *in_file;
  unsigned xsize, ysize;
  unsigned long size = 0;
  int c, i;

  xsize = ysize = 0;

  if((in_file = fopen(filename, "rb")) == NULL)
  {
    warning_msg("Nepodarilo sa otvorit subor %s\n", filename);
    return NULL;
  }

  if(fscanf(in_file, "P6 %u %u 255 ", &xsize, &ysize) != 2)
  {
    fclose(in_file);
    warning_msg("Nepodarilo sa nacitat rozmery obrazka zo suboru %s\n", filename);
    return NULL;
  }

  if(xsize == 0 || ysize == 0)
  {
    fclose(in_file);
    warning_msg("Neboli nacitane spravne rozmery obrazka %s\n", filename);
    return NULL;
  }

  size = xsize * ysize * COLBYTES;
  if((pic = (struct ppm*)malloc(sizeof(pic) + size)) == NULL)
  {
    free(pic);
    fclose(in_file);
    warning_msg("Nedostatok pamati pre alokaciu obrazka\n");
    return NULL;
  }

  i = 0;
  pic->xsize = xsize;
  pic->ysize = ysize;

  while((c = fgetc(in_file)) != EOF)
  {
    if(i >= size)
    {
      free(pic->data);
      free(pic);
      fclose(in_file);
      warning_msg("Nacitane data prekrocili vymedzenu velkost\n");
      return NULL;
    }

    pic->data[i++] = (char)c;
  }

  return pic;
}

int ppm_write(struct ppm *p, const char *filename)
{
  unsigned long psize;
  FILE *out_file;

  if(p == NULL)
  {
    warning_msg("Ziadne data k zapisu\n");
    return WRITE_FAIL;
  }

  if((out_file = fopen(filename, "wb")) == NULL)
  {
    warning_msg("Nepodarilo sa otvorit subor k zapisu %s\n", filename);
    return WRITE_FAIL;
  }

  psize = ppm_size(p);
  fprintf(out_file, "P6\n%u %u\n255\n", p->xsize, p->ysize);
  for(unsigned long i=0; i<psize; i++)
  {
    fputc(p->data[i], out_file);
  }

  fclose(out_file);
  return WRITE_OK;
}

unsigned long ppm_size(struct ppm *p)
{
  return p->xsize * p->ysize * COLBYTES;
}

void ppm_free(struct ppm *p)
{
  free(p);
  p = NULL;
}