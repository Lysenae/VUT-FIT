/*******************************************************************************
 * Súbor:   c402.c
 * Datum:   2014/11/29
 * Autor:   Daniel Klimaj, xklima22@stud.fit.vutbr.cz
 * Projekt: Nerekurzivne operacie nad binarnym stromom
 * Popis:   Implementácia neekurzivnych operacii na BS
 ******************************************************************************/

#include "c402.h"
int solved;

/**
 * Pomocná funkce, kterou budete volat pøi prùchodech stromem pro zpracování
 * uzlu urèeného ukazatelem Ptr. Tuto funkci neupravujte.
**/
void BTWorkOut(tBTNodePtr Ptr)
{
  if(Ptr == NULL)
  {
    printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
  }
  else
  {
    printf("Výpis hodnoty daného uzlu> %d\n",Ptr->Cont);
  }
}

/**
 * Inicializace zásobníku.
**/
void SInitP(tStackP *S)
{
  S->top = 0;
}

/**
 * Vlo¾í hodnotu na vrchol zásobníku.
**/
void SPushP(tStackP *S, tBTNodePtr ptr)
{
  if(S->top == MAXSTACK)
  {
    printf("Chyba: Do¹lo k pøeteèení zásobníku s ukazateli!\n");
  }
  else
  {
    S->top++;
    S->a[S->top] = ptr;
  }
}

/**
 * Odstraní prvek z vrcholu zásobníku a souèasnì vrátí jeho hodnotu.
**/
tBTNodePtr STopPopP(tStackP *S)
{
  if(S->top == 0)
  {
    printf("Chyba: Do¹lo k podteèení zásobníku s ukazateli!\n");
    return(NULL);
  } 
  else
  {
    return (S->a[S->top--]);
  }
}

/**
 * Je-li zásobník prázdný, vrátí hodnotu true.
**/
bool SEmptyP(tStackP *S)
{
  return(S->top == 0);
}

/**
 * Inicializace zásobníku.
**/
void SInitB(tStackB *S)
{
  S->top = 0;
}

/**
 * Vlo¾í hodnotu na vrchol zásobníku.
**/
void SPushB(tStackB *S,bool val)
{
  if(S->top == MAXSTACK)
  {
    printf("Chyba: Do¹lo k pøeteèení zásobníku pro boolean!\n");
  }
  else
  {
    S->top++;
    S->a[S->top] = val;
  }
}

/*   --------
** Odstraní prvek z vrcholu zásobníku a souèasnì vrátí jeho hodnotu.
**/
bool STopPopB(tStackB *S)
{
  if (S->top == 0)
  {
    printf("Chyba: Do¹lo k podteèení zásobníku pro boolean!\n");
    return(NULL);
  } 
  else
  {
    return(S->a[S->top--]);
  }
}

/**
 * Je-li zásobník prázdný, vrátí hodnotu true.
**/
bool SEmptyB(tStackB *S)
{
  return(S->top == 0);
}

/*******************************************************************************
** Následuje jádro domácí úlohy - funkce, které máte implementovat. 
*******************************************************************************/

/**
 * Inicializacia binarneho vyhladavacieho stromu.
 * @param *RootPtr - koren stromu
**/
void BTInit(tBTNodePtr *RootPtr)
{
  *RootPtr = NULL;
}

/**
** Vlozi do stromu novy uzol s hodnotou Content.
 * @param *RootPtr - koren stromu
 * @param Content - hodnota uzlu
**/
void BTInsert(tBTNodePtr *RootPtr, int Content)
{
  tBTNodePtr temp = (*RootPtr);
  tBTNodePtr node;

  if(*RootPtr == NULL)
  {
    tBTNodePtr it = malloc(sizeof(struct tBTNode));
    it->Cont = Content;
    it->LPtr = NULL;
    it->RPtr = NULL;
    (*RootPtr) = it;
  }
  else
  {
    while(temp != NULL)
    {
      node = temp;
      if(temp->Cont > Content)
      {
        temp = temp->LPtr;
      }
      else if(temp->Cont < Content)
      {
        temp = temp->RPtr;
      }
      else
      {
        node = NULL;
        break;
      }
    }

    if(node != NULL)
    {
      tBTNodePtr it = malloc(sizeof(struct tBTNode));
      it->Cont = Content;
      it->LPtr = NULL;
      it->RPtr = NULL;
      
      if(it->Cont < node->Cont)
      {
        node->LPtr = it;
      }
      else if(it->Cont > node->Cont)
      {
        node->RPtr = it;
      }
      else // Nemalo by nastat
      {
        free(it);
      }
    }
  }
}

/*                                  PREORDER                                  */

/**
 * Ide po lavej vetve podstromu, kym nenarazi na jeho najlavejsi uzol.
 * Pri prechode PreOrder navstivene uzly spracujeme volanim funkcie BTWorkOut()
 * a ukazovatele na ne si ulozime do zasobniku.
 * @param ptr - ukazovatel na uzol stromu
 * *Stack - zasobnik ukazovatelov
**/
void Leftmost_Preorder(tBTNodePtr ptr, tStackP *Stack)
{
  while(ptr != NULL)
  {
    SPushP(Stack, ptr);
    BTWorkOut(ptr);
    ptr = ptr->LPtr;
  }
}

