/* ************************************************************************** *
 * Projekt: POS #2 - Shell
 * Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz
 * Datum:   2017-04-09
 * Subor:   monitor.h
 * ************************************************************************** */

#ifndef MONITOR_H
#define MONITOR_H

#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define MT_CMDLEN 513

struct Monitor
{
  bool            running;
  char            command[MT_CMDLEN];
  int             cond_id;
  pid_t           running_pid;
  pthread_mutex_t mtx_th;
  pthread_mutex_t mtx_data;
  pthread_mutex_t mtx_proc;
  pthread_cond_t  cond;
};

bool mt_init(struct Monitor *m);
void mt_destroy(struct Monitor *m);
void mt_shutdown(struct Monitor *m);
bool mt_running(struct Monitor *m);
void mt_wait(struct Monitor *m, int id);
void mt_signal(struct Monitor *m, int id);
void mt_set_cmd(struct Monitor *m, const char *cmd);
char *mt_get_cmd(struct Monitor *m);
pid_t mt_get_running_pid(struct Monitor *m);
void mt_set_running_pid(struct Monitor *m, pid_t p);
void mt_kill_running_pid(struct Monitor *m, int signum);

#endif // MONITOR_H
