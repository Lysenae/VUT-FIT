/*******************************************************************************
 * Súbor:   c203.c
 * Datum:   2014/10/11
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Fronta znakov v poli
 * Popis:   Implementácia fronty znakov v poli
 ******************************************************************************/

#include "c203.h"

/*
** Vypí¹e upozornenie na to, ¾e do¹lo k chybe.
** @param error_code Chybový kód
*/
void queueError (int error_code)
{
  static const char* QERR_STRINGS[MAX_QERR+1] = {
    "Unknown error",
    "Queue error: UP",
    "Queue error: FRONT",
    "Queue error: REMOVE",
    "Queue error: GET",
    "Queue error: INIT"
  };

  if ( error_code <= 0 || error_code > MAX_QERR )
    error_code = 0;

  printf ( "%s\n", QERR_STRINGS[error_code] );
  err_flag = 1;
}

/*
** Inicializujte frontu.
** @param q Ukazovateµ na frontu
*/
void queueInit (tQueue* q)
{
  // Test, ci nie je predavany NULL
  if(q == NULL)
  {
    queueError(QERR_INIT);
    return;
  }

  // Naplnenie fronty
  for(int i=0; i<QUEUE_SIZE; i++)
    q->arr[i] = '*';

  // Nastavenie indexov
  q->f_index = 0;
  q->b_index = 0;
}

/*
** Vracia index nasledujúceho prvku v poli.
** @param index Index prvku poµa, pre ktorý sa zis»uje index nasledujúceho prvku
*/
int nextIndex (int index)
{
  return (index+1) % QUEUE_SIZE;
}

/*
** Vracia nenulovú hodnotu, ak je frona prázdná, inak vracia hodnotu 0. 
** @param q Ukazovateµ na frontu
*/
int queueEmpty (const tQueue* q)
{
  return (q->f_index == q->b_index);
}

/*
** Vracia nenulovú hodnotu, ak je fronta plná, inak vracia hodnotu 0. 
** @param q Ukazovateµ na frontu
*/
int queueFull (const tQueue* q)
{
  return (nextIndex(q->b_index) == q->f_index);
}

/*
** Prostredníctvom parametru c vráti znak zo zaèiatku fronty q.
** Ak je fronta prázdna, tak vypí¹e chybové hlásenie.
** @param q Ukazovateµ na frontu
** @param c Ukazovateµ na char, do ktorého sa ulo¾í znak zo zaèiatku fronty
*/
void queueFront (const tQueue* q, char* c)
{
  if(queueEmpty(q))
  {
    queueError(QERR_FRONT);
    return;
  }
  else
  {
    *c = q->arr[q->f_index];
  }
}

/*
** Odstráni znak zo zaèiatku fronty q. Ak je fronta prázdna, tak vypí¹e
** chybové hlásenie.
** @param q Ukazovateµ na frontu
*/
void queueRemove (tQueue* q)
{
  if(queueEmpty(q))
  {
    queueError(QERR_REMOVE);
    return;
  }
  else
  {
    q->f_index = nextIndex(q->f_index);
  }
}

/*
** Odstráni znak zo zaèiatku fronty a vráti ho prostredníctvom parametru c.
** Ak je fronta prázdna, tak vypí¹e chybové hlásenie.
** @param q Ukazovateµ na frontu
** @param c Ukazovateµ na char, do ktorého sa ulo¾í znak zo zaèiatku fronty
*/
void queueGet (tQueue* q, char* c)
{
  if(queueEmpty(q))
  {
    queueError(QERR_GET);
    return;
  }
  else
  {
    queueFront(q, c);
    queueRemove(q);
  }
}

/*
** Vlo¾í znak c do fronty. Ak je fronta plná, tak vypí¹e chybové hlásenie.
** @param q Ukazovateµ na frontu
** @param c Znak, ktorý sa vlo¾í na koniec fronty
*/
void queueUp (tQueue* q, char c)
{
  if(queueFull(q))
  {
    queueError(QERR_UP);
    return;
  }
  else
  {
    q->arr[q->b_index] = c;
    q->b_index = nextIndex(q->b_index);
  }
}

/* Koniec príkladu c203.c */
