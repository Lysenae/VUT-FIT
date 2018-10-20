/*******************************************************************************
 * Soubor:  proj3.c
 * Datum:   2013/12/07
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Hladanie obrazcov
 * Popis:   Program v monochromatickom obrazku vyhlada najdlhsie vodorovne a
            zvisle useky alebo stvorce.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FOUND 1
#define NOT_FOUND 0

// Struktura obsahujuca hodnoty parametrov prikazoveho riadku.
typedef struct params
{
  char *fname;  // Nazov suboru s obrazkom.
  int ecode;    // Chybovy kod programu, odpoveda vyctu tecodes.
  int state;    // Stavovy kod programu, odpoveda vyctu tstates.
} Params;

typedef struct bitmap
{
  int rows;     // Pocet riadkov obrazka
  int cols;     // Pocet stlpcov obrazka
  char *cells;  // Retazec obsahujuci pixely pobrazka
} Bitmap;

// Prototypy funkcii
void print_ecode(int ecode);
Params get_params(int argc, char *argv[]);
char int_to_char(int x);

Bitmap *bitmap_new(int rows, int cols, int *err);
Bitmap *bitmap_new_from_file(FILE *file, int *err);
void bitmap_delete(Bitmap *bm);

char getcolor(Bitmap *bitmap, int x, int y);
int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2);
int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2);
int find_square(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2);

const char *FMODE = "r";

// Stavove kody programu
enum tstates
{
  SHELP,         // Napoveda
  STEST,         // Test
  SVLINE,        // Hladat vertikalnu (zvislu) usecku
  SHLINE,        // Hladat horizontalnu (vodorovnu) usecku
  SSQUARE,       // Hladat stvorec
};

// Kody chyb programu
enum tecodes
{
  EOK = 0,       // Bez chyby
  ECLWRONG,      // Chybny parameter prikazoveho riadku
  ELOWMEM,       // Neodstatok pamati
  EBADVAL,       // Nespravna hodnota
  EFOPEN,        // Chyba pri otvarani suboru
  ESHORTFILE,    // Kratky subor
  ENOTFOUND,     // Obrazec nenajdeny
  EPOINTONLY,    // Len jeden pixel
  EUNKNOWN,      // Neznama chyba
};

// Chybove hlasenia
const char *ECODEMSG[] =
{
  [EOK]        = "Vsetko v poraidku.\n",
  [ECLWRONG]   = "Chybne parametre prikazoveho riadku!\n",
  [ELOWMEM]    = "Nedostatok pamati!\n",
  [EBADVAL]    = "Najdena nespravna hodnota!\n",
  [EFOPEN]     = "Chyba pri otvarani suboru!\n",
  [ESHORTFILE] = "Predcasny koniec suboru!\n",
  [ENOTFOUND]  = "Hladany obrazec sa v obrazku nenachadza!\n",
  [EPOINTONLY] = "Bitmapa obsahuje len jeden pixel!\n",
  [EUNKNOWN]   = "Nieco sa pokazilo!\n",
};

const char *HELPMSG =
  "Program: Hladanie obrazcov\n"
  "Autor:   Daniel Klimaj (c) 2013\n\n"

  "Program vyhlada v monochromatickom obrazku najdlhsie vodorovne a zvisle\n"
  "usecky a stvorce.\n\n"

  "Pouzitie:\n"
  "  proj3 --help\n"
  "  proj3 --test FILE\n"
  "  proj3 --hline FILE\n"
  "  proj3 --vline FILE\n"
  "  proj3 --square FILE\n\n"

  "Parametre:\n"
  " --help, -h       Vypise tento text\n"
  " --test FILE      Skontoluje ci subor FILE obsahuje validny obrazok\n"
  " --hline FILE     V obrazku zo suboru FILE vyhlada najdlhsiu vodorovnu\n"
  "                  usecku\n"
  " --vline FILE     V obrazku zo suboru FILE vyhlada najdlhsiu zvislu usecku\n"
  " --square FILE    V obrazku zo suboru FILE vyhlada najvacsi stvorec\n\n"
;

/*******************************************************************************
 * Main
*******************************************************************************/
int main(int argc, char *argv[])
{
  Params params = get_params(argc, argv);
  if (params.ecode != EOK)
  {
    print_ecode(params.ecode);
    return EXIT_FAILURE;
  }

  if (params.state == SHELP)
  {
    printf("%s", HELPMSG);
    return EXIT_SUCCESS;
  }
  else
  {
    FILE *file = fopen(params.fname, FMODE);
    if(file == NULL)  // Subor sa nepodarilo otvorit
    {
      print_ecode(EFOPEN);
      return EXIT_FAILURE;
    }

    int e;
    Bitmap *bm = bitmap_new_from_file(file, &e);

    if(params.state == STEST)
    {
      if(e == EOK)
      {
        printf("Valid\n");
        bitmap_delete(bm);
      }
      else
      {
        printf("Invalid\n");
      }
    }
    else
    {
      if(e != EOK)
      {
        print_ecode(e);
      }
      else
      {
        int x1, y1, x2, y2, status;

        if(params.state == SVLINE)
        {
          status = find_vline(bm, &x1, &y1, &x2, &y2);
        }
        else if(params.state == SHLINE)
        {
          status = find_hline(bm, &x1, &y1, &x2, &y2);
        }
        else if(params.state == SSQUARE)
        {
          status = find_square(bm, &x1, &y1, &x2, &y2);
        }

        if(status == FOUND)
        {
          printf("%d %d %d %d\n", x1, y1, x2, y2);
        }
        else
        {
          print_ecode(ENOTFOUND);
        }

        bitmap_delete(bm);
      }
    }

    fclose(file);
  }

  return EXIT_SUCCESS;
}

