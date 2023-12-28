#include "pagedir.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>


bool pagedir_init(const char *pageDirectory) {
    struct stat st = {0};
    if (stat(pageDirectory, &st) == -1) {
        
        if (mkdir(pageDirectory, 0700) != 0) {
            perror("Error creating directory");
            return false;
        }
    }

    return true;
}


void savepagedir(const webpage_t *page, const char *pageDirectory, const int document_id) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/%d", pageDirectory, document_id);
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    
    fprintf(file, "%s\n%d\n%s", page->url, page->depth, page->html);

    fclose(file);
}