/**
 * Prechod stromom typu PreOrder implementovany nerekurzivne s vyuzitim funkcie
 * Leftmost_Preorder a zasobniku ukazovatelov.
 * @param RootPtr - koren stromu
**/
void BTPreorder(tBTNodePtr RootPtr)
{
  if(RootPtr != NULL)
  {
    tBTNodePtr temp = NULL;
    tStackP s;

    SInitP(&s);
    Leftmost_Preorder(RootPtr, &s);

    while(!SEmptyP(&s))
    {
      temp = STopPopP(&s);
      Leftmost_Preorder(temp->RPtr, &s);
    }
  }
}

/*                                  INORDER                                   */ 

/**
 * Ide po lavej vetve podstromu, kym nenarazi na jeho najlavejsi uzol.
 * Pri prechode InOrder ukladame ukazovatele na vsetky navstivene uzly do
 * zasobniku.
 * @param ptr - ukazovatel na uzol stromu
 * @param *Stack - zasobnik ukazovatelov
**/
void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)
{
  while(ptr != NULL)
  {
    SPushP(Stack, ptr);
    ptr = ptr->LPtr;
  }
}

/**
 * Prechod stromom typu InOrder implementovany nerekurzivne s vyuzitim funkcie
 * Leftmost_Inorder a zasobniku ukazovatelov.
 * @param RootPtr - koren stromu
**/
void BTInorder(tBTNodePtr RootPtr)
{
  if(RootPtr != NULL)
  {
    tStackP s;
    tBTNodePtr temp = NULL;

    SInitP(&s);
    Leftmost_Inorder(RootPtr, &s);

    while(!SEmptyP(&s))
    {
      temp = STopPopP(&s);
      BTWorkOut(temp);
      Leftmost_Inorder(temp->RPtr, &s);
    }
  }
}

/*                                 POSTORDER                                  */

/**
 * Ide po lavej vetve podstromu, kym nenarazi na jeho nejlavejsi uzol.
 * Pri prechode PostOrder ukladame ukazovatele na navstivene uzly do zasobniku
 * a sucasne do zasobniku bool hodnot ukladame informaciu, ci bol uzol
 * navstiveny poprvykrat, t.j. ze sa este nema spracovavat.
 * @param ptr - ukazovatel na uzol stromu
 * @param *StackP - zasobnik ukazovatelov na uzly stromu
 * @param *StackB - zasobnik bool hodnot
**/
void Leftmost_Postorder(tBTNodePtr ptr, tStackP *StackP, tStackB *StackB)
{
  while(ptr != NULL)
  {
    SPushP(StackP, ptr);
    SPushB(StackB, TRUE);
    ptr = ptr->LPtr;
  }
}

/**
 * Prechod stromom typu PostOrder implementovany nerekurzivne s vyuzitim funkcie
 * Leftmost_Postorder, zasobniku ukazovatelov a zasobniku hotdnot typu bool.
 * @param RootPtr - koren stromu
**/
void BTPostorder(tBTNodePtr RootPtr)
{
  if(RootPtr != NULL)
  {
    tStackP sp;
    tStackB sb;
    int val;
    tBTNodePtr temp = NULL;

    SInitP(&sp);
    SInitB(&sb);
    Leftmost_Postorder(RootPtr, &sp, &sb);

    while(!SEmptyP(&sp))
    {
      temp = STopPopP(&sp);
      //SPushP(&sp, temp);
      val = STopPopB(&sb);

      if(val != FALSE)
      {
        SPushP(&sp, temp); // Potrebovali sme len Top
        SPushB(&sb, FALSE);
        Leftmost_Postorder(temp->RPtr, &sp, &sb);
      }
      else
      {
        //STopPopP(&sp);
        BTWorkOut(temp);
      }
    }
  }
}


/**
 * Zrusi vsetky uzly stromu a korektne uvolni nimi zabranou pamat.
 * @param *RootPtr - koren uzol
**/
void BTDisposeTree(tBTNodePtr *RootPtr)
{
  if((*RootPtr) != NULL)
  {
    tStackP s;
    tBTNodePtr temp;

    SInitP(&s);

    do
    {
      if((*RootPtr) == NULL)
      {
        if(!SEmptyP(&s))
        {
          (*RootPtr) = STopPopP(&s);
        }
      }
      else
      {
        if((*RootPtr)->RPtr != NULL)
        {
          SPushP(&s, (*RootPtr)->RPtr);
        }
        temp = (*RootPtr);
        (*RootPtr) = (*RootPtr)->LPtr;
        free(temp);
      }
    } while(!(((*RootPtr) == NULL) && (SEmptyP(&s))));
  }
}

/* konec c402.c */

