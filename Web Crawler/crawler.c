#include <stdio.h>
#include <stdlib.h>
#include "crawler.h"
#include "curl.h"
#include "pagedir.h"
#include "hashtable.h"
#include "bag.h"
#include "url.h"



webpage_t *webpagecreate(const char *url, int depth)
{
    webpage_t *page = malloc(sizeof(webpage_t));
    if (page == NULL)
    {
        return NULL;
    }
    page->url = strdup(url);
    if (page->url == NULL)
    {
        free(page);
        return NULL;
    }
    page->depth = depth;
    page->html = NULL;
    page->length = 0;
    return page;
}

void webpage_free(webpage_t *page)
{
    if (page != NULL)
    {
        free(page->url);
        free(page->html);
        free(page);
    }
}

char *getnext(const char *htmlinfo, int *position)
{
    const char *current = htmlinfo + *position;
    while (*current != '\0')
    {
        if (*current == '<' && *(current + 1) == 'a')
        {
            current += 2;
            while (*current != '\0' && strncmp(current, "href", 4) != 0)
            {
                current++;
            }
            if (strncmp(current, "href", 4) == 0)
            {
                current += 4;
                while (*current != '\0' && (*current == ' ' || *current == '='))
                {
                    current++;
                }
                if (*current == '\"' || *current == '\'')
                {
                    char quotetp = *current;
                    current++;
                    const char *start = current;
                    while (*current != '\0' && *current != quotetp)
                    {
                        current++;
                    }
                    if (*start != '#')
                    {
                        int length_url = current - start;
                        char *url = malloc(length_url + 1);
                        if (url != NULL)
                        {
                            strncpy(url, start, length_url);
                            url[length_url] = '\0';
                            *position = current - htmlinfo + 1;
                            return url;
}
}
}
}
}
        current++;
}
    return NULL;
}

static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth);
static void crawl(char *seedURL, char *pageDirectory, const int maxDepth);
static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen);

static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth)
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s seedURL pageDirectory maxDepth\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    *seedURL = argv[1];
    *pageDirectory = argv[2];
    if (!pagedir_init(*pageDirectory))
    {
        fprintf(stderr, "Page directory could not be initialized.\n");
        exit(EXIT_FAILURE);
    }
    *maxDepth = atoi(argv[3]);
    if (*maxDepth < 0 || *maxDepth > 10)
    {
        fprintf(stderr, "Max depth must be between 0 and 10.\n");
        exit(EXIT_FAILURE);
    }
}

static void crawl(char *seedURL, char *pageDirectory, const int maxDepth)
{
    bag_t *pagesToCrawl = bag_new();
    hashtable_t *pagesSeen = hashtable_new(1000);
    int doc_num = 1;

    webpage_t *initialPage = webpagecreate(seedURL, 0);
    insertbag(pagesToCrawl, initialPage);
    hashtable_insert(pagesSeen, seedURL, initialPage);

    while (!emptybag(pagesToCrawl))
    {
        webpage_t *page = extractbag(pagesToCrawl);
        size_t html_size;
        char *html_content = download(page->url, &html_size);

        if (html_content != NULL && hashtable_find(pagesSeen, page->url))
        {
            page->html = html_content;
            page->length = html_size;
            savepagedir(page, pageDirectory, doc_num++);
            if (page->depth < maxDepth)
            {
                pageScan(page, pagesToCrawl, pagesSeen);
            }
            webpage_free(page);
        }
        else
        {
            fprintf(stderr, "Failed to download page: %s\n", page->url);
            webpage_free(page);
            continue;
        }
    }

    deletebag (pagesToCrawl);
    hashtable_delete(pagesSeen, free);
}

static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen)
{
    char *nextURL;
    int position = 0;

    while ((nextURL = getnext(page->html, &position)) != NULL)
    {
        char *url_normal = normalizeURL(page->url, nextURL);
        if (url_normal != NULL && isInternalURL(page->url, url_normal))
        {
            if (!hashtable_find(pagesSeen, url_normal))
            {
                webpage_t *page_new = webpagecreate(url_normal, page->depth + 1);
                if (page_new != NULL)
                {
                    insertbag(pagesToCrawl, page_new);
                    hashtable_insert(pagesSeen, url_normal, page_new);
                }
            }
        }

        free(nextURL);
        free(url_normal);
    }
}

int main(const int argc, char *argv[])
{
    char *seedURL, *pageDirectory;
    int maxDepth;

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);

    return EXIT_SUCCESS;
}
