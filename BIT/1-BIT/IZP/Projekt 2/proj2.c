/*******************************************************************************
 * Soubor:  proj2.c
 * Datum:   2013/11/25
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Iteracne vypocty
 * Popis:   Program vypocita a vypise uhly obecneho trojuholniku zo zadanych
 *          suradnic, arcsin a druhu odmocninu zadaneho cisla.
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <stdbool.h>

#define EPS    1e-12
#define MY_PI  3.14159265359
#define MY_NAN -(0.0/0.0)

// Kody chyb programu
enum tecodes
{
  EOK = 0,     //Bez chyby
  ECLWRONG,    //Chybny parameter prikazoveho riadku
  EOVRFLW,     //Pretecenie
  ECONVER,     //Chyba pri konverzii
  ESAMEPTS,    //Rovnake suradnice bodov
  ELINE,       //Body tvoria usecku
  EUNKNOWN,    //Neznama chyba
};

// Stavove kody programu
enum tstates
{
  SHELP,         //Napoveda
  SASIN,         //Arcsin
  SSQRT,         //Odmocnina
  STRIANGLE      //Trojuholnik
};


// Chybove hlasenia
const char *ECODEMSG[] =
{
  [EOK]         = "Vsetko v poriadku!\n",
  [ECLWRONG]    = "Chybne parametre prikazoveho riadku!\n",
  [EOVRFLW]     = "Doslo k preteceniu pri konverzii!\n",
  [ECONVER]     = "Nemozno previest parameter na typ double!\n",
  [ESAMEPTS]    = "Niektore dva body trojuholnika maju rovnake suradnice!\n",
  [ELINE]       = "Body lezia na jednej usecke!\n",
  [EUNKNOWN]    = "Nieco sa pokazilo!\n",
};

// Text napovedy
const char *HELPMSG =
  "Program: Iteracne vypocty\n"
  "Autor:   Daniel Klimaj (c) 2013\n\n"

  "Program vypocita a vypise uhly obecneho trojuholniku zo zadanych\n"
  "suradnic, arcsin a druhu odmocninu zadaneho cisla.\n\n"

  "Pouzitie:\n"
  "  proj2 --help\n"
  "  proj2 --asin x\n"
  "  proj2 --sqrt x\n"
  "  proj2 --triangle AX AY BX BY CX CY\n\n"

  "Parametre:\n"
  "  --help, -h                    Vypise tento text\n"
  "  --asin x                      Vypocita arcus sinus pre x\n"
  "  --sqrt x                      Vypocita druhu odmocninu z x\n"
  "  --triangle AX AY BX BY CX CY  Vypocita uhly trojuholniku zadaneho \n"
  "                                suradnicami AX AY BX BY CX CY\n\n"
;

// Struktura obsahujuca hodnoty parametrov prikazoveho riadku
typedef struct params
{
  double N;          // Hodnota N z prikoazoveho riadku
  double coords[6];  // Suradnice bodov trojuholniku
  int ecode;         // Chybový kod programu, odpoveda vyctu tecodes
  int state;         // Stavový kód programu, odpovídá výètu tstates
} TParams;

// Struktura obsahujuca suradnice bodov trojuholnika
typedef struct point
{
  double x;          // Suradnica X
  double y;          // Suradnica Y
} TPoint;

/***********************
 * Prototypy funkcii
***********************/
void printECode(int ecode);
TParams getParams(int argc, char *argv[]);

//Kontrolne funkcie
bool is_valid_arg(const char *arg);
bool is_num(int c);
bool is_signed(double x);

//Matematicke funkcie
double my_fabs(double x);
double my_sqrt(double x);
double my_asin(double x);

//Alokacia a dealokacia bodov
TPoint *point_new(double x, double y);
void point_delete(TPoint *point);

//Funkcie pre riesenie trojuholnika
int solve_triangle(TPoint *pt_a, TPoint *pt_b, TPoint *pt_c);
int check_points(TPoint *pt_a, TPoint *pt_b, TPoint *pt_c);
bool check_line(TPoint *pt_m, TPoint *pt_n, TPoint *pt_w);
double get_line_length_sqr(TPoint *m, TPoint *n);
double get_angle_val(double off, double adj1, double adj2);

