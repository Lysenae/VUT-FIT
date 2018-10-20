/*******************************************************************************
 * Súbor:   c401.c
 * Datum:   2014/11/29
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Rekurzivne operacie nad binarnym stromom
 * Popis:   Implementácia rekurzivnych operacii na BS
 ******************************************************************************/

#include "c401.h"
int solved;

/**
 * Inicializacia BS.
 * @param *RootPtr - korenovy uzol stromu
**/
void BSTInit(tBSTNodePtr *RootPtr)
{
  *RootPtr = NULL;
}

/**
 * Vyhladanie uzlu stromu podla kluca a ulozenie hodnoty.
 * @param RootPtr - uzol stromu
 * @param K - kluc
 * @param *Content - ulozisko pre hodnotu uzla
 * @returns TRUE ak bolo vyhladavanie uspesne, inak FALSE
**/
int BSTSearch(tBSTNodePtr RootPtr, char K, int *Content)
{
  if(RootPtr != NULL)
  {
    if(RootPtr->Key == K)
    {
      *Content = RootPtr->BSTNodeCont;
      return TRUE;
    }
    else if(RootPtr->Key < K)
    {
      return BSTSearch(RootPtr->RPtr, K, Content);
    }
    else if(RootPtr->Key > K)
    {
      return BSTSearch(RootPtr->LPtr, K, Content);
    }
  }

  return FALSE;
}

/**
 * Vlozenie prvku do BS podla kluca.
 * @param *RootPtr - uzol stromu
 * @param K - kluc
 * @param Content - hodnota uzla
**/
void BSTInsert(tBSTNodePtr* RootPtr, char K, int Content)
{
  if(*RootPtr == NULL)
  {
    *RootPtr = malloc(sizeof(struct tBSTNode));
    if(*RootPtr == NULL)
    {
      return;
    }
    else
    {
      (*RootPtr)->Key = K;
      (*RootPtr)->RPtr = NULL;
      (*RootPtr)->LPtr = NULL;
      (*RootPtr)->BSTNodeCont = Content;
    }
  }
  else if((*RootPtr)->Key == K)
  {
    (*RootPtr)->BSTNodeCont = Content;
  }
  else if((*RootPtr)->Key < K)
  {
    BSTInsert((&(*RootPtr)->RPtr), K, Content);
  }
  else if((*RootPtr)->Key > K)
  {
    BSTInsert((&(*RootPtr)->LPtr), K, Content);
  }
}

/**
 * Nahradenie data uzla datami najpravejsieho uzlu ineho uzlu.
 * @param PtrReplaced - uzol, ktoreho data budu nahradene
 * @param *RootPtr - uzol, ktoremu sa hlada najpravejsi uzol
**/
void ReplaceByRightmost(tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr)
{
  if(*RootPtr != NULL)
  {
    if((*RootPtr)->RPtr != NULL)
    {
      ReplaceByRightmost(PtrReplaced, (&(*RootPtr)->RPtr));
    }
    else
    {
      PtrReplaced->Key = (*RootPtr)->Key;
      PtrReplaced->BSTNodeCont = (*RootPtr)->BSTNodeCont;

      tBSTNodePtr temp = *RootPtr;
      *RootPtr = (*RootPtr)->LPtr;
      free(temp);
    }
  }
}

/**
 * Zrusenie uzlu.
 * @param PtrReplaced - uzol, ktoreho data budu nahradene
 * @param *RootPtr - uzol, ktoremu sa hlada najpravejsi uzol
**/
void BSTDelete(tBSTNodePtr *RootPtr, char K)
{
  if(*RootPtr != NULL)
  {
    if((*RootPtr)->Key == K)
    {
      tBSTNodePtr temp = *RootPtr;

      if(((*RootPtr)->LPtr != NULL) && ((*RootPtr)->RPtr == NULL))
      { // Ma len lavy podstrom
        temp = (*RootPtr);
        *RootPtr = (*RootPtr)->LPtr;
        free(temp);
        temp = NULL;
      }
      else if(((*RootPtr)->RPtr != NULL) && ((*RootPtr)->LPtr == NULL))
      { // Ma len pravy podstrom
        temp = (*RootPtr);
        *RootPtr = (*RootPtr)->RPtr;
        free(temp);
        temp = NULL;
      }
      else if(((*RootPtr)->RPtr != NULL) && ((*RootPtr)->LPtr != NULL))
      { // Ma oba podstromy
        ReplaceByRightmost(*RootPtr, (&(*RootPtr)->LPtr));
      }
      else if(((*RootPtr)->RPtr == NULL) && ((*RootPtr)->LPtr == NULL))
      { // Listovy uzol
        free(*RootPtr);
        *RootPtr = NULL;
      }
    }
    else if((*RootPtr)->Key < K)
    {
      BSTDelete((&(*RootPtr)->RPtr), K);
    }
    else
    {
      BSTDelete((&(*RootPtr)->LPtr), K);
    }
  }
}

/**
 * Zrusenie stromu a nastavenie do stavu po inicializacii.
 * @param *RootPtr - koren stromu
**/void BSTDispose(tBSTNodePtr *RootPtr)
{
  if(*RootPtr != NULL)
  {
    BSTDispose((&(*RootPtr)->LPtr));
    BSTDispose((&(*RootPtr)->RPtr));
    free(*RootPtr);
    *RootPtr = NULL;
  }
}

/* konec c401.c */
