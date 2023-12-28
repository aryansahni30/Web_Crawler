#ifndef CRAWLER_H
#define CRAWLER_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char *url;
    char *html;
    size_t length;
    int depth;
} webpage_t;


void webpage_free(webpage_t *page);
webpage_t *webpagecreate(const char *url, int depth);
bool pagefetch(webpage_t *page);
char *getnext(const char *htmlContent, int *position);


#endif
