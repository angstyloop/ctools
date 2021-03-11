#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void removeDuplicateLines(const char*restrict fname) {
    FILE* fp = fopen(fname, "r");
    char* line = NULL;
    size_t n = 0;
    while (getline(&line, &n, fp)!=-1) {

    }
}
