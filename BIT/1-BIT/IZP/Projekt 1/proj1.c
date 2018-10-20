/*
 * Subor:   proj1.c
 * Datum:   2013/10/15
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Pocitanie slov
 * Popis:   Program nacita zo stdin text a vypise pocet slov, ktore obsahuju
 *          znak zadany ako parameter programu.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

/** Skrateny zapis unsigned int*/
typedef unsigned int uint;

/** Struktura obsahujuca hodnoty parametrov prikazoveho riadku */
typedef struct params
{
  char ch;                //Hladany znak
  uint position;          //Pozicia vyhldavaneho znaku v slove
  int ecode;              //Chybovy kod programu, odpoveda vyctu tecodes
  int state;              //Stavovy kod programu, odpoveda vyctu tstates
  bool debug;             //Ladiaci rezim vypnuty/zapnuty
} TParams;

/** Prototypy funkcii */
bool isUpper(int c);
bool isAlpha(int c);
bool isNum(int c);
bool isWordChar(int c);
uint strToUInt(const char *str, uint *num);
uint powax(uint x, uint a);
void readStdin(TParams *params);
uint analyzePosition(uint cpos, uint ppos, bool *found);

/** Kody chyb programu */
enum tecodes
{
  EOK = 0,     //Bez chyby
  ECLWRONG,    //Chybny prikazovy riadok
  EOVERFLOW,   //Pretecenie
  ECONVERSION, //Chyba prevodu retazca na cislo
  EZERO,       //Zadana nula
  EUNKNOWN,    //Neznama chyba
};

/** Stavove kody programu */
enum tstates
{
  SHELP = 0,   //Napoveda
  SSPEC,       //Vyhladavanie specifickeho znaku
  SNUM,        //Vyhladavanie cisla 0-9
  SUPP,        //Vyhladavanie znaku A-Z
  SALL,        //Vyhladavanie vsetkych znakov
};

/** Chybove hlalenia odpovedajuce chybovym kodom */
const char *ECODEMSG[] =
{
  [EOK]         = "Vsetko v poriadku!\n",
  [ECLWRONG]    = "Chybne parametre prikazoveho riadku!\n",
  [EOVERFLOW]   = "Doslo k preteceniu pri konverzii retazca na cele cislo!\n",
  [ECONVERSION] = "Doslo k chybe pri konverzii retazca na cislo!\n",
  [EZERO]       = "Parameter N nemoze byt rovny 0!\n",
  [EUNKNOWN]    = "Nieco sa pokazilo!\n",
};

/** Text napovedy */
const char *HELPMSG =
  "Program: Pocitanie slov\n"
  "Autor:   Daniel Klimaj (c) 2013\n\n"
  "Program nacita zo stdin text a vypise pocet slov, ktore obsahuju "
  "znak zadany ako parameter programu.\n\n"
  "Pouzitie: proj1 --help\n"
  "          proj1 X [N] [-d]\n\n"
  "Parametre:\n"
  "  --help, -h  Vypise tento text\n"
  "  X           Reprezentuje znak hladany v slovach.\n"
  "              Specialne znaky:\n"
  "                : - cislo 0-9\n"
  "                ^ - velke pismeno A-Z\n"
  "                . - lubovolny znak\n"
  "  N           Poloha hladaneho znaku v slove, kladne cele cislo\n"
  "  -d          Zapne ladiaci rezim, ktory postupne vypisuje najdene slova\n"
;

/** Maximalna dlzka retazca v ladiacom rezime (debug) */
const uint MAX_LENGTH = 80;

/**
 * Vypise chybove hlasenie odpovedajuce chybovemu kodu.
 * @param ecode kod chyby programu
 */
void printECode(int ecode)
{
  if (ecode < EOK || ecode > EUNKNOWN)
  { ecode = EUNKNOWN; }

  fprintf(stderr, "%s", ECODEMSG[ecode]);
}

/**
 * Spracuje argumenty prikazoveho riadku a vrati ich v strukture TParams.
 * Ak je format argumentov chybny, ukonci program s chybovym kodem.
 * @param argc Pocet argumentov.
 * @param argv Pole textovych retazcov s argumenty.
 * @return Vracia analyzovane argumenty prikazoveho riadku.
 */
