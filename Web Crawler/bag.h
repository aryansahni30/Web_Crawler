#ifndef BAG_H
#define BAG_H

#include "crawler.h"

typedef struct bag_node {
    webpage_t *page;
    struct bag_node *next;
} bagnode;

typedef struct {
    bagnode *head;
} bag_t;

bag_t *bag_new(void);
void insertbag(bag_t *bag, webpage_t *page);
webpage_t *extractbag(bag_t *bag);
int emptybag(const bag_t *bag);
void deletebag(bag_t *bag);
webpage_t *bag_remove(bag_t *bag);

#endif
