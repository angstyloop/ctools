#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* extractFileName(char* source) {
    char* regex = "^[^/]+/[^ ]+";
    const unsigned numGroups = 1;
    regmatch_t groups[numGroups];
    regex_t regexCompiled;
    if (regcomp(&regexCompiled, regex, REG_EXTENDED)) return NULL;
    const unsigned execFlags = 0; 
    if (regexec(&regexCompiled, source, numGroups, groups, execFlags)) return NULL;
    if (groups[0].rm_so != -1 && groups[0].rm_eo != -1) {
        const unsigned startIndex = groups[0].rm_so;
        const unsigned endIndex = groups[0].rm_eo-1;
        const unsigned numNonNullChars = endIndex - startIndex;
        char* matchingSubstring =  calloc(numNonNullChars+1, sizeof(char)); //guaranteed terminating '\0'
        memcpy(matchingSubstring, source + startIndex, numNonNullChars + sizeof(char));
        return matchingSubstring;
    } else return NULL;
}

#ifdef TEST_EXTRACT_FILE_NAME_CMDLINE
int main(int argc, char* argv[]) {
    if (argc!=2) return -1;
    char* fileName = extractFileName(argv[1]);
    if (fileName) {
        printf("%s\n", fileName);
        free(fileName);
    }
    return 0;
}
#endif

void extractFileNames(char *fname) {
    char* line = NULL;
    size_t n = 0;
    FILE* fp = fopen(fname, "r");

    while (getline(&line, &n, fp)!=-1) {
        char* fileName = extractFileName(line);
        if (fileName) {
            printf("%s\n", fileName);
            free(fileName);
        }
    }
    fclose(fp);
}

#ifdef TEST_EXTRACT_FILE_NAMES_CMDLINE
int main(int argc, char* argv[]) {
    if (argc!=2) return -1;
    extractFileNames(argv[1]);
    return 0;
}
#endif