/**
 * Vypise hlasenie odpovedajuce chybovemu kodu.
 * @param ecode kod chyby programu
 */
void print_ecode(int ecode)
{
  if (ecode < EOK || ecode > EUNKNOWN)
  { ecode = EUNKNOWN; }

  fprintf(stderr, "%s", ECODEMSG[ecode]);
}

/**
 * Spracuje argumenty prikazoveho riadku a vrati ich v strukture TParams.
 * Ak je format argumentov chybny, ukonci program s chybovym kodem.
 * @param argc Pocet argumentov.
 * @param argv Pole textovych retazcov s argumentami.
 * @return Vracia analyzovane argumenty prikazoveho riadku.
 */
Params get_params(int argc, char *argv[])
{
  Params result =
  {
    .fname = "",
    .ecode = EOK,
    .state = SHELP,
  };

  if(argc == 2)
  {
    if(strcmp("-h", argv[1]) == 0 || strcmp("--help", argv[1]) == 0)
    {
      result.state = SHELP; 
    }
    else
    {
      result.ecode = ECLWRONG;
    }
  }
  else if(argc == 3)
  {
    if(strcmp("--test", argv[1]) == 0)
    {
      result.state = STEST;
      result.fname = argv[2];
    }
    else if(strcmp("--vline", argv[1]) == 0)
    {
      result.state = SVLINE;
      result.fname = argv[2];
    }
    else if(strcmp("--hline", argv[1]) == 0)
    {
      result.state = SHLINE;
      result.fname = argv[2];
    }
    else if(strcmp("--square", argv[1]) == 0)
    {
      result.state = SSQUARE;
      result.fname = argv[2];
    }
    else
    {
      result.ecode = ECLWRONG;
    }
  }
  else
  {
    result.ecode = ECLWRONG;
  }

  return result;
}

/**
 * Skonvertuje int na char.
 * @param x Premienane cislo.
 * @return Znakovu hodnotu cisla.
 */
char int_to_char(int x)
{
  return x + '0';
}

/**
 * Vytvori bitmapu danych rozmerov.
 * @param rows Pocet riadkov bitmapy.
 * @param cols Pocet stlpcov bitmapy.
 * @param err Indikator chyby.
 * @return Vracia ukazovatel na bitmapu.
 */
Bitmap *bitmap_new(int rows, int cols, int *err)
{
  Bitmap *bm = malloc(sizeof(Bitmap));

  if(bm == NULL)
  {
    *err = ELOWMEM;
    return NULL;
  }

  bm->cells = malloc(sizeof(char)*rows*cols);

  if(bm->cells == NULL)
  {
    *err = ELOWMEM;
    free(bm);
    return NULL;
  }

  bm->rows = rows;
  bm->cols = cols;
  *err = EOK;

  return bm;
}

