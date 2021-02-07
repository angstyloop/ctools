#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#if __STDC_VERSION__ < 199901L
#define restrict
#endif

#ifndef _POSIX_SOURCE
typedef long ssize_t;
#define SSIZE_MAX LONG_MAX
#endif

ssize_t getdelim(char**restrict lineptr, size_t*restrict n, int delimiter, FILE*restrict stream);
ssize_t getline(char**restrict lineptr, size_t*restrict n, FILE*restrict stream);

#define _GETDELIM_GROWBY 128
#define _GETDELIM_MINLEN 4

ssize_t getdelim(char**restrict lineptr, size_t*restrict n, int delimiter, FILE*restrict stream){
    char* buf;
    char* pos;
    int c;
    ssize_t bytes;
    if (lineptr == NULL || n == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (stream == NULL) {
        errno = EBADF;
        return -1;
    }
    buf = *lineptr;
    if (buf == NULL || *n < _GETDELIM_MINLEN) {
        if ((buf = realloc(*lineptr,  _GETDELIM_GROWBY)) == NULL) {
            /* ENOMEM */
            return -1;
        }
        *n = _GETDELIM_GROWBY;
        *lineptr = buf;
    }
    bytes = 0;
    pos = buf;
    while ((c = getc(stream)) != EOF) {
        if (++bytes >= SSIZE_MAX) {
            errno = EOVERFLOW;
            return -1;
        }
        if (bytes >= *n - 1) {
            buf = realloc(*lineptr, *n + _GETDELIM_GROWBY);
            if (buf == NULL) {
                /* ENOMEM */
                return -1;
            }
            *n += _GETDELIM_GROWBY;
            pos = buf + bytes - 1;
            *lineptr = buf;
        }
        *pos++ = (char) c;
        if (c == delimiter) {
            break;
        }
    }
    if (ferror(stream) || (feof(stream) && (bytes == 0))) {
            /* EOF, or getc() error */
            return -1;
    }
    *pos = '\0';
    return bytes;
}

ssize_t getline(char**restrict lineptr, size_t*restrict n, FILE*restrict stream) {
    return getdelim(lineptr, n, '\n', stream);
}

#ifdef _TEST_GETDELIM_STDIN
int main() {
    char* line = NULL;
    size_t n = 0;
    while (getline(&line, &n, stdin) > 0) {
        printf("%s", line);
    }
    return 0;
}
#elif _TEST_GETDELIM_FILE
int main() {
    char* line = NULL;
    size_t n = 0;
    FILE* fileptr = fopen("foo.txt", "r");
    while (getline(&line, &n, fileptr) > 0) {
        printf("%s", line);
    }
    return 0;
}
#endif
