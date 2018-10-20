/*******************************************************************************
 * Súbor:   c016.c
 * Datum:   2014/11/29
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Hashovacia tabulka
 * Popis:   Implementácia hashovacej tabulky s rozptylenymi hodnotami
 ******************************************************************************/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/**
 * Rozptylovacia funkcia.
 * @param key - retazec, z ktoreho sa vypocita hash
 * @returns hash, v tomto pripade index v hashovacej tabulke
**/
int hashCode(tKey key)
{
  int retval = 1;
  int keylen = strlen(key);
  for ( int i=0; i<keylen; i++ )
    retval += key[i];
  return ( retval % HTSIZE );
}

/**
 * Inicializacia tabulky s explicitne zretazenymi synonymami. Tato procedura sa
 * vola pred prvym vyuzitim tabulky.
 * @param *ptrht - ukazovatel na hashovaciu tabulku
**/
void htInit(tHTable* ptrht)
{
  if(ptrht != NULL)
  {
    memset(*ptrht, 0, sizeof(struct tHTItem*)*HTSIZE);
  }
}

/**
 * Vyhladanie prvku tabulky podla kluca.
 * @param *ptrht - ukazovatel na hashovaciu tabulku
 * @returns - prvok tabulky odpovedajuci klucu alebo NULL ak prvok nebol najdeny
**/
tHTItem* htSearch(tHTable* ptrht, tKey key)
{
  if(ptrht != NULL)
  {
    int idx = hashCode(key);
    tHTItem *it = (*ptrht)[idx];

    if(it != NULL)
    {
      while(it != NULL)
      {
        if(!strcmp(it->key, key))
        {
          return it;
        }
        it = it->ptrnext;
      }
    }
  }

  return NULL;
}

/**
 * Vlozenie prvku do tabulky podla kluca a inicializacia dat prvku.
 * @param *ptrht - ukazovatel na hashovaciu tabulku
 * @param key - retazec kluca
 * @param data - data prvku
**/
void htInsert(tHTable* ptrht, tKey key, tData data)
{
  if(ptrht != NULL)
  {
    int idx = hashCode(key);
    tHTItem *it = htSearch(ptrht, key);

    if(it == NULL)
    {
      tHTItem *itNew = malloc(sizeof(struct tHTItem));

      if(itNew != NULL)
      {
        itNew->data = data;
        itNew->key = malloc(sizeof(char) * (strlen(key)+1));
        strcpy(itNew->key, key);
        itNew->ptrnext = (*ptrht)[idx];

        (*ptrht)[idx] = itNew;
      }
    }
    else
    {
      it->data = data;
    }
  }
}

/**
 * Zistenie hodnoty dat prvku tabulky.
 * @param *ptrht - ukazovatel na hashovaciu tabulku
 * @param key - retazec kluca
 * @returns ukazovatel na hodnotu dat prvku alebo NULL ak prvok neexistuje
**/
tData* htRead(tHTable* ptrht, tKey key)
{
  if(ptrht != NULL)
  {
    tHTItem *it = htSearch(ptrht, key);

    if(it != NULL)
    {
      return &(it->data);
    }
  }

  return NULL;
}

/**
 * Zrusenie prvku tabulky podla kluca.
 * @param *ptrht - ukazovatel na hashovaciu tabulku
 * @param key - retazec kluca
**/
void htDelete(tHTable* ptrht, tKey key)
{
  if(ptrht != NULL)
  {
    int idx = hashCode(key);

    // Vyhladanie prvku
    tHTItem *it = (*ptrht)[idx];
    if(it != NULL)
    {
      while(it != NULL)
      {
        if(!strcmp(it->key, key))
        {
          break;
        }
        else
        {
          it = it->ptrnext;
        }
      }

      if(it != NULL)
      {
        tHTItem *itPrev = (*ptrht)[idx];
        if(itPrev != it)
        { // Ruseny prvok nie je zaroven prvy prvok
          while(itPrev != NULL)
          {
            if(itPrev->ptrnext == it)
            {
              break;
            }
            itPrev = itPrev->ptrnext;
          }
          itPrev->ptrnext = it->ptrnext;
        }
        else
        { // Ruseny prvok je zaroven aj prvy prvok
          (*ptrht)[idx] = it->ptrnext;
        }
        
        free(it->key);
        free(it);
        it = NULL;
      }
    }
  }
}

/**
 * Zrusenie vsetkych prvkov tabulky a nastavenie tabulky do stavu po
 * inicializacii.
 * @param *ptrht - ukazovatel na hashovaciu tabulku
**/
void htClearAll(tHTable* ptrht)
{
  tHTItem *temp = NULL;
  for(int i=0; i<HTSIZE; i++)
  {
    if((*ptrht)[i] != NULL)
    {
      while((*ptrht)[i] != NULL)
      {
        temp = (*ptrht)[i];
        (*ptrht)[i] = temp->ptrnext;
        free(temp->key);
        temp->ptrnext = NULL;
        free(temp);
        temp = NULL;
      }
    }
  }

  // Nastavenie do stavu po inicializacii bez volania htInit
  memset(*ptrht, 0, sizeof(struct tHTItem*)*HTSIZE);
}

/* konec c016.c */