/**
 * Nacita bitmapu zo suboru.
 * @param file Ukazovatel na zdrojov subor s bitmapov.
 * @param err Indikator chyby.
 * @return Vracia ukazovatel na bitmapu.
 */
Bitmap *bitmap_new_from_file(FILE *file, int *err)
{
  int bm_rows, bm_cols;
  int e = fscanf(file, "%d %d", &bm_rows, &bm_cols);

  if(e == EOF)
  {
    *err = ESHORTFILE;
    return NULL;
  }
  else if(e != 2 || bm_rows < 1 || bm_cols < 1)
  {
    *err = EBADVAL;
    return NULL;
  }

  if(bm_cols == 1 && bm_rows == 1)
  {
    *err = EPOINTONLY;
    return NULL;
  }

  // Kontrola pixelov
  int c;
  Bitmap *bm = bitmap_new(bm_rows, bm_cols, err);
  if(*err != EOK && bm == NULL)
  {
    return NULL;
  }

  for(int i=0; i<(bm_rows*bm_cols); i++)
  {
    e = fscanf(file, "%d", &c);

    if(e != 1)
    {
      if(e == EOF)
      {
        *err = ESHORTFILE;
        return NULL;
      }
      else
      {
        *err = EBADVAL;
        return NULL;
      }
    }
    else if(c != 0 && c != 1)
    {
      *err = EBADVAL;
      return NULL;
    }

    bm->cells[i] = int_to_char(c);
  }

  return bm;
}

/**
 * Uvolni pamat zabranu bitmapov.
 * @param bm Ukazovatel na bitmapu.
 */
void bitmap_delete(Bitmap *bm)
{
  free(bm->cells);

  bm->cells = NULL;
  bm->rows = 0;
  bm->cols = 0;

  free(bm);
}

/**
 * Zisti farbu pixelu na zadanych suradniciach bitmapy.
 * @param bm Ukazovatel na bitmapu.
 * @param x Riadok bitmapy.
 * @param y Stlpec bitmapy.
 * @return Vracia farbu pixelu.
 */
char getcolor(Bitmap *bm, int x, int y)
{
  return bm->cells[bm->cols*x+y];
}

/**
 * Vyhlada najdlhsiu zvislu usecku v obrazku.
 * @param bm Ukazovatel na bitmapu.
 * @param x1 Riadok zaciatocneho bodu najdlhsej usecky.
 * @param y1 Stlpec zaciatocneho bodu najdlhsej usecky.
 * @param x2 Riadok koncoveho bodu najdlhsej usecky.
 * @param y2 Stlpec koncoveho bodu najdlhsej usecky.
 * @return Vracia stavovy kod.
 */
int find_vline(Bitmap *bm, int *x1, int *y1, int *x2, int *y2)
{
  int cx1, cy1, cx2, cy2, c_len;
  int longest = 0;

  for(int j=0; j<bm->cols; j++)
  {
    for(int i=0; i<bm->rows; i++)
    {
      //Vynechat hodnoty, na ktorych nemoze zacinat najdlhsia usecka
      if(i < (bm->rows - longest))
      {
        if(getcolor(bm, i, j) == '1')
        {
          cx1 = cx2 = i;
          cy1 = cy2 = j;
          c_len = 1;
          i++;

          while((i < bm->rows) && (getcolor(bm, i, j) == '1'))
          {
            c_len++;
            cx2 = i;
            cy2 = j;
            i++;
          }

          if(c_len > longest)
          {
            longest = c_len;
            *x1 = cx1;
            *y1 = cy1;
            *x2 = cx2;
            *y2 = cy2;
          }
        }
      }
    }
  }

  if(longest == 0 || longest == 1)
    return NOT_FOUND;

  return FOUND;
}

/**
 * Vyhlada najdlhsiu vodorovnu usecku v obrazku.
 * @param bm Ukazovatel na bitmapu.
 * @param x1 Riadok zaciatocneho bodu najdlhsej usecky.
 * @param y1 Stlpec zaciatocneho bodu najdlhsej usecky.
 * @param x2 Riadok koncoveho bodu najdlhsej usecky.
 * @param y2 Stlpec koncoveho bodu najdlhsej usecky.
 * @return Vracia stavovy kod.
 */
