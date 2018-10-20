/* ************************************************************************** *
 * Projekt: POS #2 - Shell
 * Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Datum:   2017-04-09
 * Subor:   monitor.c
 * ************************************************************************** */

#include "monitor.h"

/**
 * Nastavenie id vlakna, ktore blokuje cond.
 * @param m Monitor
 * @param id vlakna
**/
static void s_mt_set_cond_id(struct Monitor *m, int id)
{
  pthread_mutex_lock(&m->mtx_data);
  m->cond_id = id;
  pthread_mutex_unlock(&m->mtx_data);
}

/**
 * Ziskanie id vlakna, ktore blokuje cond.
 * @param m Monitor
 * @return id vlakna
**/
static int s_mt_get_cond_id(struct Monitor *m)
{
  int id;
  pthread_mutex_lock(&m->mtx_data);
  id = m->cond_id;
  pthread_mutex_unlock(&m->mtx_data);
  return id;
}

/**
 * Nastavenie monitoru.
 * @param m Monitor
 * @return true ak sa podarilo
**/
bool mt_init(struct Monitor *m)
{
  int rc         = 0;
  m->running     = true;
  m->cond_id     = 0;
  m->running_pid = 0;
  memset(&m->command, 0, MT_CMDLEN);
  rc += pthread_mutex_init(&m->mtx_th, NULL);
  rc += pthread_mutex_init(&m->mtx_data, NULL);
  rc += pthread_mutex_init(&m->mtx_proc, NULL);
  rc += pthread_cond_init(&m->cond, NULL);
  if(rc != 0)
  {
    fprintf(stderr, "Chyba: Nepodarilo sa vytvorit monitor\n");
    return false;
  }
  return true;
}

/**
 * Zrusenie monitoru.
 * @param m Monitor
**/
void mt_destroy(struct Monitor *m)
{
  pthread_mutex_destroy(&m->mtx_th);
  pthread_mutex_destroy(&m->mtx_data);
  pthread_mutex_destroy(&m->mtx_proc);
  pthread_cond_destroy(&m->cond);
  m->running = false;
  m->cond_id     = 0;
  m->running_pid = 0;
}

/**
 * Vypnutie riadiaceho cyklu.
 * @param m Monitor
**/
void mt_shutdown(struct Monitor *m)
{
  pthread_mutex_lock(&m->mtx_data);
  m->running = false;
  pthread_mutex_unlock(&m->mtx_data);
}

/**
 * Zisti, ci riadiaci cyklus ma bezat.
 * @param m Monitor
 * @return true ak ma bezat
**/
bool mt_running(struct Monitor *m)
{
  bool rslt;
  pthread_mutex_lock(&m->mtx_data);
  rslt = m->running;
  pthread_mutex_unlock(&m->mtx_data);
  return rslt;
}

/**
 * Vlakno zacina cakat na cond.
 * @param m Monitor
 * @param id vlakna
**/
void mt_wait(struct Monitor *m, int id)
{
  s_mt_set_cond_id(m, id);
  pthread_mutex_lock(&m->mtx_th);
  while(id == s_mt_get_cond_id(m))
    pthread_cond_wait(&m->cond, &m->mtx_th);
  pthread_mutex_unlock(&m->mtx_th);
}

/**
 * Vlakno signalizuje splnenie cond.
 * @param m Monitor
 * @return id vlakna
**/
void mt_signal(struct Monitor *m, int id)
{
  s_mt_set_cond_id(m, id);
  pthread_mutex_lock(&m->mtx_th);
  pthread_cond_signal(&m->cond);
  pthread_mutex_unlock(&m->mtx_th);
}

/**
 * Nastavenie prikazu.
 * @param m Monitor
 * @param cmd retazec
**/
void mt_set_cmd(struct Monitor *m, const char *cmd)
{
  pthread_mutex_lock(&m->mtx_data);
  strcpy(m->command, cmd);
  pthread_mutex_unlock(&m->mtx_data);
}

/**
 * Ziskanie prikazu.
 * @param m Monitor
 * @return prikaz
**/
char *mt_get_cmd(struct Monitor *m)
{
  char *buff = malloc(sizeof(m->command));
  pthread_mutex_lock(&m->mtx_data);
  strcpy(buff, m->command);
  pthread_mutex_unlock(&m->mtx_data);
  return buff;
}

/**
 * Ziskanie pid beziaceho procesu na popredi.
 * @param m Monitor
 * @return pid procesu
**/
pid_t mt_get_running_pid(struct Monitor *m)
{
  pid_t p;
  pthread_mutex_lock(&m->mtx_data);
  p = m->running_pid;
  pthread_mutex_unlock(&m->mtx_data);
  return p;
}

/**
 * Nastavenie pid procesu beziaceho na popredi.
 * @param m Monitor
 * @param p pid
**/
void mt_set_running_pid(struct Monitor *m, pid_t p)
{
  pthread_mutex_lock(&m->mtx_data);
  m->running_pid = p;
  pthread_mutex_unlock(&m->mtx_data);
}

/**
 * Zabitie procesu beziaceho na popredi.
 * @param m Monitor
 * @param signum cislo signalu
**/
void mt_kill_running_pid(struct Monitor *m, int signum)
{
  pthread_mutex_lock(&m->mtx_proc);
  pid_t p = mt_get_running_pid(m);
  if(p > 0)
    kill(p, signum);
  pthread_mutex_unlock(&m->mtx_proc);
}