/*******************************************************************************
 * Main
*******************************************************************************/
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
  }
  else if(params.state == SASIN)
  {
    printf("%.10e\n", my_asin(params.N));
  }
  else if(params.state == SSQRT)
  {
    printf("%.10e\n", my_sqrt(params.N));
  }
  else if(params.state == STRIANGLE)
  {
    TPoint *a_point = point_new(params.coords[0], params.coords[1]);
    TPoint *b_point = point_new(params.coords[2], params.coords[3]);
    TPoint *c_point = point_new(params.coords[4], params.coords[5]);

    //Je trojuholnik spravne zadany?
    int rslt = solve_triangle(a_point, b_point, c_point);

    point_delete(a_point);
    point_delete(b_point);
    point_delete(c_point);

    if(rslt != EOK)
    {
      printECode(rslt);
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

/**
 * Vypise hlasenie odpovedajuce chybovemu kodu.
 * @param ecode kod chyby programu
 */
void printECode(int ecode)
{
  if (ecode < EOK || ecode > EUNKNOWN)
  { ecode = EUNKNOWN; }

  fprintf(stderr, "%s", ECODEMSG[ecode]);
}

/**
 * Spracuje argumenty prikazoveho riadku a vrati je ve struktre TParams.
 * Ak je format argumentov chybny, ukonci program s chybovym kodem.
 * @param argc Pocet argumentù.
 * @param argv Pole textových retazcov s argumenty.
 * @return Vracia analyzovane argumenty prikazoveho riadku.
 */
TParams getParams(int argc, char *argv[])
{
  TParams result =
  {
    .N = 0,
    .coords = {0, 0, 0, 0, 0, 0},
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
    if(!is_valid_arg(argv[2]))
    {
      result.ecode = ECONVER;
    }
    else
    {
      if(strcmp("--asin", argv[1]) == 0)
      {
        result.N = strtod(argv[2], NULL);
        result.state = SASIN;
      }
      else if(strcmp("--sqrt", argv[1]) == 0)
      {
        result.N = strtod(argv[2], NULL);
        result.state = SSQRT;
      }
      else
      {
        result.ecode = ECLWRONG;
      }

      if(result.N > DBL_MAX)
      {
        result.ecode = EOVRFLW;
      }
    }
  }
  else if(argc == 8)
  {
    if(!is_valid_arg(argv[2]) || !is_valid_arg(argv[3]) ||
    !is_valid_arg(argv[4]) || !is_valid_arg(argv[5]) ||
    !is_valid_arg(argv[6]) || !is_valid_arg(argv[7]))
    {
      result.ecode = ECONVER;
    }
    else
    {
      if(strcmp("--triangle", argv[1]) == 0)
      {
        for(int i=0, j=2; i<6; i++, j++)
        {
          result.coords[i] = strtod(argv[j], NULL);
        }
        result.state = STRIANGLE;
      }
      else
      {
        result.ecode = ECLWRONG;
      }

      for(int i=0; i<6; i++)
      {
        if(result.coords[i] > DBL_MAX)
        {
          result.ecode = EOVRFLW;
          break;
        }
      }
    }
  }
  else
  {
    result.ecode = ECLWRONG;
  }

  return result;
}

/**
 * Zistuje, ci argument moze byt prevedeny na typ double.
 * @param arg analyzovany argument.
 * @return Vracia true ak je argument prevoditelny.
 */
bool is_valid_arg(const char *arg)
{
  if(!is_num(arg[0]) && arg[0] != '.' && arg[0] != '-')
    return false;

  if(arg[0] == '.')
    if(!is_num(arg[1]))
      return false;

  if(arg[0] == '-')
  {
    if(arg[1] == '.' && !is_num(arg[2]))
      return false;
  }

  return true;
}

/**
 * Zistuje, ci je znak cislica.
 * @param c analyzovany znak.
 * @return Vracia true ak je znak c cislica.
 */
bool is_num(int c)
{
  if(c >= '0' && c<= '9')
    return true;

  return false;
}

/**
 * Zistuje, ci je cislo znamienkove.
 * @param x analyzovane cislo.
 * @return Vracia true ak je cislo x znamienkove.
 */
bool is_signed(double x)
{
  if(x < 0)
    return true;

  return false;
}

/**
 * Zistuje absolutnu hodnotu cisla.
 * @param x analyzovane cislo.
 * @return Vracia absolutnu hodnotu cisla x.
 */
double my_fabs(double x)
{
  return x < 0 ? -x : x;
}

/**
 * Vypocitava druhu odmocninu cisla.
 * @param x odmocnovane cislo.
 * @return Vracia druhu odmocninu cisla x.
 */
double my_sqrt(double x)
{
  double val = 1.0;
  double delta = 1.0;
  double old_val;

  if(x == 0.0)
    return x;
  
  if(x < 0.0) // neplatny vstup
    return MY_NAN;

  if (x < 1.0) // prvy odhad odmocniny musi byt mensi ako x
    while(val > x)
      val /= 10.0;

  while(my_fabs(delta) > EPS * my_fabs(val))
  {
    old_val = val;
    val = ((x/val)+val)/2;
    delta = old_val - val;
  }

  return val;
}

/**
 * Vypocitava arcus sinus cisla.
 * @param x vstupne cislo.
 * @return Vracia arcus sinus cisla x.
 */
double my_asin(double x)
{
  if(my_fabs(x) > 1.0) // mimo definicneho oboru arcsinu
  {
    return MY_NAN;
  }

  double xx = x * x;
  double ti = x;
  double sum = ti;
  double k = 1.0;

  // arcsin pre intervaly <-1; -0.9> a <0.9; 1>
  if(my_fabs(x) >= 0.9 && my_fabs(x) <= 1.0)
  {
    if(is_signed(x))
    {
      return -(MY_PI/2) + my_asin(my_sqrt(1-xx));
    }
    else
    {
      return (MY_PI/2) - my_asin(my_sqrt(1-xx));
    }
  }
  else // arcsin pre interval (-0.9; 0.9)
  {
    while(my_fabs(ti) > EPS * my_fabs(sum))
    {
      ti = ti * k * (k/(k+1.0)) * (xx/(k+2.0));
      sum = sum + ti;
      k = k + 2.0;
    }
  }

  if(sum == 0.0)  //nevracat -0.0
    sum = 0.0;

  return sum;
}

/**
 * Zo zadanych suradnic vytvori bod.
 * @param x suradnica x.
 * @param y suradnica y.
 * @return Vracia ukazovatel na strukturu TPoint reprezentujucu bod.
 */
TPoint *point_new(double x, double y)
{
  TPoint *point = malloc(sizeof(TPoint));
  point->x = x;
  point->y = y;

  return point;
}

/**
 * Uvolni pamat, na ktorej je ulozeny bod.
 * @param point ukazovatel na strukturu TPoint.
 */
void point_delete(TPoint *point)
{
  free(point);
  point = NULL;
}

/**
 * Vypocita strany a vnutorne uhly trojuholnika.
 * @param pt_a Ukazovatel na bod A (TPoint).
 * @param pt_b Ukazovatel na bod B (TPoint).
 * @param pt_c Ukazovatel na bod C (TPoint).
 * @return Vracia chybovy kod.
 */
int solve_triangle(TPoint *pt_a, TPoint *pt_b, TPoint *pt_c)
{
  int ecode = check_points(pt_a, pt_b, pt_c);

  if(ecode != EOK)
    return ecode;

  double line_ab = get_line_length_sqr(pt_a, pt_b);
  double line_bc = get_line_length_sqr(pt_b, pt_c);
  double line_ac = get_line_length_sqr(pt_a, pt_c);

  double angle_a = get_angle_val(line_bc, line_ac, line_ab);
  double angle_b = get_angle_val(line_ac, line_bc, line_ab);
  double angle_c = get_angle_val(line_ab, line_bc, line_ac);

  //Vypis velkosti uhlov v radianoch
  printf("%.10e\n%.10e\n%.10e\n", angle_a, angle_b, angle_c);

  return EOK;
}

/**
 * Kontrola spravnosti zadanych bodov.
 * @param a Ukazovatel na bod A (TPoint).
 * @param b Ukazovatel na bod B (TPoint).
 * @param c Ukazovatel na bod C (TPoint).
 * @return Vracia chybovy kod.
 */
int check_points(TPoint *a, TPoint *b, TPoint *c)
{
  if(((a->x == b->x) && (a->y == b->y)) || ((b->x == c->x) && (b->y == c->y))
  || ((c->x == a->x) && (c->y == a->y)))
  {
    return ESAMEPTS;
  }
  else if(((a->x == b->x) && (b->x == c->x)) || ((a->y == b->y)
  && (b->y == c->y)))
  {
    return ELINE;
  }
  else if(check_line(a, b, c) || check_line(b, a, c) || check_line(c, a, b))
  {
    return ELINE;
  }
  else
  {
    return EOK;
  }
}

/**
 * Zistuje, ci bod W lezi na usecke MN.
 * @param m Ukazovatel na bod M (TPoint).
 * @param n Ukazovatel na bod N (TPoint).
 * @param w Ukazovatel na bod W (TPoint).
 * @return Vracia true, ak bod W lezi na usecke MN.
 */
bool check_line(TPoint *m, TPoint *n, TPoint *w)
{
  double t1, t2;
  t1 = (w->x - m->x)/(n->x - m->x);
  t2 = (w->y - m->y)/(n->y - m->y);

  if(t1 >= 0 && t1 <= 1 && t1 == t2)
    return true;

  return false;
}

/**
 * Zistuje vzajomnu polohu bodov a vypocita vzdialenost medzi nimi.
 * @param m Ukazovatel na bod M (TPoint).
 * @param n Ukazovatel na bod M (TPoint).
 * @return Vracia druhu mocninu vzdialenosti dvoch bodov.
 */
double get_line_length_sqr(TPoint *m, TPoint *n)
{
  TPoint *w = point_new(0.0, 0.0);
  double rslt = 0.0;

  if(m->x > n->x && m->y > n->y)            //LEFT, BELOW
  {
    w->x = m->x;
    w->y = n->y;
    double s = w->x - n->x;
    double t = m->y - w->y;
    rslt = s*s + t*t;
  }
  else if(m->x > n->x && m->y < n->y)       //LEFT, ABOVE
  {
    w->x = n->x;
    w->y = m->y;
    double s = m->x - w->x;
    double t = n->y - w->y;
    rslt = s*s+ t*t;
  }
  else if(m->x > n->x && m->y == n->y)      //LEFT, EQUAL
  {
    rslt = (m->x - n->x) * (m->x - n->x);
  }
  else if(m->x < n->x && m->y > n->y)       //RIGHT, BELOW
  {
    w->x = m->x;
    w->y = n->y;
    double s = n->x - w->x;
    double t = m->y - w->y;
    rslt = s*s + t*t;
  }
  else if(m->x < n->x && m->y < n->y)       //RIGHT, ABOVE
  {
    w->x = n->x;
    w->y = m->y;
    double s = w->x - m->x;
    double t = n->y - w->y;
    rslt = s*s + t*t;
  }
  else if(m->x < n->x && m->y == n->y)      //RIGHT, EQUAL
  {
    rslt = (n->x - m->x) * (n->x - m->x);
  }
  else if(m->x == n->x && m->y > n->y)      //EQUAL, BELOW
  {
    rslt = (m->y - n->y) * (m->y - n->y);
  }
  else if(m->x == n->x && m->y < n->y)      //EQUAL, ABOVE
  {
    rslt = (n->y - m->y) * (n->y - m->y);
  }
  else                                      // EQUAL, EQUAL
  {
    rslt = MY_NAN;
  }

  point_delete(w);
  return rslt;
}

/**
 * Vypocita vnutorne uhly trojuholnika.
 * @param off Protilahla strana k uhlu.
 * @param adj1 Prva prilahla strana k uhlu.
 * @param adj2 Druha prilahla strana k uhlu.
 * @return Vracia velkost uhlu v radianoch.
 */
double get_angle_val(double off, double adj1, double adj2)
{
  double x = (off - adj1 - adj2) / -(2 * my_sqrt(adj1) * my_sqrt(adj2));

  return MY_PI/2 - my_asin(x);
}

/** Koniec proj2.c */
