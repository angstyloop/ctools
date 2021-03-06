#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* extractImport(char* source) {
    char* regex = "^[ \\t]*import[ \\t]*.*[ .]([a-zA-Z0-9_]+) *;[ \\t]*";
    regmatch_t groups[2];
    regex_t regexCompiled;
    if (regcomp(&regexCompiled, regex, REG_EXTENDED)) return NULL;
    if (regexec(&regexCompiled, source, 2, groups, 0)) return NULL;
    if (groups[1].rm_so != -1 && groups[1].rm_eo != -1) {
        const unsigned startIndex = groups[1].rm_so;
        const unsigned endIndex = groups[1].rm_eo;
        const unsigned numNonNullChars = endIndex - startIndex;
        char* matchingSubstring = calloc(numNonNullChars+1, sizeof(char));
        memcpy(matchingSubstring, source + startIndex, numNonNullChars * sizeof(char));
        return matchingSubstring;
    } else {
        return NULL;
    }
}

#ifdef TEST_EXTRACT_IMPORT_CMDLINE
int main(int argc, char* argv[]) {
    if (argc != 2) return -1;

    char* import = extractImport(argv[1]);

    if (import) {
        printf("%s\n", import);
        free(import);
    }

    return 0;
}
#endif

void extractImports(char* fname) {
    char* line = NULL;
    size_t n = 0;
    FILE* fp = fopen(fname, "r");
    while (getline(&line, &n, fp)!=-1) {
        char* import = extractImport(line);
        if (import) {
            printf("%s\n", import);
            free(import);
        }
    }
    fclose(fp);
}

#ifdef TEST_EXTRACT_IMPORTS_CMDLINE
int main(int argc, char* argv[]) {
    if (argc != 2) return -1;
    
    extractImports(argv[1]);

    return 0;
}
#endif
