#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* extractClass(const char*restrict source) {
    char* regex = "([^/ \t]+)\\.class[ \t]*\n?$";
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

#ifdef TEST_EXTRACT_CLASS_CMDLINE
int main(int argc, char* argv[]) {
    if (argc != 2) return -1;

    char* class = extractClass(argv[1]);

    if (class) {
        printf("%s\n", class);
        free(class);
    }

    return 0;
}
#endif

void extractClasses(const char*restrict jarPath) {
    int size = snprintf(NULL, 0, "jar tf %s", jarPath);
    char* cmd = calloc(size+1, sizeof(char));
    snprintf(cmd, size+1, "jar tf %s", jarPath);
    puts(cmd);
    FILE* f = popen(cmd, "r");
    char* line = NULL;
    size_t n = 0;
    while (getline(&line, &n, f)!=-1) {
        char* class = extractClass(line);
        if (class) {
            printf("%s\n", class);
            free(class);
        }
    }
    pclose(f);
}

#ifdef TEST_EXTRACT_CLASSES_CMDLINE
int main(int argc, char* argv[]) {
    if (argc != 2) return -1;

    extractClasses(argv[1]);

    return 0;
}
#endif
