
#include <stdlib.h>
#include "bag.h"
#include "curl.h"
#include "url.h"
#include "pagedir.h"


bag_t *bag_new(void) {
    bag_t *bag = malloc(sizeof(bag_t));
    if (bag == NULL) {
        return NULL;  
    }
    bag->head = NULL;
    return bag;
}



webpage_t *extractbag(bag_t *bag) {
    if (bag == NULL || bag->head == NULL) {
        return NULL;  
    }
    bagnode *node = bag->head;
    webpage_t *page = node->page;
    bag->head = node->next;
    free(node);
    return page;
}


int emptybag(const bag_t *bag) {
    if (bag == NULL) {
        return 1;  
    }
    return (bag->head == NULL);
}




void insertbag(bag_t *bag, webpage_t *page) {
    if (bag == NULL || page == NULL) {
        return;  
    }

    bagnode *new_node = malloc(sizeof(bagnode));
    if (new_node == NULL) {
        return;  
    }

    new_node->page = page;
    new_node->next = bag->head;
    bag->head = new_node;
}



webpage_t *bag_remove(bag_t *bag) {
    if (bag == NULL || bag->head == NULL) {
        return NULL;  
    }

    bagnode *node = bag->head;
    webpage_t *page = node->page;
    bag->head = node->next;

    free(node);
    return page;
}


void deletebag(bag_t *bag) {
    if (bag == NULL) {
        return;
    }

    bagnode *current = bag->head;
    while (current != NULL) {
        bagnode *next = current->next;
        free(current->page->url);  
        free(current->page->html); 
        free(current->page);
        free(current);
        current = next;
    }

    free(bag);
}