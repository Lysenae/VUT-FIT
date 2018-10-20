/* ************************************************************************** *
 * Projekt: POS #2 - Shell
 * Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Datum:   2017-04-10
 * Subor:   process.c
 * ************************************************************************** */

#include "process.h"

/**
 * Resetuje hodnoty struktury.
 * @param p Process
**/
static void s_p_reset(struct Process *p)
{
  p->in_file    = NULL;
  p->out_file   = NULL;
  p->args       = NULL;
  p->background = false;
  p->invalid    = false;
}

/**
 * Inicializuje hodnoty struktury.
 * @param p Process
**/
static void s_p_init(struct Process *p)
{
  s_p_reset(p);
  v_init(&p->params);
}

/**
 * Vrati opacny znak.
 * @param c znak
 * @return opacny znak
**/
static char s_p_rcpl(char r)
{
  return r == '<' ? '>' : '<';
}

/**
 * Inicializuje proces.
 * @param p Process
 * @param args retazec
**/
void p_init(struct Process *p, const char *args)
{
  s_p_init(p);
  p_set_args(p, args);
  p_set_background(p);
  p_set_file(p, '<');
  p_set_file(p, '>');
  p_parse_args(p);
}

/**
 * Zrusi hodnoty struktury.
 * @param p Process
**/
void p_destroy(struct Process *p)
{
  if(p->args != NULL)     free(p->args);
  if(p->in_file != NULL)  free(p->in_file);
  if(p->out_file != NULL) free(p->out_file);
  s_p_reset(p);
  v_destroy(&p->params);
}

/**
 * Nastavi argumenty.
 * @param p Process
 * @param args retazec
**/
void p_set_args(struct Process *p, const char *args)
{
  if(p->args != NULL)
  {
    free(p->args);
    p->args = NULL;
  }
  str_set(&p->args, args);
}

/**
 * Nastavi priznak, ci sa jedna o proces na pozadi a zmaze prislusnu cast
 * argumentov.
 * @param p Process
**/
void p_set_background(struct Process *p)
{
  char *s = str_dup(p->args);
  size_t len = strlen(s);
  size_t idx = 0;
  if(str_chrn(s, '&') > 1)
  {
    p->invalid = true;
  }
  else
  {
    for(size_t i = len-1; i>0; --i)
    {
      if(s[i] == ' ')
        continue;
      else
      {
        if(s[i] == '&')
        {
          p->background = true;
          idx = i;
        }
        break;
      }
    }
    if(idx > 0)
    {
      for(size_t i=idx; i<len; ++i)
        s[i] = '\0';
      p_set_args(p, str_trim(s));
    }
  }
}

/**
 * Nastavi stdin/stdout subor procesu
 * @param p Process
 * @param c znak urcujuci premserovanie I/O
**/
void p_set_file(struct Process *p, char c)
{
  if(c != '<' && c != '>')
    return;

  char *s = str_dup(p->args);
  char f[MAX_FNAME];
  memset(f, 0, MAX_FNAME);
  int len = strlen(s);
  int occ = str_chrn(s, c);
  int idx1, idx2;
  int fi = 0;
  if(occ > 1)
  {
    p->invalid = true;
  }
  else if(occ == 1)
  {
    idx1 = str_fstc(s, c);
    idx2 = idx1 + 1;
    while(s[idx2] == ' ' && idx2 < len) ++idx2;
    while(s[idx2] != ' ' && idx2 < len)
    {
      f[fi] = s[idx2];
      ++idx2;
      ++fi;
    }
    while(s[idx2] == ' ' && idx2 < len) ++idx2;
    if(idx2 < len && s[idx2] != s_p_rcpl(c))
    {
      p->invalid = true;
    }
    else
    {
      str_deln(s, idx1, idx2-idx1);
      p_set_args(p, str_trim(s));
      if(strlen(f) < 1)
        p->invalid = true;
      else
      {
        if(c == '<')
          str_set(&p->in_file, f);
        else
          str_set(&p->out_file, f);
      }
    }
  }
}

/**
 * Spracovanie parametrov.
 * @param p Process
**/
void p_parse_args(struct Process *p)
{
  char *s = str_dup(p->args);
  char buff[512];
  memset(buff, 0, 512);
  size_t len = strlen(s);
  for(size_t i=0; i<len; ++i)
  {
    if(s[i] == ' ')
    {
      if(strlen(buff) > 0)
      {
        v_append(&p->params, buff);
        memset(buff, 0, 512);
      }
      else
        continue;
    }
    else
    {
      buff[strlen(buff)] = s[i];
    }
  }

  if(strlen(buff) > 0)
    v_append(&p->params, buff);
  str_set(&p->args, "");

  if(v_size(&p->params) == 0)
    p->invalid = true;
}

/**
 * Zisti, ci je proces platne inicializovany.
 * @param p Process
 * @return true ak je proces dobre inicializovany
**/
bool p_is_valid(struct Process *p)
{
  return !p->invalid;
}

/**
 * Zisti, ci je proces na pozadi.
 * @param p Process
 * @return true ak je proces na pozadi
**/
bool p_is_background(struct Process *p)
{
  return p->background;
}

/**
 * Vrati subor pre citanie.
 * @param p Process
 * @return cestu k suboru
**/
const char *p_in_file(struct Process *p)
{
  return p->in_file == NULL ? NULL : str_dup(p->in_file);
}

/**
 * Vrati subor pre zapis.
 * @param p Process
 * @return cestu k suboru
**/
const char *p_out_file(struct Process *p)
{
  return p->out_file == NULL ? NULL : str_dup(p->out_file);
}

/**
 * Vrati parametre procesu.
 * @param p Process
 * @return parametre procesu
**/
char **p_get_args(struct Process *p)
{
  return v_to_a(&p->params);
}

/**
 * Vypis struktury.
 * @param p Process
**/
void p_print(struct Process *p)
{
  printf("Process:\n  In: '%s'\n  Out: '%s'\n  Args: '%s'\n  Bckg: %s\n  Inv: %s\n",
    p->in_file != NULL ? p->in_file : "N/A",
    p->out_file != NULL ? p->out_file : "N/A",
    p->args != NULL ? p->args : "N/A",
    p->background ? "true" : "false",
    p->invalid ? "true" : "false"
  );
  printf("Params:\n");
  v_print(&p->params);
}
