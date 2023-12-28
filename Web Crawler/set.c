#include "set.h"

typedef struct Node
{
    char *key;
    void *item;
    struct Node *next;
} Node;

struct set
{
    Node *head;
};

set_t *set_new(void)
{
    set_t *set = malloc(sizeof(set_t));
    if (set == NULL)
        return NULL;
    set->head = NULL;
    return set;
}

bool set_insert(set_t *set, const char *key, void *item)
{
    if (!set || !key || !item)
        return false;

    Node *prev = NULL, *cur = set->head;
    while (cur)
    {
        if (!strcmp(cur->key, key))
            return false;
        prev = cur;
        cur = cur->next;
    }

    Node *node = malloc(sizeof(Node));
    if (node == NULL)
    {
        return false;
    }
    node->key = strdup(key);
    node->item = item;
    node->next = NULL;

    if (prev)
        prev->next = node;
    else
        set->head = node;

    return true;
}

void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item))
{
    if (!fp)
        return;
    Node *cur = set->head;
    while (cur)
    {
        itemprint(fp, cur->key, cur->item);
        cur = cur->next;
    }
}

void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item))
{
    if (!set || !itemfunc)
        return;
    Node *cur = set->head;
    while (cur)
    {
        itemfunc(arg, cur->key, cur->item);
        cur = cur->next;
    }
}

void *set_find(set_t *set, const char *key)
{
    if (!set || !key)
        return NULL;
    Node *cur = set->head;
    while (cur && strcmp(cur->key, key))
        cur = cur->next;
    return cur ? cur->item : NULL;
}

void set_delete(set_t *set, void (*itemdelete)(void *item))
{
    if (!set)
        return;
    Node *cur = set->head, *tmp;
    while (cur)
    {
        tmp = cur;
        cur = cur->next;
        free(tmp->key);
        if (itemdelete)
            itemdelete(tmp->item);
        free(tmp);
    }
    free(set);
}
