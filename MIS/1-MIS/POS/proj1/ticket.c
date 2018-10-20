/* ************************************************************************** *
 * Projekt: POS #1 - Ticket algoritmus
 * Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Datum:   2017-02-19
 * Subor:   ticket.c
 * ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>

#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1

int  strtoi(char *str);
long rnd();
void do_sleep();
void destroy_resources();

int  getticket();
void await(int aenter);
void advance();

void *process_thread(void *t);

struct thread_data
{
  unsigned int id;
  int ncpath;
};

int current_ticket = 0;
int next_ticket    = -1;

pthread_mutex_t process_mtx;
pthread_mutex_t ticket_mtx;
pthread_cond_t  served_cond;

int main(int argc, char **argv)
{
  if(argc != 3)
  {
    printf("./ticket nthreads ncpath\n");
    return EXIT_FAILURE;
  }

  int nthreads = strtoi(argv[1]);
  int ncpath   = strtoi(argv[2]);

  if(nthreads < 0 || ncpath < 0)
  {
    fprintf(stderr, "Neplatny typ alebo hodnota parametrov\n");
    return EXIT_FAILURE;
  }

  pthread_mutex_init(&process_mtx, NULL);
  pthread_mutex_init(&ticket_mtx, NULL);
  pthread_cond_init(&served_cond, NULL);

  pthread_t threads[nthreads];
  int res;
  struct thread_data td[nthreads];

  for(unsigned int i=0; i<(unsigned int)nthreads; ++i)
  {
    td[i].id = i+1;
    td[i].ncpath = ncpath;
    res = pthread_create(&threads[i], NULL, process_thread, (void *)&td[i]);
    if(res)
    {
      fprintf(stderr, "Nepodarilo sa vytvorit vlakno %d\n", i+1);
      destroy_resources();
      return EXIT_FAILURE;
    }
  }

  for(unsigned int i=0; i<(unsigned int)nthreads; ++i)
  {
    pthread_join(threads[i], NULL);
  }

  pthread_exit(NULL);
  destroy_resources();

  return EXIT_SUCCESS;
}

/**
 * @brief Prevedie retazec na kladne cele cislo.
 * @param str vstupny retazec
 * @return prevedene kladne cislo alebo -1 v pripade chyby
 */
int strtoi(char *str)
{
  char *ptr;
  long num = strtol(str, &ptr, 10);
  if(strcmp("", ptr) == 0 && num <= INT_MAX && num >= 0)
    return (int) num;
  else
    return -1;
}

/**
 * @brief PRNG cisel v rozsahu <0;500000000> (0-0.5 sekund).
 * @return nahodne cislo
 */
long rnd(unsigned int thread_id)
{
  double numd = (rand_r(&thread_id) % 1000) / 2000.0;
  return (long)(numd * 1000000000);
}

/**
 * @brief Vid. man nanosleep
 */
void do_sleep(long duration)
{
  nanosleep((const struct timespec[]){{0, duration}}, NULL);
}

/**
 * @brief Upratovanie.
 */
void destroy_resources()
{
  pthread_mutex_destroy(&process_mtx);
  pthread_mutex_destroy(&ticket_mtx);
  pthread_cond_destroy(&served_cond);
}

/**
 * @brief Vyda nasledujuci listok
 * @return listok
 */
int getticket()
{
  pthread_mutex_lock(&ticket_mtx);
  next_ticket += 1;
  pthread_mutex_unlock(&ticket_mtx);
  return next_ticket;
}

/**
 * @brief Vstup do kritickej sekcie, vlakno, ktoreho listok nie je na rade caka.
 * @param aenter cislo litku vlakna
 */
void await(int aenter)
{
  pthread_mutex_lock(&process_mtx);
  while(aenter > current_ticket)
  {
    pthread_cond_wait(&served_cond, &process_mtx);
  }
}

/**
 * @brief Vystup z kritickej sekcie, inkrementuje poradovnik listkov a informuje
 * cakajuce vlakna o zmene.
 */
void advance()
{
  pthread_mutex_lock(&ticket_mtx);
  current_ticket += 1;
  pthread_cond_broadcast(&served_cond);
  pthread_mutex_unlock(&process_mtx);
  pthread_mutex_unlock(&ticket_mtx);
}

/**
 * Praca vlakna, ak vstupi do kritickej sekcie vypise cislo listku, ktory mu
 * vstup umoznil a svoje id.
 * @param t data vlakna
 */
void *process_thread(void *t)
{
  struct thread_data *td = (struct thread_data *) t;
  int ticket;
  while((ticket = getticket()) < td->ncpath)
  {
    do_sleep(rnd(td->id));
    await(ticket);
    printf("%d\t(%d)\n", ticket, td->id);
    advance();
    do_sleep(rnd(td->id));
  }
  pthread_exit(NULL);
}