TParams getParams(int argc, char *argv[])
{
  TParams result =
  {
    .ch = ' ',
    .position = 0,
    .ecode    = EOK,
    .state    = SHELP,
    .debug    = false
  };

  uint pos = 0;

  if(argc == 2)
  {
    /** Vypis napovedy */
    if((strcmp("-h", argv[1]) == 0) || (strcmp("--help", argv[1]) == 0))
      result.state = SHELP;
    else if(strlen(argv[1]) == 1)
    {
      if(strlen(argv[1]) == 1 && isWordChar(argv[1][0]))
      {
        result.ch    = argv[1][0];
        result.state = SSPEC;
      }
      else if(strlen(argv[1]) == 1 && argv[1][0] == '.')
        result.state = SALL;

      else if(strlen(argv[1]) == 1 && argv[1][0] == '^')
        result.state = SUPP;

      else if(strlen(argv[1]) == 1 && argv[1][0] == ':')
        result.state = SNUM;
      else
        result.ecode = ECLWRONG;
    }
    else
      result.ecode = ECLWRONG;
  }
  else if(argc == 3)
  {
    if(strlen(argv[1]) != 1)
      result.ecode = ECLWRONG;
    else if(strlen(argv[1]) == 1 && isWordChar(argv[1][0]) && strcmp("-d",
      argv[2]) == 0)
    {
      result.ch    = argv[1][0];
      result.state = SSPEC;
      result.debug = true;
    }
    else if(strlen(argv[1]) == 1 && argv[1][0] == '.' && strcmp("-d",
      argv[2]) == 0)
    {
      result.state = SALL;
      result.debug = true;
    }
    else if(strlen(argv[1]) == 1 && argv[1][0] == '^' && strcmp("-d",
      argv[2]) == 0)
    {
      result.state = SUPP;
      result.debug = true;
    }
    else if(strlen(argv[1]) == 1 && argv[1][0] == ':' && strcmp("-d",
      argv[2]) == 0)
    {
      result.state = SNUM;
      result.debug = true;
    }
    else if(strlen(argv[1]) == 1 && isWordChar(argv[1][0]) && strToUInt(argv[2],
      &pos) == EOK)
    {
      result.ch       = argv[1][0];
      result.state    = SSPEC;
      result.position = pos;
    }

    else if(strlen(argv[1]) == 1 && argv[1][0] == '.' && strToUInt(argv[2],
      &pos) == EOK)
    {
      result.state    = SALL;
      result.position = pos;
    }
    else if(strlen(argv[1]) == 1 && argv[1][0] == '^' && strToUInt(argv[2],
      &pos) == EOK)
    {
      result.state    = SUPP;
      result.position = pos;
    }
    else if(strlen(argv[1]) == 1 && argv[1][0] == ':' && strToUInt(argv[2],
      &pos) == EOK)
    {
      result.state    = SNUM;
      result.position = pos;
    }
    else if(strToUInt(argv[2], &pos) == EOVERFLOW)
      result.ecode = EOVERFLOW;
    else if(strToUInt(argv[2], &pos) == ECONVERSION)
      result.ecode = ECONVERSION;
    else if(strToUInt(argv[2], &pos) == EZERO)
      result.ecode = EZERO;
  }
  else if(argc == 4)
  {
    if(strlen(argv[1]) != 1)
      result.ecode = ECLWRONG;
    else if(strlen(argv[1]) == 1 && isWordChar(argv[1][0]) && strcmp("-d",
      argv[2]) == 0 && strToUInt(argv[3], &pos) == EOK)
    {
      result.ch       = argv[1][0];
      result.state    = SSPEC;
      result.position = pos;
      result.debug    = true;
    }
    else if(strlen(argv[1]) == 1 && argv[1][0] == '.' && strcmp("-d",
      argv[2]) == 0 && strToUInt(argv[3], &pos) == EOK)
    {
      result.state    = SALL;
      result.position = pos;
      result.debug    = true;
    }
    else if(strlen(argv[1]) == 1 && argv[1][0] == '^' && strcmp("-d",
      argv[2]) == 0 && strToUInt(argv[3], &pos) == EOK)
    {
      result.state    = SUPP;
      result.position = pos;
      result.debug    = true;
    }
    else if(strlen(argv[1]) == 1 && argv[1][0] == ':' && strcmp("-d",
      argv[2]) == 0 && strToUInt(argv[3], &pos) == EOK)
    {
      result.state    = SNUM;
      result.position = pos;
      result.debug    = true;
    }
    else if(strlen(argv[1]) == 1 && isWordChar(argv[1][0]) && strToUInt(argv[2],
      &pos) == EOK && strcmp("-d", argv[3]) == 0)
    {
      result.ch       = argv[1][0];
      result.state    = SSPEC;
      result.position = pos;
      result.debug    = true;
    }

    else if(strlen(argv[1]) == 1 && argv[1][0] == '.' && strToUInt(argv[2],
      &pos) == EOK && strcmp("-d", argv[3]) == 0)
    {
      result.state    = SALL;
      result.position = pos;
      result.debug    = true;
    }
    else if(strlen(argv[1]) == 1 && argv[1][0] == '^' && strToUInt(argv[2],
      &pos) == EOK && strcmp("-d", argv[3]) == 0)
    {
      result.state    = SUPP;
      result.position = pos;
      result.debug    = true;
    }
    else if(strlen(argv[1]) == 1 && argv[1][0] == ':' && strToUInt(argv[2],
      &pos) == EOK && strcmp("-d", argv[3]) == 0)
    {
      result.state    = SNUM;
      result.position = pos;
      result.debug    = true;
    }
    else if(!strcmp("-d", argv[2]) == 0 && strToUInt(argv[2], &pos) ==EOVERFLOW)
      result.ecode = EOVERFLOW;
    else if(!strcmp("-d", argv[2]) == 0 && strToUInt(argv[2], &pos) == ECONVERSION)
      result.ecode = ECONVERSION;
    else if(!strcmp("-d", argv[2]) == 0 && strToUInt(argv[2], &pos) == EZERO)
      result.ecode = EZERO;
    else if(!strcmp("-d", argv[3]) == 0 && strToUInt(argv[3], &pos) == EOVERFLOW)
      result.ecode = EOVERFLOW;
    else if(!strcmp("-d", argv[3]) == 0 && strToUInt(argv[3], &pos) == ECONVERSION)
      result.ecode = ECONVERSION;
    else if(!strcmp("-d", argv[3]) == 0 && strToUInt(argv[3], &pos) == EZERO)
      result.ecode = EZERO;
    else if(strcmp("-d", argv[2]) == 0 && strcmp("-d", argv[3]) == 0)
      result.ecode = ECLWRONG;
  }
  /** Chybne zadane parametre prikazoveho riadku*/
  else
  {
    result.ecode = ECLWRONG;
  }

  return result;
}