int find_hline(Bitmap *bm, int *x1, int *y1, int *x2, int *y2)
{
  int cx1, cy1, cx2, cy2, c_len;
  int longest = 0;

  for(int i=0; i<bm->rows; i++)
  {
    for(int j=0; j<bm->cols; j++)
    {
      //Vynechat hodnoty, na ktorych nemoze zacinat najdlhsia usecka
      if(j < (bm->cols - longest))
      {
        if(getcolor(bm, i, j) == '1')
        {
          cx1 = cx2 = i;
          cy1 = cy2 = j;
          c_len = 1;
          j++;

          while((j < bm->cols) && (getcolor(bm, i, j) == '1'))
          {
            c_len++;
            cx2 = i;
            cy2 = j;
            j++;
          }

          if(c_len > longest)
          {
            longest = c_len;
            *x1 = cx1;
            *y1 = cy1;
            *x2 = cx2;
            *y2 = cy2;
          }
        }
      }
    }
  }

  if(longest == 0 || longest == 1)
    return NOT_FOUND;

  return FOUND;
}

/**
 * Vyhlada najvacsi stvorec v obrazku.
 * @param bm Ukazovatel na bitmapu.
 * @param x1 Riadok zaciatocneho bodu najvacsieho stvorca.
 * @param y1 Stlpec zaciatocneho bodu najvacsieho stvorca.
 * @param x2 Riadok koncoveho bodu najvacsieho stvorca.
 * @param y2 Stlpec koncoveho bodu najvacsieho stvorca.
 * @return Vracia stavovy kod.
 */
int find_square(Bitmap *bm, int *x1, int *y1, int *x2, int *y2)
{
  // OX/OY - origin x/y, cuhx - current upper horizontal x,...
  int ox, oy, k, cuhx, cuhy, clvx, clvy, clhx, clhy, crvx, crvy;
  int longest = 0;

  // UH/LH - upper/lower horizontal, LV/RV - left/right vertical
  bool uh, lh, lv, rv;
  uh = lh = lv = rv = true;

  int max_len = 2;
  bool do_search;

  for(int i=0; i<bm->rows; i++)
  {
    for(int j=0; j<bm->cols; j++)
    {
      if(getcolor(bm, i, j) == '1')
      {
        do_search = true;
        ox = i;
        oy = j;

        while(do_search)
        {
          if((oy + (max_len-1) < bm->cols) && (ox + (max_len-1) < bm->rows))
          {
            // Test hornej vodorovnej strany stvorca
            for(k=0; k<max_len-1; k++)
            {
              if(getcolor(bm, i, oy+k+1) == '1')
              {
                uh = true;
                cuhy = oy+k+1;
                cuhx = ox;
              }
              else
              {
                do_search = false;
                uh = false;
                break;
              }
            }
            // Test lavej zvislej strany stvorca
            for(k=0; k<max_len-1; k++)
            {
              if(getcolor(bm, ox+k+1, oy) == '1')
              {
                lv = true;
                clvx = ox+k+1;
                clvy = oy;
              }
              else
              {
                do_search = false;
                lv = false;
                break;
              }
            }
            if(uh && lv)
            {
              // Test spodnej vodorovnej strany stvorca
              for(k=0; k<max_len-1; k++)
              {
                if(getcolor(bm, clvx, clvy+k+1) == '1')
                {
                  lh = true;
                  clhx = clvx;
                  clhy = clvy+k+1;
                }
                else
                {
                  lh = false;
                  break;
                }
              }

              // Spodna horizontalna strana musi byt najdena
              if(lh)
              {
                // Test pravej zvislej strany stvorca
                for(k=0; k<max_len-1; k++)
                {
                  if(getcolor(bm, cuhx+k+1, cuhy) == '1')
                  {
                    rv = true;
                    crvx = cuhx+k+1;
                    crvy = cuhy;
                  }
                  else
                  {
                    rv = false;
                    break;
                  }
                }
              }

              //Ak su vsetky strany najdene
              if(uh && lh && lv && rv && clhx == crvx && clhy == crvy)
              {
                longest = max_len;
                *x1 = ox;
                *y1 = oy;
                *x2 = crvx;
                *y2 = crvy;
              }
              max_len++;
            }
          }
          else
          {
            do_search = false;
          }
        }
      }
    }
  }

  if(longest == 0 || longest == 1)
  {
    return NOT_FOUND;
  }

  return FOUND;
}

/** Koniec proj3.c */
