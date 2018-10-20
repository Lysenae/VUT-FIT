// htab_lookup_add.c
// Riesenie IJC-DU2, príklad 2), 25.4.2016
// Autor:     Daniel Klimaj, FIT
// Prelozene: gcc 5.3.0
// Pridanie alebo zvysenie poctu vyskytov prvku

#include "htable.h"

/**
 * Pridanie alebo zvysenie poctu vyskytov prvku.
 * @param ht Hashovacia tabulka
 * @param key Kluc
 * @return Pointer na pridany/najdeny prvok alebo NULL
 */
HtabListItem *htab_lookup_add(Htab *ht, const char *key)
{
    if(ht == NULL || key == NULL)
    {
        fprintf(stderr, "htab_lookup_add: parameters cannot be NULL\n");
        return NULL;
    }

    unsigned index = ht->hash_func(key, ht->htab_size);
    HtabListItem *item = ht->items[index];

    while(item != NULL)
    {
        if(strcmp(item->key, key) == 0)
        {
            item->data++;
            return item;
        }

        if(item->next !=NULL)
            item = item->next;
        else
            break;
    }

    HtabListItem *new_item = (HtabListItem*)malloc(sizeof(*new_item));
    if(new_item != NULL)
    {
        new_item->key = (char*)malloc(strlen(key+1));
        strcpy(new_item->key, key);
        new_item->key[strlen(key)] = '\0';
        new_item->data = 1;
        new_item->next = NULL;
    }
    else
    {
        return NULL;
    }

    if(item == NULL)
        ht->items[index] = new_item;
    else
        item->next = new_item;

    ht->n++;

    return new_item;
}