/////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
  TParams params = getParams(argc, argv);

  if (params.ecode != EOK)
  {
    printECode(params.ecode);
    return EXIT_FAILURE;
  }

  if (params.state == SHELP)
  {
    printf("%s", HELPMSG);
    return EXIT_SUCCESS;
  }
  else
    readStdin(&params);

  return EXIT_SUCCESS;
}

/**
 * Vyhodnoti znak, ci je A-Z.
 * @param  c Ascii hodnota znaku.
 * @return Vracia true ak znak je A-Z.
 */
bool isUpper(int c)
{
  if(c >= 'A' && c <= 'Z')
    return true;
  return false;
}

/**
 * Vyhodnoti znak, ci je A-Za-z.
 * @param  c Ascii hodnota znaku.
 * @return Vracia true ak znak je A-Za-z.
 */
bool isAlpha(int c)
{
  if(isUpper(c) || (c >= 'a' && c <= 'z'))
    return true;
  return false;
}

/**
 * Vyhodnoti znak, ci je 0-9.
 * @param  c Ascii hodnota znaku.
 * @return Vracia true ak znak je 0-9.
 */
bool isNum(int c)
{
  if(c >= '0' && c<= '9')
    return true;
  return false;
}

/**
 * Vyhodnoti znak, ci je A-Za-z, 0-9, '-', '_', tj. platny znak slova.
 * @param  c Ascii hodnota znaku.
 * @return Vracia true ak znak je A-Za-z, 0-9, '-', '_'.
 */
