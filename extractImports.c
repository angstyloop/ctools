#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned isMatch(regmatch_t match) {
    return match.rm_so != -1 && match.rm_eo != -1;
}

char* extractImport(char* source) {
    char* regexString = "^[ \\t]*import[ \\t]*.*[ .]([a-zA-Z0-9_]+) *;[ \\t]*";
    regmatch_t groups[2];
    regex_t regexCompiled;
    if (regcomp(&regexCompiled, regexString, REG_EXTENDED)) {
        printf("printIfImport(source=%s): Error compiling regular expression.\n", source);
        exit(EXIT_FAILURE);
    }
    if (regexec(&regexCompiled, source, 2, groups, 0)) {
        /* no match */
        return NULL;
    }
    if (isMatch(groups[1])) {
        /* match found */
        const unsigned startIndex = groups[1].rm_so;
        const unsigned endIndex = groups[1].rm_eo;
        const unsigned numNonNullChars = (endIndex - startIndex);
        char* matchingSubstring = calloc(numNonNullChars+1, sizeof(char));
        memcpy(matchingSubstring, source + startIndex, numNonNullChars * sizeof(char));
        matchingSubstring[numNonNullChars] = '\0';
        return matchingSubstring;
    } else {
        return NULL;
    }
}

#ifdef TEST_EXTRACT_IMPORT_CMDLINE
int main(int argc, char* argv[]) {
    if (argc != 2) return -1;

    char* import = extractImport(argv[1]);

    printf("%u", strlen(argv[1]));
    if (import != NULL) {
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
    while (getline(&line, &n, fp) > 0) {
        char* import = extractImport(line);
        if (import != NULL) {
            printf("%s\n", import);
            free(import);
        }
    }
    fclose(fp);
}

#ifdef TEST_EXTRACT_IMPORTS_CMDLINE
int main(int argc, char* argv[]) {
    if (argc !=2 ) {
        /* incorrect usage */
        return -1;
    } else {
        extractImports(argv[1]);
    }
}
#endif
