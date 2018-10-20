/* ************************************************************************** *
 * Projekt: POS #2 - Shell
 * Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Datum:   2017-04-09
 * Subor:   shell.c
 * ************************************************************************** */

#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "utils.h"
#include "monitor.h"
#include "process.h"

#define STDIN  0
#define STDOUT 1

#define RUN  100
#define READ 101

struct Monitor monitor;

void init_sigaction(struct sigaction *sa, int signum, void (*handler)(int));
void on_sigint(int signum);
void on_sigchld(int signum);
void *th_rt_read(void *t);
void *th_rt_run(void *t);
void run_process(struct Process *p);
int open_file(const char *path, int flags, int nfd);

int main()
{
  pthread_t th_read, th_run;
  struct sigaction sa_sigint, sa_sigchld;

  if(!mt_init(&monitor))
    return -1;

  init_sigaction(&sa_sigint, SIGINT, on_sigint);
  init_sigaction(&sa_sigchld, SIGCHLD, on_sigchld);

  pthread_create(&th_read, NULL, th_rt_read, (void *) &monitor);
  pthread_create(&th_run, NULL, th_rt_run, (void *) &monitor);

  pthread_join(th_read, NULL);
  pthread_join(th_run, NULL);

  mt_destroy(&monitor);

  return 0;
}

/**
 * Nastavenie handleru pre signal.
 * @param sa sigaction
 * @param handler riadiaca funkcia
**/
void init_sigaction(struct sigaction *sa, int signum, void (*handler)(int))
{
  sa->sa_handler = handler;
  sa->sa_flags = 0;
  sigemptyset(&sa->sa_mask);
  sigaction(signum, sa, NULL);
}

/**
 * Handler SIGINT
 * @param signum cislo signalu
**/
void on_sigint(int signum)
{
  mt_kill_running_pid(&monitor, signum);
}

/**
 * Handler SIGCHLD.
 * @param signum nevyuziva sa
**/
void on_sigchld(int __attribute__((unused)) signum)
{
  int wstatus   = 0;
  pid_t pid     = waitpid(-1, &wstatus, 0);
  pid_t running = mt_get_running_pid(&monitor);

  if(pid != running && pid > 0)
  {
    if(WIFEXITED(wstatus))
      printf("[%d] skoncil s kodom %d\n", pid, WEXITSTATUS(wstatus));
    else if(WIFSIGNALED(wstatus))
      printf("[%d] bol ukonceny signalom %d\n", pid, WTERMSIG(wstatus));
    else if(WIFSTOPPED(wstatus))
      printf("[%d] bol zastaveny signalom %d\n", pid, WSTOPSIG(wstatus));
    else
      printf("[%d] bol ukonceny\n", pid);
  }
}

/**
 * Vlakno pre citanie vstupov.
 * @param t data vlakna
**/
void *th_rt_read(void *t)
{
  struct Monitor *m = (struct Monitor *) t;
  int rc;
  char buff[MT_CMDLEN+1]; // +1 aby sa zaistilo spravne ukoncenie retazca

  while(mt_running(m))
  {
    printf("$ ");
    fflush(stdout);
    memset(buff, 0, MT_CMDLEN+1);
    rc = read(STDIN, buff, MT_CMDLEN);
    if(rc == MT_CMDLEN && buff[rc-1] != '\n') // Maximum je 512 znakov + Enter
    {
      fprintf(stderr, "Chyba: Prikaz prilis dlhy\n");
      while(getchar() != '\n') {} // precitaj prikaz az do konca
      buff[0] = '\0';
    }
    buff[rc-1] = '\0';

    if(strcmp(buff, "exit") == 0)
    {
      mt_shutdown(m);
      mt_signal(m, READ);
      break;
    }
    else if(strcmp(buff, "") == 0)
    {
      continue;
    }
    else
    {
      mt_set_cmd(m, str_trim(buff));
      mt_signal(m, READ);
      mt_wait(m, READ);
    }
  }
  pthread_exit(NULL);
}

/**
 * Vlakno pre spustanie procesov.
 * @param t data vlakna
**/
void *th_rt_run(void *t)
{
  struct Monitor *m = (struct Monitor *) t;
  struct Process p;

  while(mt_running(m))
  {
    mt_wait(m, RUN);

    if(!mt_running(m))
      break;

    p_init(&p, mt_get_cmd(m));
    if(p_is_valid(&p))
    {
      pid_t pid = fork();

      if(pid > 0) // Parent
      {
        if(!p_is_background(&p))
        {
          mt_set_running_pid(m, pid);
          waitpid(pid, NULL, 0);
          mt_set_running_pid(m, 0);
        }
      }
      else if(pid == 0) // Child
      {
        run_process(&p);
      }
      else
      {
        fprintf(stderr, "Chyba: Zlyhalo volanie fork()\n");
        mt_shutdown(m);
        break;
      }
    }
    else
    {
      fprintf(stderr, "Chyba: Chybne paramete prikazu %s\n", mt_get_cmd(m));
    }
    p_destroy(&p);
    mt_signal(m, RUN);
  }
  pthread_exit(NULL);
}

/**
 * Spustenie procesu.
 * @param p Process
**/
void run_process(struct Process *p)
{
  int ri, ro;
  ri = open_file(p_in_file(p), O_RDONLY, STDIN);
  ro = open_file(p_out_file(p), O_WRONLY | O_CREAT | O_TRUNC, STDOUT);
  if(ri < 0 || ro < 0)
  {
    exit(1);
  }

  char **args = p_get_args(p);
  if(execvp(*args, args) < 0)
  {
    fprintf(stderr, "Chyba: Nepodarilo sa spustit proces\n");
    exit(1);
  }

  if(ri > 0) close(ri);
  if(ro > 0) close(ro);
}

/**
 * Otvorenie suboru.
 * @param path cesta k suboru
 * @param flags priznaky otvorenia suboru
 * @param nfd deskriptor pre stdin alebo stdout
 * @return 0 ak subor nie je zadany, -1 ak sa otvorenie nezdarilo,
 * inak deskriptor.
**/
int open_file(const char *path, int flags, int nfd)
{
  int fd;
  if(path != NULL && strlen(path) > 0)
  {
    if(nfd == STDOUT)
      fd = open(path, flags, 0666);
    else
      fd = open(path, flags);

    if(fd < 0)
    {
      fprintf(stderr, "Chyba: Nepodarilo sa otvorit subor '%s'\n", path);
      return -1;
    }

    if(dup2(fd, nfd) < 0)
    {
      fprintf(stderr, "Chyba: Zlyhalo duplikovanie deskriptoru\n");
      return -1;
    }

    return fd;
  }
  return 0;
}
