#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getline.c>

void showUsage(char* functionName) {
    printf("Usage: %s <fileName>", functionName);
    exit(EXIT_FAILURE);
}

unsigned isMatch(regmatch_t match) {
    return match.rm_so != -1 && match.rm_eo;
}

char* extractImport(char* source) {
    char* regexString = "^[ \\t]*import[ \\t]+[a-zA-Z0-9_\\.]*\\.([a-zA-Z0-9_]+) *;[ \\t]*$";
    regmatch_t groups[2];
    regex_t regexCompiled;
    if (regcomp(&regexCompiled, regexString, REG_EXTENDED)) {
        printf("printIfImport(source=%s): Error compiling regular expression.\n", source);
        exit(EXIT_FAILURE);
    }
    if (regexec(&regexCompiled, source, 2, groups, 0)) {
        return "";
    }
    if (isMatch(groups[1])) {
        const unsigned startIndex = groups[1].rm_so;
        const unsigned endIndex = groups[1].rm_eo;
        const unsigned length = (endIndex - startIndex);
        char* matchingSubstring = calloc(length+1, sizeof(char));
        memcpy(matchingSubstring, source + startIndex, length * sizeof(char));
        matchingSubstring[length] = '\0';
        return matchingSubstring;
    } else {
        char* emptySubstring = calloc(1, sizeof(char));
        emptySubstring[0] = '\0';
        return emptySubstring;
    }
}

char* extractImports(char* fileName) {
    // need code to read a file line by line
}

int main(int argc, char* argv[]) {
    if (argc != 2) showUsage(argv[0]);
    else {
        char* import = extractImport(argv[1]);
        printf("%s\n", import);
        free(import);
    }
    return 0;
}