bool isWordChar(int c)
{
  if(isAlpha(c) || isNum(c) || c == '-' || c == '_')
    return true;
  return false;
}

/**
 * Prevod textoveho retazca na cele cislo.
 * @param  *str textovy retazec.
 * @param  *uint celocislena premenna, do ktorej sa ulozi vysledok.
 * @return Vracia chybovy kod:
 *          EOK - bez preblemov
 *          ECONVERSION - chyba pri prevode 
 *          EOVERFLOW - cislo vacsie ako INT_MAX
 *          EZERO - vysledok prevodu je rovny 0.
 */
uint strToUInt(const char *str, uint *num)
{
  uint sum, nums, value;
  sum = nums = value = 0;

  if(!isNum(str[0]))
    return ECONVERSION;
  else
  {
    for(uint i=0; i<strlen(str); i++)
    {
      if(isNum(str[i]))
        nums++;
      else
        break;    // ukoncit vyhladavanie cisel, ak sa nasiel neciselny znak
    }

    for(uint i=0; i<nums; i++)
    {
      value = (uint)(str[i] - '0') * powax(10, nums-i-1);

      if((INT_MAX - sum) < value)
        return EOVERFLOW; // doslo k preteceniu - cislo > INT_MAX
      else
        sum += value;
    }

    if(sum == 0)
      return EZERO;       //Vysledok prevodu je 0

    *num = sum;
    return EOK;
  }
}

/**
 * Vypocita x-tu mocninu cisla a.
 * @param  a umocnovane cislo.
 * @param  x exponent.
 * @return Vracia x-tu mocninu cisla a.
 */
uint powax(uint a, uint x)
{
  if(x == 0)
    return 1;
  
  uint pow = a;
  for(uint i=1; i<x; i++)
    pow *= a;

  return pow;
}

/**
 * Nacita text zo standardneho vstupu a na zaklade zadanych parametrov vypise
 * vysledok.
 * @param  *params parametre programu.
 */
void readStdin(TParams *params)
{
  char c;
  bool word_found = false;
  bool char_found = false;
  uint counter    = 0;
  uint char_pos   = 1;

  while((c = getchar()) != EOF)
  {
    if(isWordChar(c))
    {
      if(!word_found)
        word_found = true;

      if(!char_found)
      {
        if((params->state == SSPEC) && (c == params->ch))
        {
          counter += analyzePosition(char_pos, params->position, &char_found);
        }
        else if((params->state == SNUM) && isNum(c))
        {
          counter += analyzePosition(char_pos, params->position, &char_found);
        }
        else if((params->state == SUPP) && isUpper(c))
        {
          counter += analyzePosition(char_pos, params->position, &char_found);
        }
        else if((params->state == SALL) && isWordChar(c))
        {
          counter += analyzePosition(char_pos, params->position, &char_found);
        }
      }

      if(params->debug)
      {
        if(char_pos <= MAX_LENGTH)
          putchar(c);
      }

      char_pos++;
    }
    else if(!isWordChar(c) && word_found)
    {
      if(params->debug)
        putchar('\n');

      word_found = false;
      char_found = false;
      char_pos   = 1;
    }
  }

  printf("%d\n", counter);
}


/**
 * Zisti ci sa pozicia znaku zhoduje so zadanou poziciou.
 * @param  cpos pozicia znaku v slove.
 * @param  ppos parametrom zadana pozicia.
 * @param  *found pointer na priznak siganlizujuci, ze znak v slove bol najdeny.
 * @return 1 ak znak bol najdeny.
 */
uint analyzePosition(uint cpos, uint ppos, bool *found)
{
  //hladanie znaku na danej pozicii
  if((ppos > 0) && (ppos == cpos))
  {
    *found = true;
    return 1;
  }
  //parameter N nezadany
  if(ppos == 0)
  {
    *found = true;
    return 1;
  }
  return 0;
}

/** Koniec proj1.c */
