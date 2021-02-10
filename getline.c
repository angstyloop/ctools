// POSIX-inspired sorta portable line getter
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#if __STDC_VERSION__ < 199901L
#define restrict
#endif

#define BUFSIZE_DELTA 128
#define BUFSIZE_MIN 4
#define BUFSIZE_MAX = LONG_MAX

// lineptr = the line buffer. the caller is responsible for freeing this, but the bright side is it can just be NULL initialized.
// n = the recommended initial size for the line buffer. this should be at most the length of the buffer, unless the buffer is NULL.
long getDelim(char**restrict lineptr, size_t*restrict n, int delimiter, FILE*restrict stream){
    if (lineptr == NULL || n == NULL || stream == NULL) return -1;

    char* buf = *lineptr;

    if (buf == NULL || *n < BUFSIZE_MIN) {
        if ((buf = realloc(*lineptr, BUFSIZE_DELTA)) == NULL) return -1;

        *n = BUFSIZE_DELTA;
        *lineptr = buf;
    }

    long bytes = 0;
    char* pos = buf;
    int c;

    while ((c = getc(stream)) != EOF) {
        if (++bytes >= LONG_MAX) return -1;

        if (bytes >= *n - 1) {
            if ((buf = realloc(*lineptr, *n + BUFSIZE_DELTA)) == NULL) return -1;

            *n += BUFSIZE_DELTA;
            pos = buf + bytes - 1;
            *lineptr = buf;
        }

        *pos++ = (char) c;

        if (c == delimiter) break;
    }

    if (ferror(stream) || (feof(stream) && (bytes == 0))) return -1;

    *pos = '\0';

    return bytes;
}

long getLine(char**restrict lineptr, size_t*restrict n, FILE*restrict stream) {
    return getDelim(lineptr, n, '\n', stream);
}

#ifdef TEST_GET_LINE_STDIN
int main() {
    char* line = NULL;
    size_t n = 0;
    while (getLine(&line, &n, stdin) > 0) {
        printf("%s", line);
    }
    free(line);
    return 0;
}
#elif TEST_GET_LINE_FILE
int main() {
    char* line = NULL;
    size_t n = 0;
    FILE* fileptr = fopen("foo.txt", "r");
    while (getLine(&line, &n, fileptr) > 0) {
        printf("%s", line);
    }
    return 0;
}
#endif
