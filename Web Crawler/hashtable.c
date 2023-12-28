#include "hashtable.h"

unsigned int hash(const char *key)
{
  unsigned int hash_val = 0;
  while (*key != '\0')
  {
    hash_val = (hash_val << 5) + *key++;
  }
  return hash_val;
}

hashtable_t *hashtable_new(const int num_slots)
{
  hashtable_t *ht = malloc(sizeof(hashtable_t));
  if (ht == NULL)
    return NULL;
  ht->num_slots = num_slots;
  ht->slots = malloc(num_slots * sizeof(set_t *));
  if (ht->slots == NULL)
    return NULL;
  for (int i = 0; i < num_slots; i++)
    ht->slots[i] = set_new();
  return ht;
}

bool hashtable_insert(hashtable_t *ht, const char *key, void *item)
{

  if (ht == NULL || key == NULL || item == NULL)
    return false;

  int index = hash(key) % ht->num_slots;
  return set_insert(ht->slots[index], key, item);
}

void *hashtable_find(hashtable_t *ht, const char *key)
{
  if (ht == NULL || key == NULL)
    return NULL;

  int index = hash(key) % ht->num_slots;
  return set_find(ht->slots[index], key);
}

void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item))
{
  if (fp == NULL)
    return;

  if (ht == NULL)
  {
    fputs("(null)\n", fp);
  }
  else
  {
    for (int i = 0; i < ht->num_slots; ++i)
    {
      set_print(ht->slots[i], fp, itemprint);
    }
  }
}

void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item))
{
  if (ht == NULL || itemfunc == NULL)
    return;

  for (int i = 0; i < ht->num_slots; ++i)
  {
    set_iterate(ht->slots[i], arg, itemfunc);
  }
}

void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item))

{
  if (ht == NULL)
    return;

  for (int i = 0; i < ht->num_slots; ++i)
  {
    set_delete(ht->slots[i], itemdelete);
  }
  free(ht->slots);
  free(ht);
}
